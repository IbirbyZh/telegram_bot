#include <exception>
#include <sstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Parser.h>

#include "client.h"

TGClient::Client::Client(const std::string &host, const std::string &token) : uri_(host) {
    uri_.setPath(uri_.getPath() + "bot" + token);

    if (uri_.getScheme().back() == 's') {
        session_ = std::make_unique<Poco::Net::HTTPSClientSession>(uri_.getHost());
    } else {
        session_ = std::make_unique<Poco::Net::HTTPClientSession>(uri_.getHost());
    }
    session_->setPort(uri_.getPort());
}

void TGClient::Client::CheckStatus(Poco::JSON::Object::Ptr response) const {
    if (!response->getValue<bool>("ok")) {
        throw std::logic_error("response result not ok");
    }
}

bool TGClient::Client::GetMe() const {
    try {
        Get(uri_.getPath() + "/getMe");
        return true;
    } catch (...) {
        return false;
    }
}

Poco::JSON::Object::Ptr TGClient::Client::Get(const std::string &path) const {
    Poco::Net::HTTPRequest request("GET", path);
    session_->sendRequest(request);

    return ReceiveResponse();
}

Poco::JSON::Object::Ptr TGClient::Client::Post(const std::string &path,
                                               const Poco::JSON::Object &data) const {
    Poco::Net::HTTPRequest request("POST", path);
    request.setContentType("application/json");
    std::ostringstream json;
    data.stringify(json);
    request.setContentLength(json.str().size());
    session_->sendRequest(request) << json.str();

    return ReceiveResponse();
}

void TGClient::Client::SendMessage(const TGClient::Message &message) const {
    Poco::JSON::Object message_obj;
    message_obj.set("text", message.text);
    message_obj.set("chat_id", message.chat_id);
    if (message.message_id != -1) {
        message_obj.set("reply_to_message_id", message.message_id);
    }
    Post(uri_.getPath() + "/sendMessage", message_obj);
}

std::vector<TGClient::Update> TGClient::Client::GetUpdates(std::optional<int64_t> timeout,
                                                           std::optional<int64_t> offset,
                                                           std::optional<int64_t> limit) const {
    Poco::URI uri = uri_;
    uri.setPath(uri.getPath() + "/getUpdates");
    if (offset.has_value()) {
        uri.addQueryParameter("offset", std::to_string(offset.value()));
    }
    if (timeout.has_value()) {
        uri.addQueryParameter("timeout", std::to_string(timeout.value()));
    }
    if (limit.has_value()) {
        uri.addQueryParameter("limit", std::to_string(limit.value()));
    }
    return InternalGetUpdates(uri.getPathAndQuery());
}

Poco::JSON::Object::Ptr TGClient::Client::ReceiveResponse() const {
    Poco::Net::HTTPResponse response;
    std::istream &body = session_->receiveResponse(response);

    Poco::JSON::Parser parser;
    auto result = parser.parse(body).extract<Poco::JSON::Object::Ptr>();

    CheckStatus(result);
    return result;
}

std::vector<TGClient::Update> TGClient::Client::InternalGetUpdates(const std::string &path) const {
    auto response = Get(path);
    std::vector<TGClient::Update> updates;
    for (auto result_var : *response->getArray("result")) {
        auto result = result_var.extract<Poco::JSON::Object::Ptr>();
        updates.push_back(Update{result->getValue<int64_t>("update_id"), std::nullopt});

        if (result->has("message")) {
            auto message = result->get("message").extract<Poco::JSON::Object::Ptr>();
            if (message->has("text")) {
                updates.back().message =
                    TGClient::Message{message->getObject("chat")->getValue<int64_t>("id"),
                                      message->getValue<int64_t>("message_id"),
                                      message->getValue<std::string>("text")};
            }
        }
    }
    return updates;
}
