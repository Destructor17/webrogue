#include "shared_api_object.hpp"

// clang-format off
extern "C" int32_t nr_get_render_width() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_get_render_width();
}

extern "C" int32_t nr_get_render_height() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_get_render_height();
}

extern "C" void nr_start_color() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_start_color();
}

extern "C" int32_t nr_get_color_pairs_count() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_get_color_pairs_count();
}

extern "C" int32_t nr_get_colors_count() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_get_colors_count();
}

extern "C" void nr_set_color(int32_t color, int32_t r, int32_t g, int32_t b) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_set_color(color, r, g, b);
}

extern "C" void nr_set_color_pair(int32_t color_pair, int32_t fg, int32_t bg) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_set_color_pair(color_pair, fg, bg);
}

extern "C" void nr_render_set_screen_data(uint64_t in_buff_offset, int64_t size) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_render_set_screen_data(in_buff_offset, size);
}

extern "C" int32_t nr_interrupt() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_interrupt();
}

extern "C" void nr_copy_events(uint64_t out_buff_offset, int64_t size) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_copy_events(out_buff_offset, size);
}

extern "C" void nr_debug_print(uint64_t in_buff_offset, int64_t size) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_debug_print(in_buff_offset, size);
}

extern "C" int64_t nr_sqlite3_errmsg_size() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_errmsg_size();
}

extern "C" void nr_sqlite3_errmsg_get(uint64_t out_err_offset) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_errmsg_get(out_err_offset);
}

extern "C" int32_t nr_sqlite3_prepare_v2(uint64_t in_zSql_offset, int64_t zSql_size, uint64_t out_ppStmt_offset, uint64_t out_pzTail_offset) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_prepare_v2(in_zSql_offset, zSql_size, out_ppStmt_offset, out_pzTail_offset);
}

extern "C" int32_t nr_sqlite3_step(int64_t stmt) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_step(stmt);
}

extern "C" int64_t nr_sqlite3_last_insert_rowid() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_last_insert_rowid();
}

extern "C" int32_t nr_sqlite3_changes() {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_changes();
}

extern "C" int32_t nr_sqlite3_column_int(int64_t stmt, int32_t iCol) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_int(stmt, iCol);
}

extern "C" int32_t nr_sqlite3_column_type(int64_t stmt, int32_t iCol) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_type(stmt, iCol);
}

extern "C" int64_t nr_sqlite3_column_text_size(int64_t stmt, int32_t iCol) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_text_size(stmt, iCol);
}

extern "C" void nr_sqlite3_column_text_get(int64_t stmt, int32_t iCol, uint64_t out_result_offset) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_text_get(stmt, iCol, out_result_offset);
}

extern "C" double nr_sqlite3_column_double(int64_t stmt, int32_t iCol) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_double(stmt, iCol);
}

extern "C" int64_t nr_sqlite3_column_int64(int64_t stmt, int32_t iCol) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_int64(stmt, iCol);
}

extern "C" int32_t nr_sqlite3_column_bytes(int64_t stmt, int32_t iCol) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_bytes(stmt, iCol);
}

extern "C" void nr_sqlite3_column_blob_get(int64_t stmt, int32_t iCol, uint64_t out_result_offset) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_column_blob_get(stmt, iCol, out_result_offset);
}

extern "C" int32_t nr_sqlite3_finalize(int64_t stmt) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_finalize(stmt);
}

extern "C" int32_t nr_sqlite3_reset(int64_t stmt) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_reset(stmt);
}

extern "C" int32_t nr_sqlite3_bind_int(int64_t stmt, int32_t a, int32_t b) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_bind_int(stmt, a, b);
}

extern "C" int32_t nr_sqlite3_bind_null(int64_t stmt, int32_t a) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_bind_null(stmt, a);
}

extern "C" int32_t nr_sqlite3_bind_text(int64_t stmt, int32_t a, uint64_t in_text_offset, int64_t b_size) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_bind_text(stmt, a, in_text_offset, b_size);
}

extern "C" int32_t nr_sqlite3_bind_double(int64_t stmt, int32_t a, double b) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_bind_double(stmt, a, b);
}

extern "C" int32_t nr_sqlite3_bind_int64(int64_t stmt, int32_t a, int64_t b) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_bind_int64(stmt, a, b);
}

extern "C" int32_t nr_sqlite3_bind_blob(int64_t stmt, int32_t a, uint64_t in_blob_offset, int32_t n) {
    return webrogue::runtimes::embedded::sharedApiObject->nr_sqlite3_bind_blob(stmt, a, in_blob_offset, n);
}

