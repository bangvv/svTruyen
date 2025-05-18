#include "HttpServer.hpp"
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;

HttpServer::HttpServer(const std::string& address, unsigned short port, const HttpRouter& router)
    : ioc_(std::thread::hardware_concurrency()),
      acceptor_(ioc_, tcp::endpoint(net::ip::make_address(address), port)),
      pool_(std::thread::hardware_concurrency()),
      router_(router) {}

void HttpServer::run() {
    std::cout << "Server is running on http://" << acceptor_.local_endpoint() << " with "
              << std::thread::hardware_concurrency() << " threads\n";

    do_accept();
    ioc_.run();
    pool_.join();
}

void HttpServer::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            net::post(pool_, [this, s = std::move(socket)]() mutable {
				handle_session(std::move(s));
            });
        } else {
            std::cerr << "Accept error: " << ec.message() << "\n";
        }

        do_accept(); // Tiếp tục nhận kết nối
    });
}

void HttpServer::handle_session(tcp::socket socket) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        std::string method = req.method_string().to_string();
        std::string path = req.target().to_string();
        std::string body = req.body();

        std::string result = router_.route(path, method, body);

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "CppServer");
        res.set(http::field::content_type, "text/plain");
		res.set(http::field::access_control_allow_origin, "*");
        res.keep_alive(req.keep_alive());
        res.body() = result;
        res.prepare_payload();

        http::write(socket, res);
        socket.shutdown(tcp::socket::shutdown_send);
    } catch (std::exception& e) {
        std::cerr << "Session error: " << e.what() << "\n";
    }
}
