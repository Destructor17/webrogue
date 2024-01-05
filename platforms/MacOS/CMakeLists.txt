cmake_minimum_required(VERSION 3.13)

project(webrogue_macos)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS "$(SRCROOT)")

set(CLANG_HAS_WASM FALSE)
set(WEBROGUE_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
set(WEBROGUE_MOD_NAMES core basecontent)
include(../../make_webrogue.cmake)

make_webrogue_output(
    LIB_NAME webrogue_output_sdl
    TYPE SDL
    STATIC
)
target_include_directories(
    webrogue_output_sdl PUBLIC 
    ${WEBROGUE_ROOT_PATH}/external/SDL/include 
    ${WEBROGUE_ROOT_PATH}/external/SDL_ttf
)
target_link_libraries(
    webrogue_output_sdl 
    ${WEBROGUE_ROOT_PATH}/platforms/MacOS/SDL2.framework/Versions/A/SDL2 
    ${WEBROGUE_ROOT_PATH}/platforms/MacOS/SDL2_ttf.framework/Versions/A/SDL2_ttf
)
make_webrogue_core(
    LIB_NAME webrogue_core
    STATIC
)

set(WEBROGUE_COMPACT_LINKER_SHARED OFF)
include(${WEBROGUE_ROOT_PATH}/cmake/make_compact_linker.cmake)
target_link_libraries(webrogue_core compact_linker)

#libuv
set(LIBUV_BUILD_SHARED OFF)
add_subdirectory(${WEBROGUE_ROOT_PATH}/external/libuv libuv)
target_link_libraries(webrogue_core uv)
target_include_directories(webrogue_core PRIVATE ${WEBROGUE_ROOT_PATH}/external/libuv/include) 

add_subdirectory(${WEBROGUE_ROOT_PATH}/platforms/MacOS/wamr wamr)
make_webrogue_runtime(
    LIB_NAME webrogue_runtime_wamr
    TYPE WAMR
    STATIC
)
add_library(core_wrmod STATIC
    ${WEBROGUE_ROOT_PATH}/embedded_resources/core_wrmod.c ${WEBROGUE_ROOT_PATH}/embedded_resources/core_wrmod.h
)
add_executable(webrogue MACOSX_BUNDLE ${WEBROGUE_ROOT_PATH}/platforms/MacOS/Webrogue/main.mm)

target_link_libraries(
    webrogue
    webrogue_core
    webrogue_runtime_wamr
    vmlib
    webrogue_output_sdl
    core_wrmod 
)

add_custom_target("cmake_configure" COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/configure.sh)

include("${WEBROGUE_ROOT_PATH}/cmake/xcode_source_grouping.cmake")

target_source_group(TARGET webrogue ROOT_DIR "${WEBROGUE_ROOT_PATH}/src")

set_target_properties(webrogue PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_GUI_IDENTIFIER com.webrogue
    MACOSX_BUNDLE_BUNDLE_NAME webrogue
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
    # MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/customtemplate.plist.in
)

target_sources(webrogue PUBLIC ${WEBROGUE_ROOT_PATH}/mods/basecontent/basecontent.wrmod)
set_source_files_properties(${WEBROGUE_ROOT_PATH}/mods/basecontent/basecontent.wrmod PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_custom_command(TARGET webrogue POST_BUILD
    COMMENT "Perforce Checkout App"
    COMMAND mkdir -p $TARGET_BUILD_DIR/$TARGET_NAME.app/Contents/Frameworks
    COMMAND cp -rf $SRCROOT/SDL2.framework $TARGET_BUILD_DIR/$TARGET_NAME.app/Contents/Frameworks
    COMMAND cp -rf $SRCROOT/SDL2_ttf.framework $TARGET_BUILD_DIR/$TARGET_NAME.app/Contents/Frameworks
    COMMAND install_name_tool -change @rpath/SDL2_ttf.framework/Versions/A/SDL2_ttf @executable_path/../Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf $CONFIGURATION_BUILD_DIR/$EXECUTABLE_PATH 
    COMMAND install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2 @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 $CONFIGURATION_BUILD_DIR/$EXECUTABLE_PATH 
)
