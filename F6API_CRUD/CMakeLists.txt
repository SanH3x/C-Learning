cmake_minimum_required(VERSION 3.16)
project(crow_tasks_api LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gera build/compile_commands.json (usado pelo IntelliSense do VSCode e pelo clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

add_executable(tasks_api
  src/main.cpp
  src/database.cpp
)

target_include_directories(tasks_api PRIVATE src)
target_compile_options(tasks_api PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)
target_link_libraries(tasks_api PRIVATE SQLite::SQLite3 Threads::Threads)

# --- Crow -------------------------------------------------------------------
find_package(Crow CONFIG QUIET)
if(Crow_FOUND)
  message(STATUS "Crow encontrado via find_package")
  target_link_libraries(tasks_api PRIVATE Crow::Crow)
else()
  set(CROW_LOCAL_INCLUDE "${CMAKE_SOURCE_DIR}/third_party/crow/include")
  if(NOT EXISTS "${CROW_LOCAL_INCLUDE}/crow.h")
    message(FATAL_ERROR
      "Crow não encontrado. Instale via vcpkg ou copie crow.h e a pasta crow/ "
      "para third_party/crow/include/")
  endif()
  message(STATUS "Usando Crow local: ${CROW_LOCAL_INCLUDE}")

  # Crow precisa do Asio standalone (ou Boost.Asio)
  file(GLOB ASIO_LOCAL_CANDIDATES "${CMAKE_SOURCE_DIR}/third_party/asio*/include")
  find_path(ASIO_INCLUDE_DIR asio.hpp
    HINTS
      ${ASIO_LOCAL_CANDIDATES}
      "${CMAKE_SOURCE_DIR}/third_party/crow/include"
  )
  if(NOT ASIO_INCLUDE_DIR)
    message(FATAL_ERROR
      "asio.hpp não encontrado. No MSYS2: pacman -S mingw-w64-ucrt-x86_64-asio")
  endif()
  message(STATUS "Usando Asio: ${ASIO_INCLUDE_DIR}")

  target_include_directories(tasks_api SYSTEM PRIVATE
    "${CROW_LOCAL_INCLUDE}" "${ASIO_INCLUDE_DIR}")
endif()

if(WIN32)
  target_link_libraries(tasks_api PRIVATE ws2_32 mswsock)
  target_compile_definitions(tasks_api PRIVATE _WIN32_WINNT=0x0A00)
endif()
