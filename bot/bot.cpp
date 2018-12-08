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
        auto weather =weather_client_.GetWeather();
        message_text << "Winter Is Coming\n";
        message_text << "Now: " << weather.temp << "\n";
        message_text << "Feels like: " << weather.feels_like << "\n";
        SendMessage(message.chat_id, message_text.str());
    } else if (message.text == "/styleguide") {
        SendMessage(message.chat_id,
                    "Я бы пошутил на тему ревью, но я пока не до конца осоздал что это.\n"
                    "Я до сих пор не прошел ни одно...");
    } else if (message.text == "/stop") {
        return false;
    } else if (message.text == "/crash") {
        abort();
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
