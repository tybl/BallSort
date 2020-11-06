################################################################################
# License: The Unlicense (https://unlicense.org)
################################################################################
cmake_minimum_required(VERSION 3.14) # FetchContent_MakeAvailable

project(BubbleSort
	VERSION 1.0.0
	DESCRIPTION "Solves to ball/bubble sorting games"
	LANGUAGES CXX)

add_executable(sort src/main.cpp)
target_compile_options(sort PRIVATE -Wall -Wextra -Wpedantic)
