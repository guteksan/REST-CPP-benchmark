cmake_minimum_required (VERSION 3.8)

project (basic-restbed LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(log4cplus REQUIRED 2.0)
find_package(Restbed REQUIRED)

include_directories(${Restbed_INCLUDE_DIRS})

file(GLOB src
	*.cpp
	*.h*
)

add_executable(${PROJECT_NAME} ${src}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    	log4cplus::log4cplus
	${Restbed_LIBS}
)
