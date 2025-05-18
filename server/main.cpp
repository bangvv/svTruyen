#include "HttpRouter.hpp"
#include "HttpServer.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::string ip = "127.0.0.1";
    int port = 8080;

    if (argc == 3) {
        ip = argv[1];
        try {
            port = std::stoi(argv[2]);
        } catch (...) {
            std::cerr << "Invalid port format, using default 8080\n";
            port = 8080;
        }
    } else {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        std::cout << "No valid input provided. Defaulting to http://127.0.0.1:8080\n";
    }

    std::cout << "Starting server at " << ip << ":" << port << "\n";

    HttpRouter router;
    HttpServer server(ip, port, router);
    server.run();

    return 0;
}
