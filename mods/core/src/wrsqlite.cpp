#include "../include/wrsqlite.h"
#include "../include/common/wr_api.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

extern "C" {
int32_t sqlite3nr_open_v2(const char *filename, /* Database filename (UTF-8) */
                          sqlite3nr *ppDb,      /* OUT: SQLite db handle */
                          int32_t flags,        /* Flags */
                          const char *zVfs      /* Name of VFS module to use */
) {
    *ppDb = (sqlite3nr)1;
    return SQLITE_OK;
}
int32_t sqlite3nr_close(sqlite3nr db) {
    return SQLITE_OK;
}
std::vector<char> wrsqliteErrmsg;
const char *sqlite3nr_errmsg(sqlite3nr db) {
    size_t size = nr_sqlite3_errmsg_size();
    wrsqliteErrmsg.resize(size + 1);
    nr_sqlite3_errmsg_get((int64_t)wrsqliteErrmsg.data());
    return wrsqliteErrmsg.data();
}
int32_t sqlite3nr_prepare_v2(
    sqlite3nr db,           /* Database handle */
    const char *zSql,       /* SQL statement, UTF-8 encoded */
    int32_t nByte,          /* Maximum length of zSql in bytes. */
    sqlite3nr_stmt *ppStmt, /* OUT: Statement handle */
    const char **pzTail     /* OUT: Pointer to unused portion of zSql */
) {
    return nr_sqlite3_prepare_v2((int64_t)zSql, nByte, (int64_t)ppStmt,
                                 (int64_t)pzTail);
}
int32_t sqlite3nr_step(sqlite3nr_stmt stmt) {
    return nr_sqlite3_step(stmt);
}
int64_t sqlite3nr_last_insert_rowid(sqlite3nr) {
    return nr_sqlite3_last_insert_rowid();
}
int32_t sqlite3nr_changes(sqlite3nr) {
    return nr_sqlite3_changes();
}
int32_t sqlite3nr_column_int(sqlite3nr_stmt stmt, int32_t iCol) {
    return nr_sqlite3_column_int(stmt, iCol);
}
int32_t sqlite3nr_column_type(sqlite3nr_stmt stmt, int32_t iCol) {
    return nr_sqlite3_column_type(stmt, iCol);
}
std::vector<unsigned char> lastText;
const unsigned char *sqlite3nr_column_text(sqlite3nr_stmt stmt, int32_t iCol) {
    int64_t len = nr_sqlite3_column_text_size(stmt, iCol);
    if (len < 0) {
        return nullptr;
    }
    if (lastText.size() < (len + 1)) {
        lastText.resize(len + 1);
    }
    nr_sqlite3_column_text_get(stmt, iCol, (int64_t)lastText.data());
    return lastText.data();
}
double sqlite3nr_column_double(sqlite3nr_stmt stmt, int32_t iCol) {
    return nr_sqlite3_column_double(stmt, iCol);
}
int64_t sqlite3nr_column_int64(sqlite3nr_stmt stmt, int32_t iCol) {
    return nr_sqlite3_column_int64(stmt, iCol);
}
int32_t sqlite3nr_column_bytes(sqlite3nr_stmt stmt, int32_t iCol) {
    return nr_sqlite3_column_bytes(stmt, iCol);
}
std::vector<unsigned char> lastBlob;
const void *sqlite3nr_column_blob(sqlite3nr_stmt stmt, int32_t iCol) {
    size_t len = sqlite3nr_column_bytes(stmt, iCol);
    if (lastBlob.size() < len) {
        lastBlob.resize(len);
    }
    nr_sqlite3_column_blob_get(stmt, iCol, (int64_t)lastBlob.data());
    return lastBlob.data();
}
int32_t sqlite3nr_finalize(sqlite3nr_stmt pStmt) {
    return nr_sqlite3_finalize(pStmt);
}
int32_t sqlite3nr_reset(sqlite3nr_stmt pStmt) {
    return nr_sqlite3_reset(pStmt);
}
int32_t sqlite3nr_bind_int(sqlite3nr_stmt stmt, int a, int b) {
    return nr_sqlite3_bind_int(stmt, a, b);
}
int32_t sqlite3nr_bind_null(sqlite3nr_stmt stmt, int a) {
    return nr_sqlite3_bind_null(stmt, a);
}
int32_t sqlite3nr_bind_text(sqlite3nr_stmt stmt, int a, const char *b, int c,
                            void (*d)(void *)) {
    assert(d == 0);
    return nr_sqlite3_bind_text(stmt, a, (int64_t)b, c >= 0 ? c : strlen(b));
}
int32_t sqlite3nr_bind_double(sqlite3nr_stmt stmt, int a, double b) {
    return nr_sqlite3_bind_double(stmt, a, b);
}
int32_t sqlite3nr_bind_int64(sqlite3nr_stmt stmt, int a, int64_t b) {
    return nr_sqlite3_bind_int64(stmt, a, b);
}
int32_t sqlite3nr_bind_blob(sqlite3nr_stmt stmt, int a, const void *b,
                            int32_t n, void (*)(void *)) {
    return nr_sqlite3_bind_blob(stmt, a, (int64_t)b, n);
}
}