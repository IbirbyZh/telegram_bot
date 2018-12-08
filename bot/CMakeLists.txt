if (APPLE)
    link_directories(/usr/local/lib)
    link_directories(/usr/local/opt/openssl/lib)

    include_directories(/usr/local/include)
    include_directories(/usr/local/opt/openssl/include)
endif ()

include_directories(../client)
include_directories(../weather_client)

add_library(telegram_bot bot.cpp)

target_link_libraries(telegram_bot
        stdc++fs
        PocoNet PocoNetSSL PocoFoundation PocoJSON
        weather_client)