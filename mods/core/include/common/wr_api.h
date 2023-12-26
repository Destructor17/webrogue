#if !defined(WR_API_FUNCTION)
#include "../macros.h"
#include <stdint.h>
#define WR_API_FUNCTION(RET_TYPE, NAME, ARGS) NR_IMPORTED(RET_TYPE, NAME) ARGS;
#ifdef __cplusplus
extern "C" {
#define EXTERN_C
#endif
#endif

#include "wr_api_functions.def"

#ifdef EXTERN_C
}
#undef EXTERN_C
#endif

#undef WR_API_FUNCTION
