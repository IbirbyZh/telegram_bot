#include <exception>
#include <sstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Parser.h>

#include "weather_client.h"

WeatherClient::Client::Client(const std::string &host, const std::string &token)
    : token_(token), uri_(host) {
    uri_.setPath(uri_.getPath() + "v1/forecast");

    if (uri_.getScheme().back() == 's') {
        session_ = std::make_unique<Poco::Net::HTTPSClientSession>(uri_.getHost());
    } else {
        session_ = std::make_unique<Poco::Net::HTTPClientSession>(uri_.getHost());
    }
    session_->setPort(uri_.getPort());
}

WeatherClient::Weather WeatherClient::Client::GetWeather(double lon, double lat) const {
    Poco::URI uri = uri_;
    uri.addQueryParameter("lon", std::to_string(lon));
    uri.addQueryParameter("lat", std::to_string(lat));
    Poco::Net::HTTPRequest request("GET", uri.getPathAndQuery());
    request.set("X-Yandex-API-Key", token_);
    session_->sendRequest(request);

    Poco::Net::HTTPResponse response;
    std::istream &body = session_->receiveResponse(response);

    Poco::JSON::Parser parser;
    auto result = parser.parse(body).extract<Poco::JSON::Object::Ptr>();

    return Weather{result->getObject("fact")->getValue<double>("temp"),
                   result->getObject("fact")->getValue<double>("feels_like")};
}
