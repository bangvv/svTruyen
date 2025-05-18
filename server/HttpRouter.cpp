#include "HttpRouter.hpp"
#include <nlohmann/json.hpp>

HttpRouter::HttpRouter() {
    init_routes();
}

std::string HttpRouter::route(const std::string& path, const std::string& method, const std::string& body) const {
    auto it = routes_.find(path);
    if (it != routes_.end()) {
        return it->second(method, body);
    }
    return "404 - Not Found";
}

void HttpRouter::init_routes() {
    routes_["/a"] = [](const std::string& method, const std::string& body) {
        return "You called A";
    };
    routes_["/b"] = [](const std::string& method, const std::string& body) {
        return "You called B";
    };
    routes_["/login"] = [](const std::string& method, const std::string& body) {
		std::string reply = "Login failed!";
		if (body.length() > 0) {
			auto json = nlohmann::json::parse(body);
			std::string username = json["username"];
			std::string password = json["password"];

			reply = (username == "admin" && password == "123456")
								? "Login successful!"
								: "Login failed!";
		}
        return reply + " method: "+method+" body: "+body;
    };
}
