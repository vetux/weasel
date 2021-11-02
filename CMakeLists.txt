cmake_minimum_required(VERSION 3.17)

project(Weasel)

if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE -O3)
else()
    message(FATAL_ERROR "Weasel only works on linux.")
endif()

include(cmake/system.cmake)
include(cmake/frontend-qt.cmake)