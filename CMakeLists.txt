cmake_minimum_required(VERSION 3.16)
project(twiq VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Blend2D REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/CanvasWidget.cpp
    src/CanvasWidget.h
    src/SpinnerTemplates.h
    src/TemplateExplorerDialog.cpp
    src/TemplateExplorerDialog.h
)

add_executable(twiq ${PROJECT_SOURCES})

target_link_libraries(twiq PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    blend2d
    gif
)