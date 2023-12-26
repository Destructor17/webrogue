#pragma once

#include <cstdint>

// clang-format off
extern "C" int32_t nr_get_render_width();

extern "C" int32_t nr_get_render_height();

extern "C" void nr_start_color();

extern "C" int32_t nr_get_color_pairs_count();

extern "C" int32_t nr_get_colors_count();

extern "C" void nr_set_color(int32_t color, int32_t r, int32_t g, int32_t b);

extern "C" void nr_set_color_pair(int32_t color_pair, int32_t fg, int32_t bg);

extern "C" void nr_render_set_screen_data(uint64_t in_buff_offset, int64_t size);

extern "C" int32_t nr_interrupt();

extern "C" void nr_copy_events(uint64_t out_buff_offset, int64_t size);

extern "C" void nr_debug_print(uint64_t in_buff_offset, int64_t size);

extern "C" int64_t nr_sqlite3_errmsg_size();

extern "C" void nr_sqlite3_errmsg_get(uint64_t out_err_offset);

extern "C" int32_t nr_sqlite3_prepare_v2(uint64_t in_zSql_offset, int64_t zSql_size, uint64_t out_ppStmt_offset, uint64_t out_pzTail_offset);

extern "C" int32_t nr_sqlite3_step(int64_t stmt);

extern "C" int64_t nr_sqlite3_last_insert_rowid();

extern "C" int32_t nr_sqlite3_changes();

extern "C" int32_t nr_sqlite3_column_int(int64_t stmt, int32_t iCol);

extern "C" int32_t nr_sqlite3_column_type(int64_t stmt, int32_t iCol);

extern "C" int64_t nr_sqlite3_column_text_size(int64_t stmt, int32_t iCol);

extern "C" void nr_sqlite3_column_text_get(int64_t stmt, int32_t iCol, uint64_t out_result_offset);

extern "C" double nr_sqlite3_column_double(int64_t stmt, int32_t iCol);

extern "C" int64_t nr_sqlite3_column_int64(int64_t stmt, int32_t iCol);

extern "C" int32_t nr_sqlite3_column_bytes(int64_t stmt, int32_t iCol);

extern "C" void nr_sqlite3_column_blob_get(int64_t stmt, int32_t iCol, uint64_t out_result_offset);

extern "C" int32_t nr_sqlite3_finalize(int64_t stmt);

extern "C" int32_t nr_sqlite3_reset(int64_t stmt);

extern "C" int32_t nr_sqlite3_bind_int(int64_t stmt, int32_t a, int32_t b);

extern "C" int32_t nr_sqlite3_bind_null(int64_t stmt, int32_t a);

extern "C" int32_t nr_sqlite3_bind_text(int64_t stmt, int32_t a, uint64_t in_text_offset, int64_t b_size);

extern "C" int32_t nr_sqlite3_bind_double(int64_t stmt, int32_t a, double b);

extern "C" int32_t nr_sqlite3_bind_int64(int64_t stmt, int32_t a, int64_t b);

extern "C" int32_t nr_sqlite3_bind_blob(int64_t stmt, int32_t a, uint64_t in_blob_offset, int32_t n);

