cmake_minimum_required(VERSION 3.13)

project(webrogue_game)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(CLANG_HAS_WASM FALSE)
set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../..)
set(WEBROGUE_MOD_NAMES core basecontent)

include(../../../make_webrogue.cmake)

add_compile_options(
    -sUSE_SDL=2 
    -sUSE_SDL_TTF=2 
    -fno-rtti 
    -sWASM_BIGINT
    "-sASYNCIFY_IMPORTS=[initWasmModule, execFunc, continueFuncExecution, readModMem, writeModMem]"
)
add_link_options(
    -sUSE_SDL=2 
    -sUSE_SDL_TTF=2 
    -sFETCH 
    -sASYNCIFY 
    -sALLOW_MEMORY_GROWTH 
    -sASYNCIFY_STACK_SIZE=65535
    -sFORCE_FILESYSTEM 
    -fno-rtti 
    -sWASM_BIGINT
    --js-library ${CMAKE_CURRENT_LIST_DIR}/vmlib.js 
    "-sASYNCIFY_IMPORTS=[initWasmModule, execFunc, continueFuncExecution, readModMem, writeModMem]"
)

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

target_link_libraries(webrogue_output_sdl webrogue_core)
make_webrogue_runtime(
    LIB_NAME webrogue_runtime_web
    TYPE WEB
    STATIC
)

set(WEBROGUE_COMPACT_LINKER_STATIC ON)
include(${WEBROGUE_ROOT_PATH}/cmake/make_compact_linker.cmake)

add_executable(webrogue_game "./main.cpp")
target_link_libraries(webrogue_game webrogue_output_sdl webrogue_core webrogue_runtime_web compact_linker idbfs.js)
target_include_directories(webrogue_game PRIVATE ${WEBROGUE_ROOT_PATH}/embedded_resources)

add_custom_target(pack_artifacts
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${WEBROGUE_ROOT_PATH}/artifacts
    COMMAND ${CMAKE_COMMAND} -E make_directory ${WEBROGUE_ROOT_PATH}/artifacts
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/platforms/Web/game/index.html ${WEBROGUE_ROOT_PATH}/artifacts/index.html
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/platforms/Web/game/worker.js ${WEBROGUE_ROOT_PATH}/artifacts/worker.js
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/platforms/Web/game/coi-serviceworker.js ${WEBROGUE_ROOT_PATH}/artifacts/coi-serviceworker.js
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/webrogue_game.js ${WEBROGUE_ROOT_PATH}/artifacts/webrogue_game.js
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/webrogue_game.wasm ${WEBROGUE_ROOT_PATH}/artifacts/webrogue_game.wasm
    COMMAND ${CMAKE_COMMAND} -E make_directory ${WEBROGUE_ROOT_PATH}/artifacts/mods
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/mods/core/core.wrmod ${WEBROGUE_ROOT_PATH}/artifacts/mods/core.wrmod
    COMMAND ${CMAKE_COMMAND} -E copy ${WEBROGUE_ROOT_PATH}/mods/basecontent/basecontent.wrmod ${WEBROGUE_ROOT_PATH}/artifacts/mods/basecontent.wrmod
    DEPENDS webrogue_game ${WEBROGUE_ROOT_PATH}/mods/core/core.wrmod ${WEBROGUE_ROOT_PATH}/mods/basecontent/basecontent.wrmod
)
