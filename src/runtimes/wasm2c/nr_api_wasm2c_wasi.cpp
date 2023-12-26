#include "wasm2c_runtime.hpp"

// clang-format off
extern "C" {
u32 w2c_wasi__snapshot__preview1_environ_get(struct w2c_wasi__snapshot__preview1 *env, u32 ptrs, u32 buff) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.environ_get(ptrs, buff);
}

u32 w2c_wasi__snapshot__preview1_environ_sizes_get(struct w2c_wasi__snapshot__preview1 *env, u32 count_offset, u32 buffsize_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.environ_sizes_get(count_offset, buffsize_offset);
}

u32 w2c_wasi__snapshot__preview1_clock_time_get(struct w2c_wasi__snapshot__preview1 *env, u32 clk_id, u64 precision, u32 out_time_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.clock_time_get(clk_id, precision, out_time_offset);
}

u32 w2c_wasi__snapshot__preview1_fd_close(struct w2c_wasi__snapshot__preview1 *env, u32 fd) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_close(fd);
}

u32 w2c_wasi__snapshot__preview1_fd_fdstat_get(struct w2c_wasi__snapshot__preview1 *env, u32 fd, u32 out_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_fdstat_get(fd, out_offset);
}

u32 w2c_wasi__snapshot__preview1_fd_fdstat_set_flags(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_fdstat_set_flags(a, b);
}

u32 w2c_wasi__snapshot__preview1_fd_prestat_dir_name(struct w2c_wasi__snapshot__preview1 *env, u32 fd, u32 out, u32 len) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_prestat_dir_name(fd, out, len);
}

u32 w2c_wasi__snapshot__preview1_fd_prestat_get(struct w2c_wasi__snapshot__preview1 *env, u32 fd, u32 out) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_prestat_get(fd, out);
}

u32 w2c_wasi__snapshot__preview1_fd_read(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 g) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_read(a, b, c, g);
}

u32 w2c_wasi__snapshot__preview1_fd_readdir(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u64 d, u32 e) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_readdir(a, b, c, d, e);
}

u32 w2c_wasi__snapshot__preview1_fd_seek(struct w2c_wasi__snapshot__preview1 *env, u32 fd, u64 offset, u32 whence, u32 out_pos_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_seek(fd, offset, whence, out_pos_offset);
}

u32 w2c_wasi__snapshot__preview1_fd_write(struct w2c_wasi__snapshot__preview1 *env, u32 fd, u32 raw_wasi_iovs_offset, u32 iovs_len, u32 out_nwritten_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_write(fd, raw_wasi_iovs_offset, iovs_len, out_nwritten_offset);
}

u32 w2c_wasi__snapshot__preview1_path_filestat_get(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_filestat_get(a, b, c, d, e);
}

u32 w2c_wasi__snapshot__preview1_path_open(struct w2c_wasi__snapshot__preview1 *env, u32 dirfd, u32 dirflags, u32 in_path_offset, u32 path_len, u32 oflags, u64 fs_rights_base, u64 fs_rights_inheriting, u32 fs_flags, u32 out_fd_offset) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_open(dirfd, dirflags, in_path_offset, path_len, oflags, fs_rights_base, fs_rights_inheriting, fs_flags, out_fd_offset);
}

u32 w2c_wasi__snapshot__preview1_path_remove_directory(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_remove_directory(a, b, c);
}

u32 w2c_wasi__snapshot__preview1_path_rename(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_rename(a, b, c, d, e, f);
}

u32 w2c_wasi__snapshot__preview1_path_unlink_file(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_unlink_file(a, b, c);
}

void w2c_wasi__snapshot__preview1_proc_exit(struct w2c_wasi__snapshot__preview1 *env, u32 exit_code) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.proc_exit(exit_code);
}

u32 w2c_wasi__snapshot__preview1_random_get(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.random_get(a, b);
}

u32 w2c_wasi__snapshot__preview1_fd_sync(struct w2c_wasi__snapshot__preview1 *env, u32 a) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_sync(a);
}

u32 w2c_wasi__snapshot__preview1_fd_allocate(struct w2c_wasi__snapshot__preview1 *env, u32 a, u64 b, u64 c) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_allocate(a, b, c);
}

u32 w2c_wasi__snapshot__preview1_fd_datasync(struct w2c_wasi__snapshot__preview1 *env, u32 a) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_datasync(a);
}

u32 w2c_wasi__snapshot__preview1_fd_tell(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_tell(a, b);
}

u32 w2c_wasi__snapshot__preview1_fd_pwrite(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u64 d, u32 e) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_pwrite(a, b, c, d, e);
}

u32 w2c_wasi__snapshot__preview1_clock_res_get(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.clock_res_get(a, b);
}

u32 w2c_wasi__snapshot__preview1_fd_renumber (struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_renumber (a, b);
}

u32 w2c_wasi__snapshot__preview1_fd_pread(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u64 d, u32 e) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_pread(a, b, c, d, e);
}

u32 w2c_wasi__snapshot__preview1_path_create_directory(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_create_directory(a, b, c);
}

u32 w2c_wasi__snapshot__preview1_sched_yield(struct w2c_wasi__snapshot__preview1 *env) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.sched_yield();
}

u32 w2c_wasi__snapshot__preview1_fd_filestat_set_times(struct w2c_wasi__snapshot__preview1 *env, u32 a, u64 b, u64 c, u32 d) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_filestat_set_times(a, b, c, d);
}

u32 w2c_wasi__snapshot__preview1_args_sizes_get(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.args_sizes_get(a, b);
}

u32 w2c_wasi__snapshot__preview1_fd_filestat_set_size(struct w2c_wasi__snapshot__preview1 *env, u32 a, u64 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_filestat_set_size(a, b);
}

u32 w2c_wasi__snapshot__preview1_sock_recv(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.sock_recv(a, b, c, d, e, f);
}

u32 w2c_wasi__snapshot__preview1_fd_filestat_get(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_filestat_get(a, b);
}

u32 w2c_wasi__snapshot__preview1_sock_shutdown(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.sock_shutdown(a, b);
}

u32 w2c_wasi__snapshot__preview1_poll_oneoff(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.poll_oneoff(a, b, c, d);
}

u32 w2c_wasi__snapshot__preview1_path_link(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_link(a, b, c, d, e, f, g);
}

u32 w2c_wasi__snapshot__preview1_sock_accept(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.sock_accept(a, b, c);
}

u32 w2c_wasi__snapshot__preview1_path_readlink(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_readlink(a, b, c, d, e, f);
}

u32 w2c_wasi__snapshot__preview1_fd_advise(struct w2c_wasi__snapshot__preview1 *env, u32 a, u64 b, u64 c, u32 d) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_advise(a, b, c, d);
}

u32 w2c_wasi__snapshot__preview1_path_filestat_set_times(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u64 e, u64 f, u32 g) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_filestat_set_times(a, b, c, d, e, f, g);
}

u32 w2c_wasi__snapshot__preview1_args_get(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.args_get(a, b);
}

u32 w2c_wasi__snapshot__preview1_path_symlink(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.path_symlink(a, b, c, d, e);
}

u32 w2c_wasi__snapshot__preview1_sock_send(struct w2c_wasi__snapshot__preview1 *env, u32 a, u32 b, u32 c, u32 d, u32 e) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.sock_send(a, b, c, d, e);
}

u32 w2c_wasi__snapshot__preview1_fd_fdstat_set_rights(struct w2c_wasi__snapshot__preview1 *env, u32 a, u64 b, u64 c) {
    auto runtime =
        ((webrogue::runtimes::wasm2c::Wasm2cModsRuntime *)env);
    return runtime->wasiObject.fd_fdstat_set_rights(a, b, c);
}

}
