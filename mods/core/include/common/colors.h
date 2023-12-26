#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint32_t nr_color_pair;

typedef uint32_t nr_char;

typedef struct nr_glyph {
    nr_char unicode_char;
    nr_color_pair color;
} nr_glyph;
#ifdef __cplusplus
static_assert(sizeof(nr_glyph) == 8, "wrong nr_glyph size");
#endif

#ifdef __cplusplus
}
#endif
