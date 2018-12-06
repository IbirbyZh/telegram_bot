#include <iostream>

#include "bot.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Using \n ./run_bot HOST TOKEN STATE_FILE\n";
        return 1;
    }
    auto bot = Bot(argv[1], argv[2], argv[3]);
    bot.Loop();
    return 0;
}