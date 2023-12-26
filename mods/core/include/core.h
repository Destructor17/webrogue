#pragma once

#include "common/colors.h"
#include "common/events.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef void (*webrogue_initialization_step)();
void webrogue_core_add_initialization_step(
    webrogue_initialization_step observer);
void webrogue_core_interrupt();

void webrogue_core_print(const char *);

webrogue_event *webrogue_core_get_events(size_t *out_event_count);

nr_glyph *webrogue_core_get_drawing_area(size_t *out_width, size_t *out_height);

#ifdef __cplusplus
}
#endif
