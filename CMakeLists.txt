cmake_minimum_required(VERSION 3.21)
project(chorgle)

option(WITH_WASM "Use Emscripten to build wasm target, if available." ON)
option(WITH_CCDB "Export compile commands, and ensure compat with Emscripten." ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ${WITH_CCDB})

include(ExternalProject)

ExternalProject_Add(resbuild
	BUILD_ALWAYS 1
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/resbuild
	BINARY_DIR ${CMAKE_BINARY_DIR}/resbuild
	CMAKE_COMMAND cmake # force CMAKE on path
)
ExternalProject_Add_StepTargets(resbuild build)

ExternalProject_Add(glfw
	PREFIX thirdparty
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.8
	UPDATE_COMMAND ""

	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/glfw"
	CMAKE_COMMAND cmake
)

add_library(glfw3-static STATIC IMPORTED)
add_dependencies(glfw3-static glfw)

set_target_properties(glfw3-static PROPERTIES
	IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/glfw/lib/libglfw3.a)

include(resource.cmake)

add_resources(chorgle_res 
	shaders/sprite.frag
	shaders/sprite.vert
)

set(CHORGLE_SOURCES
	main.c
	window.c
	graphics.c
)

if(EMSCRIPTEN AND WITH_WASM)
	message(STATUS "EMSCRIPTEN Detected, changing platform-specific source files")

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
	add_subdirectory(thirdparty/mimalloc-2.0.7/)
	find_package(OpenGL REQUIRED)

	target_link_libraries(chorgle PRIVATE OpenGL::GL mimalloc-static)
endif()
target_link_libraries(chorgle PRIVATE glfw3-static)

