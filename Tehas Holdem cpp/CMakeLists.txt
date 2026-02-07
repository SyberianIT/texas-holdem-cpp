cmake_minimum_required(VERSION 3.10)
project(TexasHoldem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(texas_holdem
    main.cpp
    koloda.cpp
    igrok.cpp
    ocenka_ruki.cpp
    igra.cpp
)

target_include_directories(texas_holdem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})