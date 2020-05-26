cmake_minimum_required (VERSION 3.8)

project (basic-pistache LANGUAGES CXX VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(Pistache CONFIG REQUIRED)
find_package(log4cplus REQUIRED 2.0)

file(GLOB SRC
	*.cpp
	*.h*
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
    pistache_static
    log4cplus::log4cplus
)

