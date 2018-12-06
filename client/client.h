#pragma once

#include <string>
#include <memory>
#include <optional>

#include <Poco/URI.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>

namespace TGClient {
struct Message {
    int64_t chat_id;
    int64_t message_id;
    std::string text;
};

struct Update {
    int64_t update_id;
    std::optional<Message> message;
};

class Client {
public:
    Client(const std::string &host, const std::string &token);
    bool GetMe() const;
    std::vector<Update> GetUpdates(std::optional<int64_t> timeout = std::nullopt,
                                   std::optional<int64_t> offset = std::nullopt,
                                   std::optional<int64_t> limit = std::nullopt) const;
    void SendMessage(const Message &message) const;

private:
    Poco::URI uri_;
    std::unique_ptr<Poco::Net::HTTPClientSession> session_;

    Poco::JSON::Object::Ptr Get(const std::string &path) const;
    Poco::JSON::Object::Ptr Post(const std::string &path, const Poco::JSON::Object &data) const;
    Poco::JSON::Object::Ptr ReceiveResponse() const;
    std::vector<Update> InternalGetUpdates(const std::string &path) const;

    void CheckStatus(Poco::JSON::Object::Ptr response) const;
};
}  // namespace TGClient
