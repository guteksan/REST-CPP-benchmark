cmake_minimum_required (VERSION 3.8)

project (basic-cpprestsdk LANGUAGES CXX VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(cpprestsdk REQUIRED)
find_package(log4cplus REQUIRED 2.0)

message(OPENSSL LIBS " ${OPENSSL_LIBRARIES}")

file(GLOB SRC
	*.cpp
	*.h*
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
   cpprestsdk::cpprest
   log4cplus::log4cplus
   ${OPENSSL_LIBRARIES}
   ${Boost_LIBRARIES}
)

