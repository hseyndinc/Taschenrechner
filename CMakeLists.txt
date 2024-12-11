cmake_minimum_required(VERSION 3.10)

project(Taschenrechner)

include_directories(include)

set(SOURCES
        src/main.cpp
        src/Calculator.cpp
)

add_executable(Taschenrechner ${SOURCES})
