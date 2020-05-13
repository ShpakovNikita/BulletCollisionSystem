cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(VERSION 2.8)

set(TARGET_NAME TestTask)
project(${TARGET_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Cmake")
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/Bin")
set(TASK_SOURCE_PATH "${CMAKE_SOURCE_DIR}/Source")

include_directories("External")
include_directories("External/imgui")

file(GLOB IMGUI_HEADER_FILES LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/External/imgui/examples/*sdl*.h"
    "${CMAKE_SOURCE_DIR}/External/imgui/examples/*opengl3*.h"
)
file(GLOB IMGUI_SOURCE_FILES LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/External/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/External/imgui/examples/*sdl*.cpp"
    "${CMAKE_SOURCE_DIR}/External/imgui/examples/*opengl3*.cpp"
)

file(GLOB GL3W_HEADER_FILES LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/External/gl3w/GL/*.h"
)
file(GLOB GL3W_SOURCE_FILES LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/External/gl3w/src/*.c"
)

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

add_definitions(-DASSETS_DIR=\"${CMAKE_SOURCE_DIR}/Assets/\")

add_executable(${TARGET_NAME} 
    ${SOURCE_FILES} 
    ${HEADER_FILES} 
    ${IMGUI_SOURCE_FILES}
    ${IMGUI_HEADER_FILES}
    ${GL3W_HEADER_FILES}
    ${GL3W_SOURCE_FILES}
 )
source_group(TREE ${TASK_SOURCE_PATH} FILES ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

target_include_directories(${TARGET_NAME}
    PRIVATE
    ${TASK_SOURCE_PATH}/Engine
    ${TASK_SOURCE_PATH}/Client
    External/gl3w/
    External/SDL2/
    External/imgui/
)

target_compile_definitions(${TARGET_NAME}
    PRIVATE NOMINMAX
)

if (MSVC)
    file(GLOB_RECURSE DLLS "${TASK_LIBS_PATH}/*.dll")
    message(${DLLS})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${DLLS}
            $<TARGET_FILE_DIR:${TARGET_NAME}>)
endif()
