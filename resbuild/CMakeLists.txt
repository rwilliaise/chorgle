cmake_minimum_required(VERSION 3.21)
project(resbuild)

add_executable(resbuild main.c)

install(TARGETS resbuild EXPORT resbuild
		RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
