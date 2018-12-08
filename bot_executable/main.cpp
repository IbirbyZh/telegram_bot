#include <iostream>

#include "bot.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Using \n ./run_bot HOST TOKEN STATE_FILE WEATHER_HOST WEATHER_TOKEN\n";
        return 1;
    }
    auto bot = Bot(argv[1], argv[2], argv[3], argv[4], argv[5]);
    bot.Loop();
    return 0;
}