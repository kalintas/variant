
cmake_minimum_required(VERSION 3.19.3)

project(VARIANT)

include(cmake/CompilerWarnings.cmake)
set(WARNINGS_AS_ERRORS OFF)

set(
    SRC_FILES
    src/variant.h
    src/main.cpp
)

set(EXECUTABLE_NAME "e")

add_executable(${EXECUTABLE_NAME} ${SRC_FILES} ${HEADER_FILES})

set_project_warnings(${EXECUTABLE_NAME})

set_target_properties(
    ${EXECUTABLE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)