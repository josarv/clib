cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(clib
        VERSION 0.0.1
        DESCRIPTION "Library for the C programming language"
        HOMEPAGE_URL "https://github.com/josarv/clib"
        LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")

add_subdirectory(include/dynamic_array)
