if(NOT DEFINED CLANG_HAS_WASM)
    set(CLANG_HAS_WASM FALSE)
endif()

if(CLANG_HAS_WASM)
    if(NOT EXISTS ${WEBROGUE_ROOT_PATH}/mods/tools/generated_toolchain.cmake)
        exec_program(${CMAKE_COMMAND} ARGS -P ${WEBROGUE_ROOT_PATH}/mods/tools/download_toolchain.cmake)
    endif()
    if(NOT EXISTS ${WEBROGUE_ROOT_PATH}/mods/tools/wasi-sysroot)
        exec_program(${CMAKE_COMMAND} ARGS -P ${WEBROGUE_ROOT_PATH}/mods/tools/download_sysroot.cmake)
    endif()
endif()

if(WEBROGUE_MOD_NAMES)
    set(mod_names ${WEBROGUE_MOD_NAMES})
else()
    message(FATAL_ERROR "WEBROGUE_MOD_NAMES not defined")
endif()

#sources
set(
    WEBROGUE_CORE_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/core/ApiObject.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/ApiObject.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/Buffer2d.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/CompactLinking.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/CompactLinking.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/Config.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/Config.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/ConsoleStream.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/ConsoleStream.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/ConsoleWriter.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/ConsoleWriter.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/DB.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/DB.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/load_embedded_mods.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/ResourceStorage.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/ResourceStorage.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/ModsRuntime.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/ModsRuntime.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/webrogueMain.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/webrogueMain.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/Output.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/Output.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/utf.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/utf.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/Vec2.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/Vec2.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/VFS.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/VFS.cpp

    ${WEBROGUE_ROOT_PATH}/external/sqlite_amb/sqlite3.c
    ${WEBROGUE_ROOT_PATH}/external/sqlite_amb/sqlite3.h

    ${WEBROGUE_ROOT_PATH}/external/xz/linux/lib/xz/xz_dec_lzma2.c
    ${WEBROGUE_ROOT_PATH}/external/xz/linux/lib/xz/xz_dec_stream.c
    ${WEBROGUE_ROOT_PATH}/external/xz/linux/lib/xz/xz_crc32.c
    

    ${WEBROGUE_ROOT_PATH}/src/core/wasi_templates.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/wasi_types.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/WASIObject.hpp
    ${WEBROGUE_ROOT_PATH}/src/core/WASIObject.cpp
    ${WEBROGUE_ROOT_PATH}/src/core/WASIObjectFS.cpp
)


set(
    WEBROGUE_SDL_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/outputs/sdl/SDLOutput.hpp
    ${WEBROGUE_ROOT_PATH}/src/outputs/sdl/SDLOutput.cpp
    ${WEBROGUE_ROOT_PATH}/embedded_resources/sdl_font_ttf.c
    ${WEBROGUE_ROOT_PATH}/embedded_resources/sdl_font_ttf.h
)

set(
    WEBROGUE_CURSES_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/outputs/curses/CursesOutput.hpp
    ${WEBROGUE_ROOT_PATH}/src/outputs/curses/CursesOutput.cpp
)

set(
    WEBROGUE_EMBEDDED_RUNTIME_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/embedded_runtime.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/embedded_runtime.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/wr_api_embedding_glue.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/wr_api_embedding_glue.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/shared_api_object.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/shared_api_object.cpp
)

set(
    WEBROGUE_M3_RUNTIME_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/runtimes/m3/m3_runtime.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/m3/m3_runtime.cpp

    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_api_libc.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_api_wasi.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_api_uvwasi.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_api_meta_wasi.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_api_tracer.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_bind.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_code.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_compile.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_core.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_env.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_exec.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_function.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_info.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_module.c
    ${WEBROGUE_ROOT_PATH}/external/wasm3/source/m3_parse.c
)

set(
    WEBROGUE_WAMR_RUNTIME_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/runtimes/wamr/wamr_runtime.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wamr/wamr_runtime.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wamr/wamr_templates.hpp
)

set(
    WEBROGUE_WASM2C_RUNTIME_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/wr_api_wasm2c_glue.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/wr_api_wasm2c_wasi.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/wasm2c_runtime.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/wasm2c_runtime.cpp

    ${WEBROGUE_ROOT_PATH}/external/wabt/wasm2c/wasm-rt-impl.h
    ${WEBROGUE_ROOT_PATH}/external/wabt/wasm2c/wasm-rt-impl.c
    ${WEBROGUE_ROOT_PATH}/external/wabt/wasm2c/wasm-rt-exceptions-impl.c
)

set(
    WEBROGUE_WASMEDGE_RUNTIME_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasmedge/wasmedge_runtime.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasmedge/wasmedge_runtime.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/wasmedge/wasmedge_templates.hpp
)

set(
    WEBROGUE_WEB_RUNTIME_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/src/runtimes/web/imported_func_wrapper.cpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/web/imported_func_wrapper.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/web/web_runtime.hpp
    ${WEBROGUE_ROOT_PATH}/src/runtimes/web/web_runtime.cpp
)

set(
    UVWASI_SOURCE_FILES

    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/clocks.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/fd_table.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/path_resolver.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/poll_oneoff.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/sync_helpers.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/uv_mapping.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/uvwasi.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/wasi_rights.c
    ${WEBROGUE_ROOT_PATH}/external/uvwasi/src/wasi_serdes.c
)

if(WIN32)
    source_group(TREE ${WEBROGUE_ROOT_PATH}/src)
endif()
# resources
function(embed_resource resource)
    string(REGEX MATCH "([^/]+)$" filename ${resource})
    string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
    set(copied_resource ${WEBROGUE_ROOT_PATH}/embedded_resources/${filename})
    add_custom_command(
        OUTPUT ${copied_resource}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${resource} ${copied_resource}
        DEPENDS ${resource}
    )
    add_custom_command(
        OUTPUT ${WEBROGUE_ROOT_PATH}/embedded_resources/${filename}.h ${WEBROGUE_ROOT_PATH}/embedded_resources/${filename}.c
        COMMAND ${CMAKE_COMMAND} "-DINPUT_FILE=${copied_resource}" -P ${WEBROGUE_ROOT_PATH}/cmake/embed_resource.cmake
        WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH}
        DEPENDS ${copied_resource}
    )
endfunction()

embed_resource(${WEBROGUE_ROOT_PATH}/src/outputs/sdl/sdl_font.ttf)

#glue
add_custom_command(
    OUTPUT ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/wr_api_embedding_glue.hpp ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/wr_api_embedding_glue.cpp
    COMMAND ${CMAKE_COMMAND} "-DWR_API_HEADER=mods/core/include/common/wr_api_functions.def" "-DWR_API_EMBEDDING=src/runtimes/embedded/wr_api_embedding_glue" -P src/runtimes/embedded/gen_glue_embedded.cmake
    WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH}
    DEPENDS ${WEBROGUE_ROOT_PATH}/mods/core/include/common/wr_api_functions.def ${WEBROGUE_ROOT_PATH}/src/runtimes/embedded/gen_glue_embedded.cmake
)
add_custom_command(
    OUTPUT ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/wr_api_wasm2c_glue.cpp
    COMMAND ${CMAKE_COMMAND} "-DWR_API_HEADER=mods/core/include/common/wr_api_functions.def" "-DWR_API_EMBEDDING=src/runtimes/wasm2c/wr_api_wasm2c_glue" -P src/runtimes/wasm2c/gen_glue_wasm2c.cmake
    WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH}
    DEPENDS ${WEBROGUE_ROOT_PATH}/mods/core/include/common/wr_api_functions.def ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/gen_glue_wasm2c.cmake
)
add_custom_command(
    OUTPUT ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/wr_api_wasm2c_wasi.cpp
    COMMAND ${CMAKE_COMMAND} "-DWR_API_HEADER=src/core/wasi_functions.def" "-DWR_API_EMBEDDING=src/runtimes/wasm2c/wr_api_wasm2c_wasi" -P src/runtimes/wasm2c/gen_wasi_wasm2c.cmake
    WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH}
    DEPENDS ${WEBROGUE_ROOT_PATH}/src/core/wasi_functions.def ${WEBROGUE_ROOT_PATH}/src/runtimes/wasm2c/gen_wasi_wasm2c.cmake
)

#headers
macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${curdir}/${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()
subdirlist(WEBROGUE_HEADER_SEARCH_PATHS ${WEBROGUE_ROOT_PATH}/src)

#wasm2c
set(WABT_SUBDIR_ADDED 0)
macro(add_wabt_subdir)
    if(${WABT_SUBDIR_ADDED} EQUAL 0)
        set(BUILD_TESTS FALSE)
        add_subdirectory(${WEBROGUE_ROOT_PATH}/external/wabt)
        set(WABT_SUBDIR_ADDED 1)
    endif()
endmacro()

function(make_pdcurses)
    set(options)
    set(oneValueArgs TARGET_NAME OS)
    set(multiValueArgs INCLUDE_PATHS LIBS)
    cmake_parse_arguments(MAKE_PDCURSES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(PDCURSES_DIR ${WEBROGUE_ROOT_PATH}/external/pdcurses)
    file(GLOB_RECURSE PDCURSES_SRC ${PDCURSES_DIR}/pdcurses/*.c)
    file(GLOB_RECURSE PDCURSES_OS_SRC ${PDCURSES_DIR}/${MAKE_PDCURSES_OS}/*.c)
    add_library(${MAKE_PDCURSES_TARGET_NAME} STATIC ${PDCURSES_DIR}/${MAKE_PDCURSES_OS}/pdcclip.c ${PDCURSES_SRC} ${PDCURSES_OS_SRC})
    target_include_directories(${MAKE_PDCURSES_TARGET_NAME} PRIVATE ${PDCURSES_DIR}/${MAKE_PDCURSES_OS} ${PDCURSES_DIR} ${MAKE_PDCURSES_INCLUDE_PATHS})
    target_link_libraries(${MAKE_PDCURSES_TARGET_NAME} PRIVATE ${MAKE_PDCURSES_LIBS})
    target_compile_definitions(${MAKE_PDCURSES_TARGET_NAME} PUBLIC PDC_RGB)
endfunction()

set(EMBEDDED_MOD_SUBDIRS)

set(PACK_MODS_COMMANDS)
set(PACK_MODS_DEPPENDS)
set(PACK_MODS_OUTPUTS)
set(COPY_MOD_DIRS_COMMANDS)

foreach(mod_name ${mod_names})
    list(APPEND PACK_MODS_COMMANDS COMMAND ${CMAKE_COMMAND} -E copy_if_different ${WEBROGUE_ROOT_PATH}/mods/${mod_name}/${mod_name}.wrmod ${CMAKE_CURRENT_BINARY_DIR}/mods/${mod_name}.wrmod)
    list(APPEND PACK_MODS_DEPPENDS ${WEBROGUE_ROOT_PATH}/mods/${mod_name}/${mod_name}.wrmod)
    list(APPEND PACK_MODS_OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/mods/${mod_name}.wrmod)
endforeach()

add_custom_target(pack_mods_to_build_dir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/mods
    ${PACK_MODS_COMMANDS}
    BYPRODUCTS ${PACK_MODS_OUTPUTS}
)

if(WIN32)
    set(MODS_BUILD_GENERATOR_ARGS -G "NMake Makefiles")
else()
    set(MODS_BUILD_GENERATOR_ARGS)
endif()
if(CLANG_HAS_WASM)
    string(JOIN "\\;" WEBROGUE_MOD_NAME_CONFIGURE_ARG ${WEBROGUE_MOD_NAMES})
    execute_process(
        COMMAND ${CMAKE_COMMAND} 
            -B ${CMAKE_CURRENT_BINARY_DIR}/mods_build -S ${WEBROGUE_ROOT_PATH}/mods
            --toolchain=tools/generated_toolchain.cmake
            ${MODS_BUILD_GENERATOR_ARGS}
            -DCMAKE_MODULE_PATH=${WEBROGUE_ROOT_PATH}/cmake 
            -DCMAKE_BUILD_TYPE=Debug
            -DCMAKE_LINKER=a
            -DWEBROGUE_MOD_NAMES=${WEBROGUE_MOD_NAME_CONFIGURE_ARG}
        RESULT_VARIABLE r
    )
    if(NOT r EQUAL 0)
        message(FATAL_ERROR "Error while configuring wasm mods")
    endif()
    set(BUILD_MODS_OUTPUTS)
    foreach(mod_name ${mod_names})
        list(APPEND BUILD_MODS_OUTPUTS ${WEBROGUE_ROOT_PATH}/mods/${mod_name}/${mod_name}.wrmod ${WEBROGUE_ROOT_PATH}/mods/${mod_name}/${mod_name}.ewrmod)
    endforeach()
    add_custom_target(build_wasm_mods
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/mods_build --target final_linking
        WORKING_DIRECTORY "${WEBROGUE_ROOT_PATH}"
        BYPRODUCTS ${BUILD_MODS_OUTPUTS} ${CMAKE_CURRENT_BINARY_DIR}/mods_build/linked.wasm
    )
    add_dependencies(pack_mods_to_build_dir build_wasm_mods)
endif()

embed_resource(${WEBROGUE_ROOT_PATH}/mods/core/core.wrmod)

function(make_webrogue_output)
    set(options STATIC SHARED)
    set(oneValueArgs TYPE LIB_NAME PDCURSES_OS)
    set(multiValueArgs)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(${ARGS_TYPE} STREQUAL SDL)
        set(SOURCES ${WEBROGUE_SDL_SOURCE_FILES})
    elseif(${ARGS_TYPE} STREQUAL NCURSES)
        set(SOURCES ${WEBROGUE_CURSES_SOURCE_FILES})
    elseif(${ARGS_TYPE} STREQUAL PDCURSES)
        set(PDCURSES_DIR ${WEBROGUE_ROOT_PATH}/external/pdcurses)
        file(GLOB_RECURSE PDCURSES_SRC ${PDCURSES_DIR}/pdcurses/*.c)
        file(GLOB_RECURSE PDCURSES_OS_SRC ${PDCURSES_DIR}/${ARGS_PDCURSES_OS}/*.c)
        set(
            SOURCES 
            ${WEBROGUE_CURSES_SOURCE_FILES}
            ${PDCURSES_DIR}/${ARGS_PDCURSES_OS}/pdcclip.c ${PDCURSES_SRC} ${PDCURSES_OS_SRC}
        )
    else()
        message(FATAL_ERROR "Unknown output type: ${ARGS_TYPE}")
    endif()
    if(${ARGS_STATIC})
        set(LIB_TYPE STATIC)
    elseif(${ARGS_SHARED})
        set(LIB_TYPE SHARED)
    else()
        message(FATAL_ERROR "Specify STATIC or SHARED library")
    endif()
    add_library(${ARGS_LIB_NAME} ${LIB_TYPE} ${SOURCES})

    if(${ARGS_TYPE} STREQUAL NCURSES)
        target_compile_definitions(${ARGS_LIB_NAME} PRIVATE WEBROGUE_NCURSES)
    elseif(${ARGS_TYPE} STREQUAL PDCURSES)
        target_compile_definitions(${ARGS_LIB_NAME} PRIVATE WEBROGUE_PDCURSES)
        set(options)
        set(oneValueArgs TARGET_NAME )
        set(multiValueArgs INCLUDE_PATHS LIBS)
        cmake_parse_arguments(MAKE_PDCURSES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
        target_include_directories(
            ${ARGS_LIB_NAME} PRIVATE 
            ${PDCURSES_DIR}/${ARGS_PDCURSES_OS}
            ${PDCURSES_DIR}
        )
    endif()
endfunction()

function(make_webrogue_runtime)
    set(options STATIC SHARED)
    set(oneValueArgs TYPE LIB_NAME)
    set(multiValueArgs EMBEDDED_RUNTIME_MODS)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(${ARGS_TYPE} STREQUAL WASM3)
        set(SOURCES ${WEBROGUE_M3_RUNTIME_SOURCE_FILES})
        
        exec_program(git ${WEBROGUE_ROOT_PATH}/external/wasm3 ARGS apply ../wasm3.patch OUTPUT_VARIABLE v)
    elseif(${ARGS_TYPE} STREQUAL WASM2C)
        set(
            SOURCES 
            ${WEBROGUE_WASM2C_RUNTIME_SOURCE_FILES} 
            ${CMAKE_CURRENT_BINARY_DIR}/linked.h ${CMAKE_CURRENT_BINARY_DIR}/linked.c
        )
        add_wabt_subdir()
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/linked.h ${CMAKE_CURRENT_BINARY_DIR}/linked.c
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/mods_build/linked.wasm ${CMAKE_CURRENT_BINARY_DIR}/mods_build/linked_strip.wasm
            COMMAND $<TARGET_FILE:wasm-strip> ${CMAKE_CURRENT_BINARY_DIR}/mods_build/linked_strip.wasm
            COMMAND $<TARGET_FILE:wasm2c> ${CMAKE_CURRENT_BINARY_DIR}/mods_build/linked_strip.wasm -o ${CMAKE_CURRENT_BINARY_DIR}/linked.c --module-name=linked
            WORKING_DIRECTORY ${WEBROGUE_ROOT_PATH} 
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mods_build/linked.wasm wasm2c wasm-strip
        )
    elseif(${ARGS_TYPE} STREQUAL WAMR)
        set(SOURCES ${WEBROGUE_WAMR_RUNTIME_SOURCE_FILES})
    elseif(${ARGS_TYPE} STREQUAL EMBEDDED)
        set(SOURCES ${WEBROGUE_EMBEDDED_RUNTIME_SOURCE_FILES})
        foreach(mod_to_embed ${ARGS_EMBEDDED_RUNTIME_MODS})
            embed_resource(${WEBROGUE_ROOT_PATH}/mods/${mod_to_embed}/${mod_to_embed}.ewrmod)
            list(APPEND SOURCES ${WEBROGUE_ROOT_PATH}/embedded_resources/${mod_to_embed}_ewrmod.c ${WEBROGUE_ROOT_PATH}/embedded_resources/${mod_to_embed}_ewrmod.h)
        endforeach()
    elseif(${ARGS_TYPE} STREQUAL WASMEDGE)
        set(SOURCES ${WEBROGUE_WASMEDGE_RUNTIME_SOURCE_FILES})
        exec_program(git ${WEBROGUE_ROOT_PATH}/external/wasmedge ARGS apply ../wasmedge.patch OUTPUT_VARIABLE v)
    elseif(${ARGS_TYPE} STREQUAL WEB)
        set(SOURCES ${WEBROGUE_WEB_RUNTIME_SOURCE_FILES})
    else()
        message(FATAL_ERROR "Unknown runtime type: ${ARGS_TYPE}")
    endif()
    if(${ARGS_STATIC})
        set(LIB_TYPE STATIC)
    elseif(${ARGS_SHARED})
        set(LIB_TYPE SHARED)
    else()
        message(FATAL_ERROR "Specify STATIC or SHARED library")
    endif()
    add_library(${ARGS_LIB_NAME} ${LIB_TYPE} ${SOURCES})

    if(${ARGS_TYPE} STREQUAL WASM2C)
        target_include_directories(${ARGS_LIB_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${WEBROGUE_ROOT_PATH}/external/wabt/wasm2c)
        target_compile_definitions(${ARGS_LIB_NAME} PRIVATE LINKED_HEADER=${CMAKE_CURRENT_BINARY_DIR}/linked.h)
        set(WASM2C_MOD_LIST_HEADER_CONTENT "")
        foreach(mod_to_embed ${WEBROGUE_MOD_NAMES})
            string(APPEND WASM2C_MOD_LIST_HEADER_CONTENT "mod_to_embed(${mod_to_embed})\n")
        endforeach()
        set(WASM2C_MOD_LIST_HEADER ${CMAKE_CURRENT_BINARY_DIR}/wasm2c_mod_list_${ARGS_LIB_NAME}.h)
        file(WRITE ${WASM2C_MOD_LIST_HEADER} ${WASM2C_MOD_LIST_HEADER_CONTENT})
        target_compile_definitions(${ARGS_LIB_NAME} PUBLIC WASM2C_MOD_LIST_HEADER=${WASM2C_MOD_LIST_HEADER})
    elseif(ARGS_TYPE STREQUAL EMBEDDED)
        list(SORT ARGS_EMBEDDED_RUNTIME_MODS)
        set(MOD_LIST_HEADER_CONTENT "")
        set(MOD_ZIP_LIST_HEADER_CONTENT "")
        set(WEBROGUE_MOD_LIB_TYPE ${LIB_TYPE})
        foreach(mod_to_embed ${ARGS_EMBEDDED_RUNTIME_MODS})
            string(APPEND MOD_LIST_HEADER_CONTENT "mod_to_embed(${mod_to_embed})\n")
            string(APPEND MOD_ZIP_LIST_HEADER_CONTENT "#include \"${mod_to_embed}_ewrmod.h\"\n")
            function(register_mod)
                set(options)
                set(oneValueArgs NAME)
                set(multiValueArgs)
                cmake_parse_arguments(ADD_MOD "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
                if(NOT ${mod_to_embed} STREQUAL ${ADD_MOD_NAME})
                    message(FATAL_ERROR "Mod name must match directory name, got ${NAME} and ${mod_to_embed}")
                endif()
            endfunction()
            if(NOT ${mod_to_embed} IN_LIST EMBEDDED_MOD_SUBDIRS)
                add_subdirectory(${WEBROGUE_ROOT_PATH}/mods/${mod_to_embed} ${mod_to_embed}_embedded)
                set(WEBROGUE_MOD_DEFS WEBROGUE_EMBEDDED_MODS)
                target_compile_definitions(${mod_to_embed} PUBLIC ${WEBROGUE_MOD_DEFS})
                set(EMBEDDED_MOD_SUBDIRS ${EMBEDDED_MOD_SUBDIRS} ${mod_to_embed})
                set(EMBEDDED_MOD_SUBDIRS ${EMBEDDED_MOD_SUBDIRS} PARENT_SCOPE)
            endif()
        endforeach()
        target_link_libraries(${ARGS_LIB_NAME} ${ARGS_EMBEDDED_RUNTIME_MODS})

        set(MOD_LIST_HEADER ${CMAKE_CURRENT_BINARY_DIR}/mod_list_${ARGS_LIB_NAME}.h)
        file(WRITE ${MOD_LIST_HEADER} ${MOD_LIST_HEADER_CONTENT})
        target_compile_definitions(${ARGS_LIB_NAME} PUBLIC MOD_LIST_HEADER=${MOD_LIST_HEADER})

        set(MOD_ZIP_LIST_HEADER ${CMAKE_CURRENT_BINARY_DIR}/mod_zip_list_${ARGS_LIB_NAME}.h)
        file(WRITE ${MOD_ZIP_LIST_HEADER} ${MOD_ZIP_LIST_HEADER_CONTENT})
        target_compile_definitions(${ARGS_LIB_NAME} PUBLIC MOD_ZIP_LIST_HEADER=${MOD_ZIP_LIST_HEADER})
        
        target_compile_definitions(${ARGS_LIB_NAME} PUBLIC WEBROGUE_EMBEDDED_MODS)
    endif()
endfunction()

function(make_webrogue_core)
    set(options STATIC SHARED NO_WASM)
    set(oneValueArgs LIB_NAME)
    set(multiValueArgs)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(${ARGS_STATIC})
        set(LIB_TYPE STATIC)
    elseif(${ARGS_SHARED})
        set(LIB_TYPE SHARED)
    else()
        message(FATAL_ERROR "Specify STATIC or SHARED library")
    endif()
    
    set(
        SOURCES 
        ${WEBROGUE_CORE_SOURCE_FILES}
        ${UVWASI_SOURCE_FILES}
    )
    add_library(${ARGS_LIB_NAME} ${LIB_TYPE} ${SOURCES})
    add_dependencies(${ARGS_LIB_NAME} pack_mods_to_build_dir)
    target_include_directories(
        ${ARGS_LIB_NAME} PRIVATE 
        ${WEBROGUE_ROOT_PATH}/external/xz/userspace 
        ${WEBROGUE_ROOT_PATH}/external/xz/linux/include/linux
        ${WEBROGUE_ROOT_PATH}/external/uvwasi/include
    )
endfunction()

