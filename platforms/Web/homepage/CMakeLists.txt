cmake_minimum_required(VERSION 3.13)

project(webrogue_homepage)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(CLANG_HAS_WASM FALSE)
set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../..)
set(WEBROGUE_MOD_NAMES core basecontent)

include(../../../make_webrogue.cmake)

add_compile_options(-sUSE_SDL=2 -sUSE_SDL_TTF=2 -fno-rtti)
add_link_options(-sUSE_SDL=2 -sUSE_SDL_TTF=2 -sASYNCIFY -sALLOW_MEMORY_GROWTH -sASYNCIFY_STACK_SIZE=65535 -sFORCE_FILESYSTEM -fno-rtti)

make_webrogue_output(
    LIB_NAME webrogue_output_sdl
    TYPE SDL
    STATIC
)
make_webrogue_core(
    LIB_NAME webrogue_core
    STATIC
    NO_WASM
)

#libuv
set(LIBUV_BUILD_SHARED OFF)
add_subdirectory(${WEBROGUE_ROOT_PATH}/external/libuv libuv)
target_link_libraries(webrogue_core uv)
target_include_directories(webrogue_core PRIVATE ${WEBROGUE_ROOT_PATH}/external/libuv/include) 

target_link_libraries(webrogue_output_sdl webrogue_core)
make_webrogue_runtime(
    LIB_NAME webrogue_runtime_embedded
    TYPE EMBEDDED
    STATIC
    EMBEDDED_RUNTIME_MODS basecontent core
)
add_executable(webrogue_homepage "./main.cpp")
target_link_libraries(webrogue_homepage webrogue_output_sdl webrogue_core webrogue_runtime_embedded idbfs.js)
target_include_directories(webrogue_homepage PRIVATE ${WEBROGUE_ROOT_PATH}/embedded_resources)

add_custom_target(pack_artifacts
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${WEBROGUE_ROOT_PATH}/artifacts
    COMMAND ${CMAKE_COMMAND} -E make_directory ${WEBROGUE_ROOT_PATH}/artifacts
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/platforms/Web/homepage/index.html ${WEBROGUE_ROOT_PATH}/artifacts/index.html
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/webrogue_homepage.js ${WEBROGUE_ROOT_PATH}/artifacts/webrogue_homepage.js
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/webrogue_homepage.wasm ${WEBROGUE_ROOT_PATH}/artifacts/webrogue_homepage.wasm
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/platforms/Web/noscript/noscript.png ${WEBROGUE_ROOT_PATH}/artifacts/noscript/noscript.png
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/platforms/Web/noscript/index.html ${WEBROGUE_ROOT_PATH}/artifacts/noscript/index.html
    DEPENDS webrogue_homepage
)
