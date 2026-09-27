cmake_minimum_required(VERSION 3.25)

project(speedrun)

find_package(geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    GD_VERSION 2.2081
    
    SOURCES
        src/main.cpp
)
