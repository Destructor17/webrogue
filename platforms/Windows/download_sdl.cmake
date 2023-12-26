file(
    DOWNLOAD "https://www.libsdl.org/release/SDL2-devel-2.24.2-VC.zip" ${CMAKE_BINARY_DIR}/SDL2-VC.zip
    EXPECTED_MD5 "07142d27d9b355641fc316416ba32e1d"
    SHOW_PROGRESS
)

execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${CMAKE_BINARY_DIR}/SDL2-VC.zip
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE rv)
if(NOT rv EQUAL 0)
    message(FATAL_ERROR "error: extraction of '${_filename}' failed")
endif()

file(
    DOWNLOAD "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.20.1-VC.zip" ${CMAKE_BINARY_DIR}/SDL2_ttf-VC.zip
    EXPECTED_MD5 "0e93c9052d01eebb9c18b1e641322637"
    SHOW_PROGRESS
)

execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${CMAKE_BINARY_DIR}/SDL2_ttf-VC.zip
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE rv)
if(NOT rv EQUAL 0)
    message(FATAL_ERROR "error: extraction of '${_filename}' failed")
endif()

file(GLOB SDL2_INCLUDE_DIR ${CMAKE_BINARY_DIR}/SDL2-*/include/)
file(GLOB SDL2_TTF_INCLUDE_DIR ${CMAKE_BINARY_DIR}/SDL2_ttf-*/include)
file(GLOB SDL2_LIBRARY ${CMAKE_BINARY_DIR}/SDL2-*/lib/x64/SDL2.lib)
file(GLOB SDL2_TTF_LIBRARY ${CMAKE_BINARY_DIR}/SDL2_ttf-*/lib/x64/SDL2_ttf.lib)

file(GLOB SDL2_DLLS ${CMAKE_BINARY_DIR}/SDL2-*/lib/x64/SDL2.dll)
file(GLOB SDL2_TTF_DLLS ${CMAKE_BINARY_DIR}/SDL2_ttf-*/lib/x64/SDL2_ttf.dll)
