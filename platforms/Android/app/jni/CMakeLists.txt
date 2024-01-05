cmake_minimum_required(VERSION 3.13)

project(webrogue)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
set(CLANG_HAS_WASM FALSE)
set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../..)
set(WEBROGUE_MOD_NAMES core basecontent)

include(../../../../make_webrogue.cmake)

set(SDL_SHARED ON)
set(SDL_STATIC OFF)
add_subdirectory(../../../../external/SDL SDL2)
#find_library(SDL2 SDL2)
set(SDL2TTF_VENDORED ON)
set(SDL2TTF_INSTALL OFF)
add_subdirectory(${WEBROGUE_ROOT_PATH}/external/SDL_ttf sdl_ttf)

add_subdirectory(${WEBROGUE_ROOT_PATH}/platforms/Android/app/jni/wamr wamr)

make_webrogue_output(
    LIB_NAME webrogue_output_sdl
    TYPE SDL
    STATIC
)
make_webrogue_core(
    LIB_NAME webrogue_core
    STATIC
)
set(WEBROGUE_COMPACT_LINKER_STATIC ON)
include(${WEBROGUE_ROOT_PATH}/cmake/make_compact_linker.cmake)
target_link_libraries(webrogue_core compact_linker)

#libuv
set(LIBUV_BUILD_SHARED OFF)
add_subdirectory(${WEBROGUE_ROOT_PATH}/external/libuv libuv)
target_link_libraries(webrogue_core uv_a)
target_include_directories(webrogue_core PRIVATE ${WEBROGUE_ROOT_PATH}/external/libuv/include) 

make_webrogue_runtime(
    LIB_NAME webrogue_runtime_wamr
    TYPE WAMR
    STATIC
)
target_include_directories(
    webrogue_output_sdl PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../../../external/SDL/include 
    ${CMAKE_CURRENT_LIST_DIR}/../../../../external/SDL_ttf
)
target_link_libraries(webrogue_output_sdl SDL2_ttf SDL2)
target_link_libraries(webrogue_runtime_wamr vmlib webrogue_core)


add_library(webrogue SHARED ${CMAKE_CURRENT_LIST_DIR}/android_main.cpp)
target_link_libraries(
    webrogue
    webrogue_output_sdl
    webrogue_core
    webrogue_runtime_wamr
    log
)

#mods
add_custom_target(copy_resources_to_raw
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${WEBROGUE_ROOT_PATH}/mods/basecontent/basecontent.wrmod ${WEBROGUE_ROOT_PATH}/platforms/Android/app/src/main/res/raw/basecontent
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${WEBROGUE_ROOT_PATH}/mods/core/core.wrmod ${WEBROGUE_ROOT_PATH}/platforms/Android/app/src/main/res/raw/core
    DEPENDS ${WEBROGUE_ROOT_PATH}/mods/core/core.wrmod ${WEBROGUE_ROOT_PATH}/mods/basecontent/basecontent.wrmod
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${WEBROGUE_ROOT_PATH}/external/SDL/android-project/app/src/main/java/org/libsdl/app/ ${WEBROGUE_ROOT_PATH}/platforms/Android/app/src/main/java/org/libsdl/app
)

target_include_directories(webrogue PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../../../external/SDL/include ${CMAKE_CURRENT_LIST_DIR}/../../../../external/SDL_ttf ${WEBROGUE_ROOT_PATH}/src/)
target_compile_definitions(webrogue PRIVATE OUTPUT_SDL)

add_dependencies(webrogue copy_resources_to_raw)
