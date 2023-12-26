include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/parse_api_header.cmake)

parse_api_header(FUNC_COUNT num_of_funcs)

set(out_content "#include \"wasm2c_runtime.hpp\"\n\n// clang-format off\n")
string(APPEND out_content "extern \"C\" {\n")
foreach(func_i RANGE 1 ${num_of_funcs})
    parse_api_header(
        FUNC_ID ${func_i} 
        FUNC_NAME func_name
        RET_TYPE ret_type
        ARGS args
        ARGS_WITHOUT_TYPES args_without_types
    )
    if("${args}" STREQUAL "")
        set(delim "")
    else()
        set(delim ", ")
    endif()
    set(w2c_args ${args})
    string(REPLACE "void *" "u32 " w2c_args "${w2c_args}")
    string(REPLACE "uint32_t" "u32" w2c_args "${w2c_args}")
    string(REPLACE "int32_t" "u32" w2c_args "${w2c_args}")
    string(REPLACE "uint64_t" "u64" w2c_args "${w2c_args}")
    string(REPLACE "int64_t" "u64" w2c_args "${w2c_args}")
    string(REPLACE "double" "double" w2c_args "${w2c_args}")

    set(w2c_args_without_types ${args})
    string(REPLACE "void *" "runtime->linked.w2c_memory.data + " w2c_args_without_types "${w2c_args_without_types}")
    string(REPLACE "uint32_t " "" w2c_args_without_types "${w2c_args_without_types}")
    string(REPLACE "int32_t " "" w2c_args_without_types "${w2c_args_without_types}")
    string(REPLACE "uint64_t " "" w2c_args_without_types "${w2c_args_without_types}")
    string(REPLACE "int64_t " "" w2c_args_without_types "${w2c_args_without_types}")
    string(REPLACE "double " "" w2c_args_without_types "${w2c_args_without_types}")

    set(w2c_ret_type ${ret_type})
    string(REPLACE "void *" "void *" w2c_ret_type "${w2c_ret_type}")
    string(REPLACE "uint32_t" "u32" w2c_ret_type "${w2c_ret_type}")
    string(REPLACE "int32_t" "u32" w2c_ret_type "${w2c_ret_type}")
    string(REPLACE "uint64_t" "u64" w2c_ret_type "${w2c_ret_type}")
    string(REPLACE "int64_t" "u64" w2c_ret_type "${w2c_ret_type}")
    string(REPLACE "double" "double" w2c_ret_type "${w2c_ret_type}")

    string(APPEND out_content "${w2c_ret_type} w2c_webrogue_${func_name}(struct w2c_webrogue *env${delim}${w2c_args}) {\n") 
    string(APPEND out_content "    auto runtime =\n")
    string(APPEND out_content "        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);\n")
    string(APPEND out_content "    return runtime->apiObject.${func_name}(${w2c_args_without_types});\n")
    string(APPEND out_content "}\n")
    string(APPEND out_content "\n")
endforeach()
string(APPEND out_content "}\n")
file(WRITE ${NR_API_EMBEDDING}.cpp "${out_content}")
