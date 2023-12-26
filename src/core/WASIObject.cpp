#include "WASIObject.hpp"
#include "Config.hpp"
#include "ModsRuntime.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "wasi_templates.hpp"

// #if defined(__DJGPP__)
// #include <time.h>
// typedef int clockid_t;
// static inline int clock_getres(int clk_id, struct timespec *spec) {
//     return -1; // Defaults to 1000000
// }
// struct timespec {
//     uint64_t tv_sec;
//     uint64_t tv_nsec;
// };
// static inline int clock_gettime(int clk_id, struct timespec *spec) {
//     struct timeval tp;
//     struct timezone tzp;
//     gettimeofday(&tp, &tzp);
//     spec->tv_sec = tp.tv_sec;
//     spec->tv_nsec = tp.tv_sec * 1000;
//     return 0;
// }
// static inline clockid_t convert_clockid(nr_wasi_clockid_t in) {
//     return 0;
// }
// #elif defined(_WIN32)

// #if !defined(__MINGW32__)

// #define _AMD64_

// #include "sysinfoapi.h"

// static inline int clock_gettime(int clk_id, struct timespec *spec) {
//     __int64 wintime;
//     GetSystemTimeAsFileTime((FILETIME *)&wintime);
//     wintime -= 116444736000000000i64;            // 1jan1601 to 1jan1970
//     spec->tv_sec = wintime / 10000000i64;        // seconds
//     spec->tv_nsec = wintime % 10000000i64 * 100; // nano-seconds
//     return 0;
// }

// static inline int clock_getres(int clk_id, struct timespec *spec) {
//     return -1; // Defaults to 1000000
// }

// #endif

// static inline clockid_t convert_clockid(nr_wasi_clockid_t in) {
//     return 0;
// }

// #else // _WIN32

// #include <time.h>

// static inline clockid_t convert_clockid(nr_wasi_clockid_t in) {
//     switch (in) {
//     case __WASI_CLOCKID_MONOTONIC:
//         return CLOCK_MONOTONIC;
//     case __WASI_CLOCKID_PROCESS_CPUTIME_ID:
//         return CLOCK_PROCESS_CPUTIME_ID;
//     case __WASI_CLOCKID_REALTIME:
//         return CLOCK_REALTIME;
//     case __WASI_CLOCKID_THREAD_CPUTIME_ID:
//         return CLOCK_THREAD_CPUTIME_ID;
//     default:
//         return (clockid_t)-1;
//     }
// }

// #endif // _WIN32

namespace webrogue {
namespace core {
WASIObject::WASIObject(ModsRuntime *pRuntime, ResourceStorage *resourceStorage,
                       Config *config)
    : runtime(pRuntime), vfs(resourceStorage, config) {
}

NR_API_FUNCTION(int32_t, environ_get, (uint32_t ptrs, uint32_t buff)) {
    assert(false);
    return 28;
}

NR_API_FUNCTION(int32_t, environ_sizes_get,
                (int32_t count_offset, int32_t buffsize_offset)) {
    int32_t data = byteswap<int32_t>(0);
    WASI_CHECK(runtime->setVMData(&data, count_offset, sizeof(int32_t)));
    WASI_CHECK(runtime->setVMData(&data, buffsize_offset, sizeof(int32_t)));
    return 0;
}

NR_API_FUNCTION(int32_t, random_get, (uint32_t a, uint32_t b)) {
    return 0;
}

NR_API_FUNCTION(uint32_t, clock_time_get,
                (uint32_t clk_id, uint64_t precision,
                 uint32_t out_time_offset)) {

    // // m3ApiGetArg(__wasi_clockid_t, wasi_clk_id)
    // //     m3ApiGetArg(__wasi_timestamp_t, precision)
    // //         m3ApiGetArgMem(__wasi_timestamp_t *, time)

    // //             m3ApiCheckMem(time, sizeof(__wasi_timestamp_t));

    // clockid_t clk = convert_clockid(clk_id);
    // if (clk < 0)
    //     return 28;

    // struct timespec tp;
    // if (clock_gettime(clk, &tp) != 0) {
    //     return 8;
    // }

    // WASI_CHECK(
    //     runtime->setVMData(&tp, out_time_offset,
    //     sizeof(nr_wasi_timestamp_t)));
    // // m3ApiWriteMem64(time, convert_timespec(&tp));
    return 0;
}

NR_API_FUNCTION(void, proc_exit, (int32_t a)) {
    abort();
}

NR_API_FUNCTION(int32_t, fd_sync, (uint32_t a)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_allocate, (uint32_t a, uint64_t b, uint64_t c)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_datasync, (uint32_t a)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_tell, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_pwrite,
                (uint32_t a, uint32_t b, uint32_t c, uint64_t d, uint32_t e)) {
    abort();
}
NR_API_FUNCTION(int32_t, clock_res_get, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_renumber, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_pread,
                (uint32_t a, uint32_t b, uint32_t c, uint64_t d, uint32_t e)) {
    abort();
}
NR_API_FUNCTION(uint32_t, path_create_directory,
                (uint32_t a, uint32_t b, uint32_t c)) {
    abort();
}
NR_API_FUNCTION(int32_t, sched_yield, ()) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_filestat_set_times,
                (uint32_t a, uint64_t b, uint64_t c, uint32_t d)) {
    abort();
}
NR_API_FUNCTION(int32_t, args_sizes_get, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_filestat_set_size, (uint32_t a, uint64_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, sock_recv,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                 uint32_t f)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_filestat_get, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, sock_shutdown, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, poll_oneoff,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d)) {
    abort();
}
NR_API_FUNCTION(int32_t, path_link,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                 uint32_t f, uint32_t g)) {
    abort();
}
NR_API_FUNCTION(int32_t, sock_accept, (uint32_t a, uint32_t b, uint32_t c)) {
    abort();
}
NR_API_FUNCTION(int32_t, path_readlink,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                 uint32_t f)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_advise,
                (uint32_t a, uint64_t b, uint64_t c, uint32_t d)) {
    abort();
}
NR_API_FUNCTION(int32_t, path_filestat_set_times,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint64_t e,
                 uint64_t f, uint32_t g)) {
    abort();
}
NR_API_FUNCTION(int32_t, args_get, (uint32_t a, uint32_t b)) {
    abort();
}
NR_API_FUNCTION(int32_t, path_symlink,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)) {
    abort();
}
NR_API_FUNCTION(int32_t, sock_send,
                (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)) {
    abort();
}
NR_API_FUNCTION(int32_t, fd_fdstat_set_rights,
                (uint32_t a, uint64_t b, uint64_t c)) {
    abort();
}

} // namespace core
} // namespace webrogue
