cmake_minimum_required(VERSION 3.10)
project(CppTaskAppPostgres)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
include_directories(${PQXX_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

add_executable(CppTaskAppPostgres src/main.cpp src/taskmanager.cpp src/taskmanagerui.cpp src/consolepainter.cpp)

target_link_libraries(CppTaskAppPostgres ${PQXX_LIBRARIES} pthread)

add_custom_target(
    format
    COMMAND clang-format -i --style=Google src/*.cpp src/*.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting code using clang-format"
)

add_custom_target(
    check-style
    COMMAND clang-format -n --style=Google src/*.cpp src/*.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Checking code style using clang-format"
)

add_dependencies(CppTaskAppPostgres format check-style)