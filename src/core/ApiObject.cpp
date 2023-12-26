#include "../../mods/core/include/common/colors.h"
#include "Config.hpp"
#include "ModsRuntime.hpp"
#include "byteswap.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace webrogue {
namespace core {
ApiObject::ApiObject(ModsRuntime *pRuntime, Config *pConfig)
    : runtime(pRuntime), config(pConfig) {
}

#define WR_API_FUNCTION_IMPL(RET_TYPE, NAME, ARGS) RET_TYPE ApiObject::NAME ARGS

// rendering

WR_API_FUNCTION_IMPL(int32_t, nr_get_render_width, ()) {
    if (!runtime->isInitialized) {
        assert(false);
        return -1;
    }
    return output->size().x;
}
WR_API_FUNCTION_IMPL(int32_t, nr_get_render_height, ()) {
    if (!runtime->isInitialized) {
        assert(false);
        return -1;
    }
    return output->size().y;
}
WR_API_FUNCTION_IMPL(void, nr_start_color, ()) {
    if (!runtime->isInitialized) {
        assert(false);
        return;
    }
    output->startColor();
}
WR_API_FUNCTION_IMPL(int32_t, nr_get_color_pairs_count, ()) {
    if (!runtime->isInitialized) {
        assert(false);
        return -1;
    }
    return output->getColorPairsCount();
}
WR_API_FUNCTION_IMPL(int32_t, nr_get_colors_count, ()) {
    if (!runtime->isInitialized) {
        assert(false);
        return -1;
    }
    return output->getColorsCount();
}
WR_API_FUNCTION_IMPL(void, nr_set_color,
                     (int32_t color, int32_t r, int32_t g, int32_t b)) {
    if (!runtime->isInitialized) {
        assert(false);
        return;
    }
    output->setColor(color, r, g, b);
}
WR_API_FUNCTION_IMPL(void, nr_set_color_pair,
                     (int32_t color_pair, int32_t fg, int32_t bg)) {
    if (!runtime->isInitialized) {
        assert(false);
        return;
    }
    output->setColorPair(color_pair, fg, bg);
}
WR_API_FUNCTION_IMPL(void, nr_render_set_screen_data,
                     (uint64_t in_buff_offset, int64_t size)) {

    if (size != output->size().x * output->size().y) {
        assert(false);
        return;
    }
    if (!runtime->getVMData(output->getBuffer(), in_buff_offset,
                            size * sizeof(nr_glyph))) {
        assert(false);
        return;
    }
}
WR_API_FUNCTION_IMPL(int32_t, nr_interrupt, ()) {
    if (!runtime->isInitialized) {
        assert(false);
        return -1;
    }
    webrogue_event consoleWriterExitEvent = {webrogue_event_type::None, 0, 0};
    output->endFrame();
    runtime->onFrameEnd();
    while (true) { // frames loop
        output->beginFrame();
        rawEvents.clear();
        if (consoleWriterExitEvent.type != webrogue_event_type::None) {
            rawEvents.push_back({consoleWriterExitEvent.type,
                                 consoleWriterExitEvent.data1,
                                 consoleWriterExitEvent.data2, 0});
            consoleWriterExitEvent = {webrogue_event_type::None, 0, 0};
        }
        while (true) { // events loop
            webrogue_event event = output->getEvent();
            if (event.type == webrogue_event_type::None)
                break;
            rawEvents.push_back({event.type, event.data1, event.data2, 0});
            if (event.type == webrogue_event_type::Console)
                consoleWriter->isShown = true;
        } // events loop
        if (consoleWriter->isShown) {
            consoleWriterExitEvent = consoleWriter->present();
        } else {
            if (!rawEvents.empty()) {
                return rawEvents.size();
            }
            output->lazyEnd();
        }
    } // frames loop
}

WR_API_FUNCTION_IMPL(void, nr_copy_events,
                     (uint64_t out_buff_offset, int64_t size)) {
    if (!runtime->isInitialized) {
        assert(false);
        return;
    }
    if (size != rawEvents.size()) {
        assert(false);
        return;
    }
    if (!runtime->setVMData(rawEvents.data(), out_buff_offset,
                            size * sizeof(webrogue_raw_event))) {
        assert(false);
        return;
    }
}

// debug

WR_API_FUNCTION_IMPL(void, nr_debug_print,
                     (uint64_t in_buff_offset, int64_t size)) {
    std::vector<char> hostData;
    hostData.resize(size + 1);
    if (!runtime->getVMData(hostData.data(), in_buff_offset, size)) {
        assert(false);
        return;
    }
    hostData[size] = '\0';
    std::string hostString = hostData.data();
    *runtime->wrout << hostString;
}

// sqlite

WR_API_FUNCTION_IMPL(int64_t, nr_sqlite3_errmsg_size, ()) {
    return strlen(sqlite3_errmsg(db->getDb()));
}
WR_API_FUNCTION_IMPL(void, nr_sqlite3_errmsg_get, (uint64_t out_err_offset)) {
    const char *err = sqlite3_errmsg(db->getDb());
    size_t len = strlen(err);
    if (!runtime->setVMData(err, out_err_offset, len + 1)) {
        assert(false);
        return;
    }
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_prepare_v2,
                     (uint64_t in_zSql_offset, int64_t zSql_size,
                      uint64_t out_ppStmt_offset, uint64_t out_pzTail_offset)) {
    std::vector<char> zSql;
    zSql.resize(zSql_size + 1);

    if (!runtime->getVMData(zSql.data(), in_zSql_offset, zSql_size)) {
        assert(false);
        return -1;
    }
    zSql[zSql_size] = '\0';

    const char *hostPzTail = nullptr;
    int64_t stmtId;

    int result = sqlite3_prepare_v2(
        db->getDb(), zSql.data(), zSql_size, db->stmtNew(&stmtId),
        out_pzTail_offset == 0 ? &hostPzTail : nullptr);
    if (out_pzTail_offset != 0) {
        auto pzTailResult = in_zSql_offset + (hostPzTail - zSql.data());
        switch (runtime->voidptrSize()) {
        case 4: {
            uint32_t pztail = byteswap<uint32_t>(pzTailResult);
            if (!runtime->setVMData(&pztail, out_pzTail_offset,
                                    sizeof(uint32_t))) {
                assert(false);
                return -1;
            }
            break;
        }

        case 8: {
            uint64_t pztail = byteswap<uint32_t>(pzTailResult);
            if (!runtime->setVMData(&pztail, out_pzTail_offset,
                                    sizeof(uint64_t))) {
                assert(false);
                return -1;
            }
            break;
        }
        }
    }
    if (result != SQLITE_OK)
        db->stmtDelete(stmtId);
    else {
        stmtId = byteswap<int64_t>(stmtId);
        if (!runtime->setVMData(&stmtId, out_ppStmt_offset, sizeof(int64_t))) {
            assert(false);
            return -1;
        }
    }
    return result;
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_step, (int64_t stmt)) {
    return sqlite3_step(db->stmtById(stmt));
}
WR_API_FUNCTION_IMPL(int64_t, nr_sqlite3_last_insert_rowid, ()) {
    return sqlite3_last_insert_rowid(db->getDb());
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_changes, ()) {
    return sqlite3_changes(db->getDb());
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_column_int,
                     (int64_t stmt, int32_t iCol)) {
    return sqlite3_column_int(db->stmtById(stmt), iCol);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_column_type,
                     (int64_t stmt, int32_t iCol)) {
    return sqlite3_column_type(db->stmtById(stmt), iCol);
}
WR_API_FUNCTION_IMPL(int64_t, nr_sqlite3_column_text_size,
                     (int64_t stmt, int32_t iCol)) {

    const char *text =
        (const char *)sqlite3_column_text(db->stmtById(stmt), iCol);
    if (text)
        return strlen(text);
    return -1;
}
WR_API_FUNCTION_IMPL(void, nr_sqlite3_column_text_get,
                     (int64_t stmt, int32_t iCol, uint64_t out_result_offset)) {
    const char *text =
        (const char *)sqlite3_column_text(db->stmtById(stmt), iCol);
    size_t len = strlen(text);
    if (!runtime->setVMData(text, out_result_offset, len + 1)) {
        assert(false);
        return;
    }
}
WR_API_FUNCTION_IMPL(double, nr_sqlite3_column_double,
                     (int64_t stmt, int32_t iCol)) {
    return sqlite3_column_double(db->stmtById(stmt), iCol);
}
WR_API_FUNCTION_IMPL(int64_t, nr_sqlite3_column_int64,
                     (int64_t stmt, int32_t iCol)) {
    return sqlite3_column_int64(db->stmtById(stmt), iCol);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_column_bytes,
                     (int64_t stmt, int32_t iCol)) {
    return sqlite3_column_bytes(db->stmtById(stmt), iCol);
}
WR_API_FUNCTION_IMPL(void, nr_sqlite3_column_blob_get,
                     (int64_t stmt, int32_t iCol, uint64_t out_result_offset)) {
    size_t len = sqlite3_column_bytes(db->stmtById(stmt), iCol);
    if (!runtime->setVMData(sqlite3_column_blob(db->stmtById(stmt), iCol),
                            out_result_offset, len)) {
        assert(false);
        return;
    }
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_finalize, (int64_t stmt)) {

    int result = sqlite3_finalize(db->stmtById(stmt));
    if (result == SQLITE_OK)
        db->stmtDelete(stmt);
    return result;
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_reset, (int64_t stmt)) {
    return sqlite3_reset(db->stmtById(stmt));
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_bind_int,
                     (int64_t stmt, int32_t a, int32_t b)) {

    return sqlite3_bind_int(db->stmtById(stmt), a, b);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_bind_null, (int64_t stmt, int32_t a)) {

    return sqlite3_bind_null(db->stmtById(stmt), a);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_bind_text,
                     (int64_t stmt, int32_t a, uint64_t in_text_offset,
                      int64_t b_size)) {
    std::vector<char> hostText;
    hostText.resize(b_size + 1);
    if (!runtime->getVMData(hostText.data(), in_text_offset, b_size)) {
        assert(false);
        return -1;
    }
    hostText[b_size] = '\0';

    return sqlite3_bind_text(db->stmtById(stmt), a, hostText.data(), b_size,
                             SQLITE_TRANSIENT);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_bind_double,
                     (int64_t stmt, int32_t a, double b)) {
    return sqlite3_bind_double(db->stmtById(stmt), a, b);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_bind_int64,
                     (int64_t stmt, int32_t a, int64_t b)) {

    return sqlite3_bind_int64(db->stmtById(stmt), a, b);
}
WR_API_FUNCTION_IMPL(int32_t, nr_sqlite3_bind_blob,
                     (int64_t stmt, int32_t a, uint64_t in_blob_offset,
                      int32_t n)) {
    std::vector<char> hostBlob;
    hostBlob.resize(n);
    if (!runtime->getVMData(hostBlob.data(), in_blob_offset, n)) {
        assert(false);
        return -1;
    }
    return sqlite3_bind_blob(db->stmtById(stmt), a, hostBlob.data(), n,
                             nullptr);
}

} // namespace core
} // namespace webrogue
