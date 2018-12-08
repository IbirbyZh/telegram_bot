#pragma once

#include <string>
#include <memory>
#include <optional>

#include <Poco/URI.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>

namespace WeatherClient {

struct Weather {
    double temp;
    double feels_like;
};

class Client {
public:
    Client(const std::string &host, const std::string &token);
    Weather GetWeather(double lon=37.620393, double lat=55.75396) const;


private:
    std::string token_;
    Poco::URI uri_;
    std::unique_ptr<Poco::Net::HTTPClientSession> session_;
};
}  // namespace TGClient
