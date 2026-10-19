
cmake_minimum_required(VERSION 3.16)

project(MovieScriptTranslator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(MovieScriptTranslator
    src/main.cpp
    src/translator.cpp
    src/lexer.cpp
    src/parser.cpp
    src/semantic.cpp

)

target_include_directories(MovieScriptTranslator PRIVATE
    include
)

target_link_libraries(MovieScriptTranslator PRIVATE
    Qt6::Widgets
)
