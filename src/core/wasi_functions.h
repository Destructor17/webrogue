#if !defined(NR_API_FUNCTION)
#include <stdint.h>
#define NR_API_FUNCTION(RET_TYPE, NAME, ARGS) RET_TYPE NAME ARGS;
#ifdef __cplusplus
extern "C" {
#define EXTERN_C
#endif
#endif

// clang-format off

NR_API_FUNCTION(int32_t,  environ_get,             (uint32_t ptrs, uint32_t buff))
NR_API_FUNCTION(int32_t,  environ_sizes_get,       (int32_t count_offset, int32_t buffsize_offset))
NR_API_FUNCTION(uint32_t, clock_time_get,          (uint32_t clk_id, uint64_t precision, uint32_t out_time_offset))
NR_API_FUNCTION(uint32_t, fd_close,                (uint32_t fd))
NR_API_FUNCTION(int32_t,  fd_fdstat_get,           (uint32_t fd, uint32_t out_offset))
NR_API_FUNCTION(int32_t,  fd_fdstat_set_flags,     (int32_t a, int32_t b))
NR_API_FUNCTION(int32_t,  fd_prestat_dir_name,     (uint32_t fd, uint32_t out, uint32_t len))
NR_API_FUNCTION(int32_t,  fd_prestat_get,          (uint32_t fd, uint32_t out))
NR_API_FUNCTION(int32_t,  fd_read,                 (int32_t a, int32_t b, int32_t c, int32_t g))
NR_API_FUNCTION(uint32_t, fd_readdir,              (uint32_t a, uint32_t b, uint32_t c, uint64_t d, uint32_t e))
NR_API_FUNCTION(int32_t,  fd_seek,                 (int32_t fd, int64_t offset, int32_t whence, uint32_t out_pos_offset))
NR_API_FUNCTION(uint32_t, fd_write,                (uint32_t fd, uint32_t raw_wasi_iovs_offset, uint32_t iovs_len, uint32_t out_nwritten_offset))
NR_API_FUNCTION(uint32_t, path_filestat_get,       (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e))
NR_API_FUNCTION(uint32_t, path_open,               (uint32_t dirfd, uint32_t dirflags, uint32_t in_path_offset, uint32_t path_len, uint32_t oflags, uint64_t fs_rights_base, uint64_t fs_rights_inheriting, uint32_t fs_flags, uint32_t out_fd_offset))
NR_API_FUNCTION(uint32_t, path_remove_directory,   (uint32_t a, uint32_t b, uint32_t c))
NR_API_FUNCTION(uint32_t, path_rename,             (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f))
NR_API_FUNCTION(uint32_t, path_unlink_file,        (uint32_t a, uint32_t b, uint32_t c))
NR_API_FUNCTION(void,     proc_exit,               (int32_t exit_code))
NR_API_FUNCTION(int32_t,  random_get,              (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  fd_sync,                 (uint32_t a))
NR_API_FUNCTION(int32_t,  fd_allocate,             (uint32_t a, uint64_t b, uint64_t c))
NR_API_FUNCTION(int32_t,  fd_datasync,             (uint32_t a))
NR_API_FUNCTION(int32_t,  fd_tell,                 (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  fd_pwrite,               (uint32_t a, uint32_t b, uint32_t c, uint64_t d, uint32_t e))
NR_API_FUNCTION(int32_t,  clock_res_get,           (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  fd_renumber  ,           (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  fd_pread,                (uint32_t a, uint32_t b, uint32_t c, uint64_t d, uint32_t e))
NR_API_FUNCTION(uint32_t, path_create_directory,   (uint32_t a, uint32_t b, uint32_t c))
NR_API_FUNCTION(int32_t,  sched_yield,             ())
NR_API_FUNCTION(int32_t,  fd_filestat_set_times,   (uint32_t a, uint64_t b, uint64_t c, uint32_t d))
NR_API_FUNCTION(int32_t,  args_sizes_get,          (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  fd_filestat_set_size,    (uint32_t a, uint64_t b))
NR_API_FUNCTION(int32_t,  sock_recv,               (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f))
NR_API_FUNCTION(int32_t,  fd_filestat_get,         (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  sock_shutdown,           (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  poll_oneoff,             (uint32_t a, uint32_t b, uint32_t c, uint32_t d))
NR_API_FUNCTION(int32_t,  path_link,               (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g))
NR_API_FUNCTION(int32_t,  sock_accept,             (uint32_t a, uint32_t b, uint32_t c))
NR_API_FUNCTION(int32_t,  path_readlink,           (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f))
NR_API_FUNCTION(int32_t,  fd_advise,               (uint32_t a, uint64_t b, uint64_t c, uint32_t d))
NR_API_FUNCTION(int32_t,  path_filestat_set_times, (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint64_t e, uint64_t f, uint32_t g))
NR_API_FUNCTION(int32_t,  args_get,                (uint32_t a, uint32_t b))
NR_API_FUNCTION(int32_t,  path_symlink,            (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e))
NR_API_FUNCTION(int32_t,  sock_send,               (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e))
NR_API_FUNCTION(int32_t,  fd_fdstat_set_rights,    (uint32_t a, uint64_t b, uint64_t c))








#ifdef EXTERN_C
}
#undef EXTERN_C
#endif

#undef NR_API_FUNCTION
