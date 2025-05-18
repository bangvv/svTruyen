#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>
#include "HttpRouter.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;

class HttpServer {
public:
    HttpServer(const std::string& address, unsigned short port, const HttpRouter& router);
    void run();

private:
    void do_accept();
	void handle_session(tcp::socket socket);

    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::thread_pool pool_;
    const HttpRouter& router_;
    int count_ = 0;
};
