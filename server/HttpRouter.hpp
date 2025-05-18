#pragma once
#include <string>
#include <unordered_map>
#include <functional>

class HttpRouter {
public:
    using Handler = std::function<std::string(const std::string& method, const std::string& body)>;

    HttpRouter();

    std::string route(const std::string& path, const std::string& method, const std::string& body) const;

private:
    std::unordered_map<std::string, Handler> routes_;
    void init_routes(); // định nghĩa các route ở đây
};
