cmake_minimum_required(VERSION 3.21)
project(chorgle)

option(WITH_WASM "Use Emscripten to build wasm target, if available." ON)
option(WITH_CCDB "Export compile commands, and ensure compat with Emscripten." ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ${WITH_CCDB})

include(ExternalProject)

ExternalProject_Add(resbuild_project
	PREFIX resbuild
	BUILD_ALWAYS 1
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resbuild
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/resbuild
	CMAKE_COMMAND cmake # force CMAKE on path
)

ExternalProject_Get_Property(resbuild_project install_dir)
include(resource.cmake)

add_resources(chorgle_res 
	shaders/sprite.frag
	shaders/sprite.vert
)

# default sources
set(PLATFORM_SOURCES
	window.c
)

set(CHORGLE_SOURCES
	main.c
)

if(EMSCRIPTEN AND WITH_WASM)
	message(STATUS "EMSCRIPTEN Detected, changing platform-specific source files")

	set(PLATFORM_SOURCES
		window_wasm.c
		gfx_wasm.c
	)

	add_executable(chorgle ${PLATFORM_SOURCES} ${CHORGLE_SOURCES})
	target_link_options(chorgle PRIVATE -sFULL_ES2 -sMAX_WEBGL_VERSION=2)

	if (WITH_CCDB)
		cmake_path(SET __EMSCRIPTEN_INCLUDE "${EMSCRIPTEN_ROOT_PATH}/cache/sysroot/include")
		cmake_path(NATIVE_PATH __EMSCRIPTEN_INCLUDE EMSCRIPTEN_INCLUDE_DIR)
		target_compile_options(chorgle PRIVATE -isystem ${EMSCRIPTEN_INCLUDE_DIR} -Ulinux -U_WIN32 -U__APPLE__ -D__EMSCRIPTEN__)
	endif()
else()
	add_executable(chorgle WIN32 MACOSX_BUNDLE ${PLATFORM_SOURCES} ${CHORGLE_SOURCES})
endif()

add_dependencies(chorgle chorgle_res)

if(NOT EMSCRIPTEN OR NOT WITH_WASM)
	add_subdirectory(thirdparty/glfw-3.3.8/)
	add_subdirectory(thirdparty/mimalloc-2.0.7/)
	add_subdirectory(thirdparty/glad/)

	find_package(OpenGL REQUIRED)

	target_link_libraries(chorgle PRIVATE OpenGL::GL glad glfw mimalloc-static)
endif()

