if (APPLE)
    link_directories(/usr/local/lib)
    link_directories(/usr/local/opt/openssl/lib)

    include_directories(/usr/local/include)
    include_directories(/usr/local/opt/openssl/include)
endif ()

add_library(telegram_client client.cpp)
