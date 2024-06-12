cmake_minimum_required(VERSION 3.28)
project(WLEDControllerCPP)


set(CMAKE_CXX_STANDARD 17)

add_executable(WLEDControllerCPP main.cpp)

target_link_libraries(WLEDControllerCPP ws2_32)
