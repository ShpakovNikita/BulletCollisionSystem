cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(VERSION 2.8)
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/Bin")
set(TASK_SOURCE_PATH "${CMAKE_SOURCE_DIR}/Source")

set(TARGET_NAME TestTask)
project(${TARGET_NAME})

file(GLOB IMGUI_SOURCE_FILES "${CMAKE_SOURCE_DIR}/External/imgui/*.cpp")

if(WIN32)
    set(TASK_LIBS_PATH ${CMAKE_SOURCE_DIR}/Libs/Win32)
else(WIN32)
    message("Non win32 systems not supported from the box, some errors may occur!")
endif()

find_package(SDL2 REQUIRED)

if(WIN32)
	if (NOT SDL2_FOUND)
		find_library(SDL2_LIB NAMES sdl2 PATHS ${TASK_LIBS_PATH}/SDL2)
        find_library(SDL2_LIB_MAIN NAMES sdl2main PATHS ${TASK_LIBS_PATH}/SDL2)
        set(SDL2_LIBRARY ${SDL2_LIB} ${SDL2_LIB_MAIN})
        
		if (SDL2_LIBRARY)
			set(SDL2_FOUND ON)
			MESSAGE("Using bundled Sdl2 library version")
		endif()
	endif()
endif(WIN32)

if (NOT SDL2_FOUND)
	message(FATAL_ERROR "Could not find Sdl2 library!")
else()
	message(STATUS ${SDL2_LIBRARY})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
endif()

file(GLOB_RECURSE SOURCE_FILES "${TASK_SOURCE_PATH}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${TASK_SOURCE_PATH}/*.hpp")

target_include_directories(${TARGET_NAME}
    External
    External/imgui
)

target_compile_definitions(${TARGET_NAME}
    PRIVATE NOMINMAX
)

add_executable(${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${IMGUI_SOURCE_FILES})
