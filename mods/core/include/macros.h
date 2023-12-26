#pragma once

#if !defined(WEBROGUE_EMBEDDED_MODS) && !defined(WEBROGUE_WASM_MODS)
#error missing webrogue mod defenitions
#endif

#if !defined(WEBROGUE_EMBEDDED_MODS)
#define NR_IMPORTED(RETURN_TYPE, NAME)                                         \
    __attribute__((import_name(#NAME)))                                        \
    __attribute__((import_module("webrogue"))) RETURN_TYPE NAME
#else
#define NR_IMPORTED(RETURN_TYPE, NAME) RETURN_TYPE NAME
#endif

#if !defined(WEBROGUE_EMBEDDED_MODS)
#define NR_EXPORTED(RETURN_TYPE, NAME)                                         \
    __attribute__((export_name(#NAME))) RETURN_TYPE NAME
#else
#define NR_EXPORTED(RETURN_TYPE, NAME) RETURN_TYPE NAME
#endif
