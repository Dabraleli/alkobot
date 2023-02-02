cmake_minimum_required(VERSION 3.16)
project(alkobot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Sql
        REQUIRED)

add_subdirectory(tgbot-cpp)

add_executable(alkobot main.cpp Database.cpp Database.h FishachBot.cpp FishachBot.h api/Fish/Fish.cpp api/Fish/Fish.h api/Beer/Beer.cpp api/Beer/Beer.h utils/Utils.cpp utils/Utils.h)
target_link_libraries(alkobot
        Qt5::Core
        Qt5::Sql
        TgBot
        )
