#include <fstream>

#include "bot.h"

Bot::Bot(const std::string &host, const std::string &token, const std::string &state_file,
         const std::string &weather_host, const std::string &weather_token)
    : state_(),
      generator_(42),
      distribution_(0, 10),
      client_(host, token),
      weather_client_(weather_host, weather_token),
      state_file_(state_file.data()) {
}

void Bot::Loop() {
    InitState();
    bool loop = true;
    while (loop) {
        std::vector<TGClient::Update> updates = client_.GetUpdates(10, state_.update_id + 1);
        for (const TGClient::Update &update : updates) {
            UpdateState(update.update_id);
            if (update.message.has_value()) {
                loop = ProcessMessage(update.message.value());
            }
        }
    }
}

void Bot::InitState() {
    if (std::filesystem::exists(state_file_)) {
        std::ifstream state_stream(state_file_);
        state_stream >> state_.update_id;
    } else {
        state_.update_id = 0;
    }
}

bool Bot::ProcessMessage(const TGClient::Message &message) {
    if (message.text == "/random") {
        SendMessage(message.chat_id, std::to_string(distribution_(generator_)));
    } else if (message.text == "/weather") {
        std::stringstream message_text;
        auto weather = weather_client_.GetWeather();
        message_text << "Winter Is Coming\n";
        message_text << "Now: " << weather.temp << "\n";
        message_text << "Feels like: " << weather.feels_like << "\n";
        SendMessage(message.chat_id, message_text.str());
    } else if (message.text == "/stop") {
        return false;
    } else if (message.text == "/crash") {
        abort();
    } else if (message.text == "/help" || message.text == "/start") {
        std::stringstream message_text;
        message_text << "Source code: http://github.com/IbirbyZh/telegram_bot\n";
        message_text << "/weather to get current weather in Moscow\n";
        message_text << "/random to recive random number\n";
        SendMessage(message.chat_id, message_text.str());
    } else if (message.text.substr(0, 8) == "/weather") {
        std::stringstream message_in_text(message.text);
        std::string s;
        message_in_text >> s;
        double lon, lat;
        message_in_text >> lon >> lat;

        auto weather = weather_client_.GetWeather(lon, lat);
        std::stringstream message_text(message.text);
        message_text << "Winter Is Coming\n";
        message_text << "Now: " << weather.temp << "\n";
        message_text << "Feels like: " << weather.feels_like << "\n";
        SendMessage(message.chat_id, message_text.str());
    }
    return true;
}

void Bot::UpdateState(int64_t update_id) {
    state_.update_id = update_id;
    std::ofstream state_stream(state_file_);
    state_stream << update_id;
}

void Bot::SendMessage(int64_t chat_id, std::string &&message) {
    client_.SendMessage(TGClient::Message{chat_id, -1, message});
}
