cmake_minimum_required(VERSION 3.13)

project(webrogue)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR})
set(CLANG_HAS_WASM TRUE)
# if(WIN32)
#     set(CLANG_HAS_WASM FALSE)
# endif()
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/mods/)
set(WEBROGUE_MOD_NAMES core basecontent)
#
include(make_webrogue.cmake)

list(APPEND CMAKE_MODULE_PATH ${WEBROGUE_ROOT_PATH}/cmake/sdl2)
set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR})

# add_subdirectory(external/llvm/llvm)

if(NOT WIN32)
    set(WASMEDGE_BUILD_AOT_RUNTIME ON)
    set(WASMEDGE_BUILD_SHARED_LIB OFF)
    set(WASMEDGE_BUILD_STATIC_LIB ON)
    enable_language(ASM)
    enable_language(ASM-ATT)
    add_subdirectory(external/wasmedge wasmedge)
endif()

# set(WAMR_BUILD_DEBUG_INTERP 1)
set(WEBROGUE_LIBRARY_TYPE SHARED)
if(APPLE)
    add_subdirectory(${WEBROGUE_ROOT_PATH}/platforms/MacOS/wamr wamr)

    set(WEBROGUE_LIBRARY_TYPE STATIC) # would be nice to fix
elseif(WIN32)
    message(WARNING "No WAMR for Windows(")
    if(${CMAKE_C_COMPILER_FRONTEND_VARIANT} STREQUAL "GNU")
        
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:8388608")
    endif()
    
    set(WEBROGUE_LIBRARY_TYPE STATIC) # would be nice to fix
else()
    add_subdirectory(${WEBROGUE_ROOT_PATH}/platforms/Linux/wamr wamr)
endif()
set(WAMR_SUBDIR_ADDED 1)

if(${WEBROGUE_LIBRARY_TYPE} STREQUAL STATIC)
    set(WEBROGUE_COMPACT_LINKER_STATIC ON)
elseif(${WEBROGUE_LIBRARY_TYPE} STREQUAL SHARED)
    set(WEBROGUE_COMPACT_LINKER_SHARED ON)
else()
    message(FATAL_ERROR)
endif()    
make_webrogue_core(
    LIB_NAME webrogue_core
    ${WEBROGUE_LIBRARY_TYPE}
)

#libuv
if(${WEBROGUE_LIBRARY_TYPE} STREQUAL STATIC)
    set(LIBUV_BUILD_SHARED OFF)
elseif(${WEBROGUE_LIBRARY_TYPE} STREQUAL SHARED)
    set(LIBUV_BUILD_SHARED ON)
else()
    message(FATAL_ERROR)
endif()    
add_subdirectory(${WEBROGUE_ROOT_PATH}/external/libuv libuv)

target_link_libraries(webrogue_core uv)
target_include_directories(webrogue_core PRIVATE ${WEBROGUE_ROOT_PATH}/external/libuv/include) 

add_library(core_wrmod ${WEBROGUE_LIBRARY_TYPE} 
    ${WEBROGUE_ROOT_PATH}/embedded_resources/core_wrmod.c ${WEBROGUE_ROOT_PATH}/embedded_resources/core_wrmod.h
)

set(WEBROGUE_RUNTIMES)

make_webrogue_runtime(
    LIB_NAME webrogue_runtime_embedded
    TYPE EMBEDDED
    ${WEBROGUE_LIBRARY_TYPE}
    EMBEDDED_RUNTIME_MODS ${WEBROGUE_MOD_NAMES}
)
list(APPEND WEBROGUE_RUNTIMES embedded)

make_webrogue_runtime(
    LIB_NAME webrogue_runtime_m3
    TYPE WASM3
    ${WEBROGUE_LIBRARY_TYPE}
)
list(APPEND WEBROGUE_RUNTIMES m3)

include(${WEBROGUE_ROOT_PATH}/cmake/make_compact_linker.cmake)

if(NOT WIN32)
    make_webrogue_runtime(
        LIB_NAME webrogue_runtime_wasmedge
        TYPE WASMEDGE
        ${WEBROGUE_LIBRARY_TYPE}
    )
    target_link_libraries(webrogue_runtime_wasmedge wasmedgeCAPI)
    
    list(APPEND WEBROGUE_RUNTIMES wasmedge)
endif()

if(NOT WIN32) # MSVC can not handle insanily long sqlpp names
    make_webrogue_runtime(
        LIB_NAME webrogue_runtime_wasm2c
        TYPE WASM2C
        ${WEBROGUE_LIBRARY_TYPE}
    )
    list(APPEND WEBROGUE_RUNTIMES wasm2c)
endif()

if(NOT WIN32)
    make_webrogue_runtime(
        LIB_NAME webrogue_runtime_wamr
        TYPE WAMR
        ${WEBROGUE_LIBRARY_TYPE}
    )
    target_link_libraries(webrogue_runtime_wamr vmlib webrogue_core)
    list(APPEND WEBROGUE_RUNTIMES wamr)
endif()

make_webrogue_runtime(
    LIB_NAME webrogue_runtime_web
    TYPE WEB
    ${WEBROGUE_LIBRARY_TYPE}
)
list(APPEND WEBROGUE_RUNTIMES web)

set(WEBROGUE_OUTPUTS)

if(WIN32)
    include(${WEBROGUE_ROOT_PATH}/platforms/Windows/download_sdl.cmake)
    
    include_directories(${WEBROGUE_ROOT_PATH}/external/dirent/include)
    
    make_webrogue_output(
        LIB_NAME webrogue_output_pdcurses
        TYPE PDCURSES
        ${WEBROGUE_LIBRARY_TYPE}
        PDCURSES_OS wincon
    )
    target_include_directories(webrogue_output_pdcurses PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(webrogue_output_pdcurses ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    list(APPEND WEBROGUE_OUTPUTS pdcurses)

    make_webrogue_output(
        LIB_NAME webrogue_output_sdl
        TYPE SDL
        ${WEBROGUE_LIBRARY_TYPE}
    )
    target_include_directories(webrogue_output_sdl PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(webrogue_output_sdl ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    list(APPEND WEBROGUE_OUTPUTS sdl)
else()
    find_package(SDL2)
    find_package(SDL2_ttf)
    if(SDL2_FOUND)
        make_webrogue_output(
            LIB_NAME webrogue_output_pdcurses
            TYPE PDCURSES
            ${WEBROGUE_LIBRARY_TYPE}
            PDCURSES_OS sdl2
        )
        target_include_directories(webrogue_output_pdcurses PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
        target_link_libraries(webrogue_output_pdcurses ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
        list(APPEND WEBROGUE_OUTPUTS pdcurses)

        make_webrogue_output(
            LIB_NAME webrogue_output_sdl
            TYPE SDL
            ${WEBROGUE_LIBRARY_TYPE}
        )
        target_include_directories(webrogue_output_sdl PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
        target_link_libraries(webrogue_output_sdl ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
        list(APPEND WEBROGUE_OUTPUTS sdl)
    endif()

    make_webrogue_output(
        LIB_NAME webrogue_output_ncurses
        TYPE NCURSES
        ${WEBROGUE_LIBRARY_TYPE}
    )
    target_link_libraries(webrogue_output_ncurses ncurses)
    list(APPEND WEBROGUE_OUTPUTS ncurses)
endif()

add_custom_target(all_webrogue)

foreach(RUNTIME ${WEBROGUE_RUNTIMES})
    foreach(OUTPUT ${WEBROGUE_OUTPUTS})
        set(EXECUTABLE_NAME webrogue_${OUTPUT}_${RUNTIME})
        if(WIN32)
            set(EXECUTABLE_TYPE WIN32)
        else()
            set(EXECUTABLE_TYPE)
        endif()
        add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_TYPE} ${WEBROGUE_ROOT_PATH}/main.cpp)
        target_link_libraries(
            ${EXECUTABLE_NAME}
            webrogue_runtime_${RUNTIME}
            webrogue_output_${OUTPUT}
            core_wrmod
            webrogue_core
            compact_linker
        )
        target_include_directories(${EXECUTABLE_NAME} PRIVATE ${WEBROGUE_ROOT_PATH}/src)
        target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)
        add_dependencies(all_webrogue ${EXECUTABLE_NAME})
        if(OUTPUT STREQUAL sdl)
            target_compile_definitions(${EXECUTABLE_NAME} PRIVATE OUTPUT_SDL)
            target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
        endif()
        if(RUNTIME STREQUAL embedded)
            target_include_directories(${EXECUTABLE_NAME} PRIVATE ${WEBROGUE_ROOT_PATH}/embedded_resources)
        endif()
    endforeach()
endforeach()

# compact_linker_test
add_executable(
    compact_linker_test
    ${WEBROGUE_ROOT_PATH}/src/linker/wasm-ld-mini.cc
)

target_link_libraries(compact_linker_test compact_linker)

target_compile_definitions(
    compact_linker_test PRIVATE WASM_OBJDUMP_PATH=$<TARGET_FILE:wasm-objdump>
)

if(NOT EXISTS ${WEBROGUE_ROOT_PATH}/src/linker/test_src/generated_toolchain.cmake)
    exec_program(${CMAKE_COMMAND} ARGS -P${WEBROGUE_ROOT_PATH}/src/linker/test_src/download_toolchain.cmake)
endif()
add_custom_target(conf_wasm_files_for_compact_ported_linker_test
    COMMAND ${CMAKE_COMMAND} 
      -B ${CMAKE_CURRENT_BINARY_DIR}/test_build -S ${WEBROGUE_ROOT_PATH}/src/linker/test_src
      --toolchain=${WEBROGUE_ROOT_PATH}/src/linker/test_src/generated_toolchain.cmake
      -DWASM_LD_MINI_WASM_OBJDUMP_PATH=$<TARGET_FILE:wasm-objdump>
      -DWASM_LD_MINI_WASM_STRIP_PATH=$<TARGET_FILE:wasm-strip>
      ${MODS_BUILD_GENERATOR_ARGS}
      -DCMAKE_BUILD_TYPE=Release
    WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH}/src/linker/test_src
    DEPENDS wasm-objdump wasm-strip
)
add_custom_target(build_wasm_files_for_compact_ported_linker_test
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/test_build
    WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH}/src/linker/test_src
    DEPENDS conf_wasm_files_for_compact_ported_linker_test
)
add_dependencies(compact_linker_test build_wasm_files_for_compact_ported_linker_test)


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
