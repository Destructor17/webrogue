#include "wasm2c_runtime.hpp"

// clang-format off
extern "C" {
u32 w2c_webrogue_nr_get_render_width(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_get_render_width();
}

u32 w2c_webrogue_nr_get_render_height(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_get_render_height();
}

void w2c_webrogue_nr_start_color(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_start_color();
}

u32 w2c_webrogue_nr_get_color_pairs_count(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_get_color_pairs_count();
}

u32 w2c_webrogue_nr_get_colors_count(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_get_colors_count();
}

void w2c_webrogue_nr_set_color(struct w2c_webrogue *env, u32 color, u32 r, u32 g, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_set_color(color, r, g, b);
}

void w2c_webrogue_nr_set_color_pair(struct w2c_webrogue *env, u32 color_pair, u32 fg, u32 bg) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_set_color_pair(color_pair, fg, bg);
}

void w2c_webrogue_nr_render_set_screen_data(struct w2c_webrogue *env, u64 in_buff_offset, u64 size) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_render_set_screen_data(in_buff_offset, size);
}

u32 w2c_webrogue_nr_interrupt(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_interrupt();
}

void w2c_webrogue_nr_copy_events(struct w2c_webrogue *env, u64 out_buff_offset, u64 size) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_copy_events(out_buff_offset, size);
}

void w2c_webrogue_nr_debug_print(struct w2c_webrogue *env, u64 in_buff_offset, u64 size) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_debug_print(in_buff_offset, size);
}

u64 w2c_webrogue_nr_sqlite3_errmsg_size(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_errmsg_size();
}

void w2c_webrogue_nr_sqlite3_errmsg_get(struct w2c_webrogue *env, u64 out_err_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_errmsg_get(out_err_offset);
}

u32 w2c_webrogue_nr_sqlite3_prepare_v2(struct w2c_webrogue *env, u64 in_zSql_offset, u64 zSql_size, u64 out_ppStmt_offset, u64 out_pzTail_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_prepare_v2(in_zSql_offset, zSql_size, out_ppStmt_offset, out_pzTail_offset);
}

u32 w2c_webrogue_nr_sqlite3_step(struct w2c_webrogue *env, u64 stmt) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_step(stmt);
}

u64 w2c_webrogue_nr_sqlite3_last_insert_rowid(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_last_insert_rowid();
}

u32 w2c_webrogue_nr_sqlite3_changes(struct w2c_webrogue *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_changes();
}

u32 w2c_webrogue_nr_sqlite3_column_int(struct w2c_webrogue *env, u64 stmt, u32 iCol) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_int(stmt, iCol);
}

u32 w2c_webrogue_nr_sqlite3_column_type(struct w2c_webrogue *env, u64 stmt, u32 iCol) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_type(stmt, iCol);
}

u64 w2c_webrogue_nr_sqlite3_column_text_size(struct w2c_webrogue *env, u64 stmt, u32 iCol) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_text_size(stmt, iCol);
}

void w2c_webrogue_nr_sqlite3_column_text_get(struct w2c_webrogue *env, u64 stmt, u32 iCol, u64 out_result_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_text_get(stmt, iCol, out_result_offset);
}

double w2c_webrogue_nr_sqlite3_column_double(struct w2c_webrogue *env, u64 stmt, u32 iCol) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_double(stmt, iCol);
}

u64 w2c_webrogue_nr_sqlite3_column_int64(struct w2c_webrogue *env, u64 stmt, u32 iCol) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_int64(stmt, iCol);
}

u32 w2c_webrogue_nr_sqlite3_column_bytes(struct w2c_webrogue *env, u64 stmt, u32 iCol) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_bytes(stmt, iCol);
}

void w2c_webrogue_nr_sqlite3_column_blob_get(struct w2c_webrogue *env, u64 stmt, u32 iCol, u64 out_result_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_column_blob_get(stmt, iCol, out_result_offset);
}

u32 w2c_webrogue_nr_sqlite3_finalize(struct w2c_webrogue *env, u64 stmt) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_finalize(stmt);
}

u32 w2c_webrogue_nr_sqlite3_reset(struct w2c_webrogue *env, u64 stmt) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_reset(stmt);
}

u32 w2c_webrogue_nr_sqlite3_bind_int(struct w2c_webrogue *env, u64 stmt, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_bind_int(stmt, a, b);
}

u32 w2c_webrogue_nr_sqlite3_bind_null(struct w2c_webrogue *env, u64 stmt, u32 a) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_bind_null(stmt, a);
}

u32 w2c_webrogue_nr_sqlite3_bind_text(struct w2c_webrogue *env, u64 stmt, u32 a, u64 in_text_offset, u64 b_size) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_bind_text(stmt, a, in_text_offset, b_size);
}

u32 w2c_webrogue_nr_sqlite3_bind_double(struct w2c_webrogue *env, u64 stmt, u32 a, double b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_bind_double(stmt, a, b);
}

u32 w2c_webrogue_nr_sqlite3_bind_int64(struct w2c_webrogue *env, u64 stmt, u32 a, u64 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_bind_int64(stmt, a, b);
}

u32 w2c_webrogue_nr_sqlite3_bind_blob(struct w2c_webrogue *env, u64 stmt, u32 a, u64 in_blob_offset, u32 n) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->apiObject.nr_sqlite3_bind_blob(stmt, a, in_blob_offset, n);
}

}
