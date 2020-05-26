cmake_minimum_required (VERSION 3.8)

project (basic-httplib LANGUAGES CXX VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(log4cplus REQUIRED 2.0)

file(GLOB SRC
	*.cpp
	*.h*
)

include_directories(httplib)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME}
   	log4cplus::log4cplus
	Threads::Threads
)
