if (APPLE)
    link_directories(/usr/local/lib)
    link_directories(/usr/local/opt/openssl/lib)

    include_directories(/usr/local/include)
    include_directories(/usr/local/opt/openssl/include)
endif ()

include_directories(../bot)
include_directories(../client)

add_executable(run_bot main.cpp)

target_link_libraries(run_bot
        stdc++fs
        PocoNet PocoNetSSL PocoFoundation PocoJSON
        telegram_bot telegram_client)
