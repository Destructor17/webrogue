include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/parse_api_header.cmake)

parse_api_header(FUNC_COUNT num_of_funcs)

set(out_content "#include \"../../core/wasm_types.hpp\"\n#include \"shared_api_object.hpp\"\nusing namespace webrogue::core;\n// clang-format off\n")

foreach(func_i RANGE 1 ${num_of_funcs})
    parse_api_header(
        FUNC_MACRO WR_API_FUNCTION
        FUNC_ID ${func_i} 
        FUNC_NAME func_name
        RET_TYPE ret_type
        ARGS args
        ARGS_WITHOUT_TYPES args_without_types
    )
    string(REPLACE "WASMRawU32" "uint32_t" ret_type "${ret_type}")
    string(REPLACE "WASMRawI32" "int32_t" ret_type "${ret_type}")
    string(REPLACE "WASMRawF32" "float" ret_type "${ret_type}")
    string(REPLACE "WASMRawU64" "uint64_t" ret_type "${ret_type}")
    string(REPLACE "WASMRawI64" "int64_t" ret_type "${ret_type}")
    string(REPLACE "WASMRawF64" "double" ret_type "${ret_type}")

    string(REPLACE "WASMRawU32" "uint32_t" args "${args}")
    string(REPLACE "WASMRawI32" "int32_t" args "${args}")
    string(REPLACE "WASMRawF32" "float" args "${args}")
    string(REPLACE "WASMRawU64" "uint64_t" args "${args}")
    string(REPLACE "WASMRawI64" "int64_t" args "${args}")
    string(REPLACE "WASMRawF64" "double" args "${args}")

    set(nargs "${args}")
    if(nargs)
        string(REPLACE "uint32_t " "WASMRawU32::make(" nargs "${nargs}")
        string(REPLACE "int32_t " "WASMRawI32::make(" nargs "${nargs}")
        string(REPLACE "float " "WASMRawF32::make(" nargs "${nargs}")
        string(REPLACE "uint64_t " "WASMRawU64::make(" nargs "${nargs}")
        string(REPLACE "int64_t " "WASMRawI64::make(" nargs "${nargs}")
        string(REPLACE "double " "WASMRawF64::make(" nargs "${nargs}")
        string(REPLACE "," ")," nargs "${nargs}")
        string(APPEND nargs ")")
    endif()

    string(APPEND out_content "extern \"C\" ${ret_type} ${func_name}(${args}) {\n") 
    string(APPEND out_content "    return webrogue::runtimes::embedded::sharedApiObject->${func_name}(${nargs})")
    if("${ret_type}" STREQUAL "void")
    else()
        string(APPEND out_content ".get()")
    endif()
    string(APPEND out_content ";\n")
    string(APPEND out_content "}\n")
    string(APPEND out_content "\n")
endforeach()

file(WRITE ${WR_API_EMBEDDING}.cpp "${out_content}")
