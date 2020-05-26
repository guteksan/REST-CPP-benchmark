cmake_minimum_required (VERSION 3.8)

project (basic-crow LANGUAGES CXX VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(log4cplus REQUIRED 2.0)
find_package(Boost 1.54 COMPONENTS system thread REQUIRED )

file(GLOB SRC
	*.cpp
	*.h*
)

include_directories(crow)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
   log4cplus::log4cplus
   Boost::system
   Boost::thread
)

