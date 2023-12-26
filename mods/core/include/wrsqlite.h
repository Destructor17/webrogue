#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

struct sqlite3nrStruct;
typedef sqlite3nrStruct *sqlite3nr;
typedef int64_t sqlite3nr_stmt;

#define SQLITE_OK 0 /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR 1       /* Generic error */
#define SQLITE_INTERNAL 2    /* Internal logic error in SQLite */
#define SQLITE_PERM 3        /* Access permission denied */
#define SQLITE_ABORT 4       /* Callback routine requested an abort */
#define SQLITE_BUSY 5        /* The database file is locked */
#define SQLITE_LOCKED 6      /* A table in the database is locked */
#define SQLITE_NOMEM 7       /* A malloc() failed */
#define SQLITE_READONLY 8    /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT 9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR 10      /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT 11    /* The database disk image is malformed */
#define SQLITE_NOTFOUND 12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL 13       /* Insertion failed because database is full */
#define SQLITE_CANTOPEN 14   /* Unable to open the database file */
#define SQLITE_PROTOCOL 15   /* Database lock protocol error */
#define SQLITE_EMPTY 16      /* Internal use only */
#define SQLITE_SCHEMA 17     /* The database schema changed */
#define SQLITE_TOOBIG 18     /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT 19 /* Abort due to constraint violation */
#define SQLITE_MISMATCH 20   /* Data type mismatch */
#define SQLITE_MISUSE 21     /* Library used incorrectly */
#define SQLITE_NOLFS 22      /* Uses OS features not supported on host */
#define SQLITE_AUTH 23       /* Authorization denied */
#define SQLITE_FORMAT 24     /* Not used */
#define SQLITE_RANGE 25      /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB 26     /* File opened that is not a database file */
#define SQLITE_NOTICE 27     /* Notifications from sqlite3_log() */
#define SQLITE_WARNING 28    /* Warnings from sqlite3_log() */
#define SQLITE_ROW 100       /* sqlite3_step() has another row ready */
#define SQLITE_DONE 101      /* sqlite3_step() has finished executing */

#define SQLITE_INTEGER 1
#define SQLITE_FLOAT 2
#define SQLITE_BLOB 4
#define SQLITE_NULL 5
#ifdef SQLITE_TEXT
#undef SQLITE_TEXT
#else
#define SQLITE_TEXT 3
#endif
#define SQLITE3_TEXT 3

typedef void (*sqlite3_destructor_type)(void *);
#define SQLITE_STATIC ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT ((sqlite3_destructor_type)-1)

int32_t sqlite3nr_open_v2(const char *filename, /* Database filename (UTF-8) */
                          sqlite3nr *ppDb,      /* OUT: SQLite db handle */
                          int32_t flags,        /* Flags */
                          const char *zVfs      /* Name of VFS module to use */
);
int32_t sqlite3nr_close(sqlite3nr db);
const char *sqlite3nr_errmsg(sqlite3nr db);
int32_t sqlite3nr_prepare_v2(
    sqlite3nr db,           /* Database handle */
    const char *zSql,       /* SQL statement, UTF-8 encoded */
    int32_t nByte,          /* Maximum length of zSql in bytes. */
    sqlite3nr_stmt *ppStmt, /* OUT: Statement handle */
    const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);
int32_t sqlite3nr_step(sqlite3nr_stmt);
int64_t sqlite3nr_last_insert_rowid(sqlite3nr);
int32_t sqlite3nr_changes(sqlite3nr);
int32_t sqlite3nr_column_int(sqlite3nr_stmt, int32_t iCol);
int32_t sqlite3nr_column_type(sqlite3nr_stmt, int32_t iCol);
const unsigned char *sqlite3nr_column_text(sqlite3nr_stmt, int32_t iCol);
double sqlite3nr_column_double(sqlite3nr_stmt, int32_t iCol);
int64_t sqlite3nr_column_int64(sqlite3nr_stmt, int32_t iCol);
int32_t sqlite3nr_column_bytes(sqlite3nr_stmt, int32_t iCol);
const void *sqlite3nr_column_blob(sqlite3nr_stmt, int32_t iCol);
int32_t sqlite3nr_finalize(sqlite3nr_stmt pStmt);
int32_t sqlite3nr_reset(sqlite3nr_stmt pStmt);
int32_t sqlite3nr_bind_int(sqlite3nr_stmt, int, int);
int32_t sqlite3nr_bind_null(sqlite3nr_stmt, int);
int32_t sqlite3nr_bind_text(sqlite3nr_stmt, int, const char *, int,
                            void (*)(void *));
int32_t sqlite3nr_bind_double(sqlite3nr_stmt, int, double);
int32_t sqlite3nr_bind_int64(sqlite3nr_stmt, int, int64_t);
int32_t sqlite3nr_bind_blob(sqlite3nr_stmt, int, const void *, int32_t n,
                            void (*)(void *));

#ifdef __cplusplus
}
#endif