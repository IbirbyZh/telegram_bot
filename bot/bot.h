#pragma once

#include <string>
#include <random>
#include <filesystem>

#include "client.h"

class Bot {
    struct State {
        int64_t update_id;
    };

public:
    Bot(const std::string &host, const std::string &token, const std::string &state_file);
    void Loop();

private:
    State state_;
    std::mt19937 generator_;
    std::uniform_int_distribution<int> distribution_;
    TGClient::Client client_;
    std::filesystem::path state_file_;

    void InitState();
    bool ProcessMessage(const TGClient::Message &message);
    void UpdateState(int64_t update_id);
    void SendMessage(int64_t chat_id, std::string &&message);
};