cmake_minimum_required(VERSION 3.16)
project(futuristic_calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 Pfad für macOS mit Homebrew
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# Finde die benötigten Qt Pakete
find_package(Qt6 COMPONENTS
        Widgets
        Multimedia
        MultimediaWidgets
        REQUIRED
)

# Definiere das ausführbare Programm und seine Quelldateien
add_executable(calculator_gui
        main.cpp
        calculator_gui.cpp
        calculator_gui.h
        calculator_gui.ui
)

# Verlinke die Qt-Bibliotheken
target_link_libraries(calculator_gui PRIVATE
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
)