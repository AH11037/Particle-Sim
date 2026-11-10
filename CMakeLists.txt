
cmake_minimum_required(VERSION 3.10.0)

project(ParticleSim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
add_executable(ParticleSim main.cpp)

target_link_libraries(ParticleSim PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
