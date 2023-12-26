include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/parse_api_header.cmake)

parse_api_header(FUNC_COUNT num_of_funcs)

set(out_content "#include \"shared_api_object.hpp\"\n\n// clang-format off\n")

foreach(func_i RANGE 1 ${num_of_funcs})
    parse_api_header(
        FUNC_ID ${func_i} 
        FUNC_NAME func_name
        RET_TYPE ret_type
        ARGS args
        ARGS_WITHOUT_TYPES args_without_types
    )
    string(APPEND out_content "extern \"C\" ${ret_type} ${func_name}(${args}) {\n") 
    string(APPEND out_content "    return webrogue::runtimes::embedded::sharedApiObject->${func_name}(${args_without_types});\n")
    string(APPEND out_content "}\n")
    string(APPEND out_content "\n")
endforeach()

file(WRITE ${NR_API_EMBEDDING}.cpp "${out_content}")

set(out_content "#pragma once\n\n#include <cstdint>\n\n// clang-format off\n")

foreach(func_i RANGE 1 ${num_of_funcs})
    parse_api_header(
        FUNC_ID ${func_i} 
        FUNC_NAME func_name
        RET_TYPE ret_type
        ARGS args
        ARGS_WITHOUT_TYPES args_without_types
    )
    string(APPEND out_content "extern \"C\" ${ret_type} ${func_name}(${args});\n") 
    string(APPEND out_content "\n")
endforeach()

file(WRITE ${NR_API_EMBEDDING}.hpp "${out_content}")
