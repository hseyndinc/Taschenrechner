cmake_minimum_required(VERSION 3.16)
project(futuristic_calculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 Pfad
set(CMAKE_PREFIX_PATH "/Users/hseyndinc/Qt/6.7.3/macos")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(calculator_gui
        main.cpp
        calculator_gui.cpp
        calculator_gui.h
        calculator_gui.ui
)

target_link_libraries(calculator_gui PRIVATE Qt6::Widgets)