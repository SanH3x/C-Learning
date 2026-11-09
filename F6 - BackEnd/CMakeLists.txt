cmake_minimum_required(VERSION 3.15)
project(api_crow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(api_crow src/main.cpp)

# Headers locais (sem vcpkg): crow.h + pasta crow/ e asio.hpp + pasta asio/
target_include_directories(api_crow PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/crow/include
    ${CMAKE_SOURCE_DIR}/third_party/asio/include
)

# Crow usa o Asio standalone (sem Boost)
target_compile_definitions(api_crow PRIVATE ASIO_STANDALONE)

# Threads (necessario para multithreaded()) e sockets no Windows/MinGW
find_package(Threads REQUIRED)
target_link_libraries(api_crow PRIVATE Threads::Threads)
if(WIN32)
    target_link_libraries(api_crow PRIVATE ws2_32 mswsock)
endif()
