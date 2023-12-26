#include "Config.hpp"
#include "ModsRuntime.hpp"
#include "WASIObject.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "VFS.hpp"
#include "wasi_templates.hpp"

namespace webrogue {
namespace core {

WASI_FUNCTION_IMPL(uint32_t, fd_close, (uint32_t fd)) {
    return vfs.close(fd) ? 0 : 8;
}

WASI_FUNCTION_IMPL(int32_t, fd_fdstat_get, (uint32_t fd, uint32_t out_offset)) {
    nr_wasi_fdstat_t fdstat;
    fdstat.fs_filetype = byteswap<nr_wasi_filetype_t>(3); // 4 for file
    fdstat.fs_flags = byteswap<nr_wasi_fdflags_t>(0);
    fdstat.fs_rights_base =
        byteswap<nr_wasi_rights_t>((uint64_t)-1); // all rights
    fdstat.fs_rights_inheriting =
        byteswap<nr_wasi_rights_t>((uint64_t)-1); // all rights
    WASI_CHECK(
        runtime->setVMData(&fdstat, out_offset, sizeof(nr_wasi_fdstat_t)));
    return 0;
}

WASI_FUNCTION_IMPL(int32_t, fd_fdstat_set_flags, (int32_t a, int32_t b)) {
    // TODO
    return 0;
}

WASI_FUNCTION_IMPL(int32_t, fd_prestat_dir_name,
                   (uint32_t fd, uint32_t out, uint32_t len)) {
    std::string name;
    if (!vfs.preopendDirName(fd, name)) {
        return 8;
    }
    if (len != name.size() + 1) {
        return 8;
    }
    WASI_CHECK(runtime->setVMData(name.c_str(), out, len));
    return 0;
}

WASI_FUNCTION_IMPL(int32_t, fd_prestat_get, (uint32_t fd, uint32_t out)) {
    std::string name;
    if (!vfs.preopendDirName(fd, name)) {
        return 8;
    }
    int32_t ret[2];
    ret[0] = byteswap<int32_t>(0);
    ret[1] = byteswap<int32_t>(name.size() + 1);
    WASI_CHECK(runtime->setVMData(ret, out, sizeof(int32_t) * 2));
    return 0;
}

WASI_FUNCTION_IMPL(int32_t, fd_read,
                   (int32_t fd, int32_t raw_wasi_iovs_offset, int32_t iovs_len,
                    int32_t out_nread_offset)) {

    size_t hostNread = 0;
    bool hasError = false;
    std::vector<uint8_t> hostData;
    for (int i = 0; i < iovs_len; i++) {
        nr_wasi_iovec_t io;
        WASI_CHECK(runtime->getVMData(
            &io, allignedOffset<nr_wasi_iovec_t>(raw_wasi_iovs_offset, i),
            sizeof(nr_wasi_iovec_t)));

        nr_wasi_size_t buffOffset = byteswap<nr_wasi_size_t>(io.buf);
        nr_wasi_size_t size = byteswap<nr_wasi_size_t>(io.buf_len);

        if (!size)
            continue;
        hostData.resize(size);

        size_t currentNread;

        if (!vfs.read(fd, hostData.data(), size, currentNread)) {
            hasError = true;
            assert(false);
            break;
        }
        WASI_CHECK(runtime->setVMData(hostData.data(), buffOffset, size));
        hostNread += currentNread;
    }

    nr_wasi_size_t nread = byteswap<nr_wasi_size_t>(hostNread);

    WASI_CHECK(
        runtime->setVMData(&nread, out_nread_offset, sizeof(nr_wasi_size_t)));
    return hasError ? 8 : 0;
}

WASI_FUNCTION_IMPL(uint32_t, fd_readdir,
                   (uint32_t a, uint32_t b, uint32_t c, uint64_t d,
                    uint32_t e)) {
    assert(false);
    return 8;
}

WASI_FUNCTION_IMPL(int32_t, fd_seek,
                   (int32_t fd, int64_t offset, int32_t whence,
                    uint32_t out_pos_offset)) {
    size_t hostPos;
    if (vfs.seek(fd, offset, whence, hostPos)) {
        nr_wasi_filesize_t pos = byteswap<nr_wasi_filesize_t>(hostPos);
        WASI_CHECK(runtime->setVMData(&pos, out_pos_offset,
                                      sizeof(nr_wasi_filesize_t)));
        return 0;
    }
    return 8;
}

WASI_FUNCTION_IMPL(uint32_t, fd_write,
                   (uint32_t fd, uint32_t raw_wasi_iovs_offset,
                    uint32_t iovs_len, uint32_t out_nwritten_offset)) {

    size_t hostNwritten = 0;
    bool hasError = false;
    std::vector<uint8_t> hostData;
    for (int i = 0; i < iovs_len; i++) {
        nr_wasi_iovec_t io;
        WASI_CHECK(runtime->getVMData(
            &io, allignedOffset<nr_wasi_iovec_t>(raw_wasi_iovs_offset, i),
            sizeof(nr_wasi_iovec_t)));

        nr_wasi_size_t buffOffset = byteswap<nr_wasi_size_t>(io.buf);
        nr_wasi_size_t size = byteswap<nr_wasi_size_t>(io.buf_len);

        if (!size)
            continue;
        hostData.resize(size);
        WASI_CHECK(runtime->getVMData(hostData.data(), buffOffset, size));

        if (!vfs.write(fd, hostData.data(), size)) {
            hasError = true;
            assert(false);
            break;
        }
        hostNwritten += size;
    }

    nr_wasi_size_t nwritten = byteswap<nr_wasi_size_t>(hostNwritten);

    WASI_CHECK(runtime->setVMData(&nwritten, out_nwritten_offset,
                                  sizeof(nr_wasi_size_t)));
    return hasError ? 8 : 0;
}

WASI_FUNCTION_IMPL(uint32_t, path_filestat_get,
                   (uint32_t a, uint32_t b, uint32_t c, uint32_t d,
                    uint32_t e)) {
    assert(false);
    return 8;
}

WASI_FUNCTION_IMPL(uint32_t, path_open,
                   (uint32_t dirfd, uint32_t dirflags, uint32_t in_path_offset,
                    uint32_t path_len, uint32_t oflags, uint64_t fs_rights_base,
                    uint64_t fs_rights_inheriting, uint32_t fs_flags,
                    uint32_t out_fd_offset)) {

    std::vector<char> pathData;
    pathData.resize(path_len + 1);
    WASI_CHECK(runtime->getVMData(pathData.data(), in_path_offset, path_len));
    pathData[path_len] = '\0';

    std::string pathString = pathData.data();
    if (pathString.size() != path_len) {
        assert(false);
        return 28;
    }
    size_t outFd;
    if (!vfs.open(pathString, outFd, fs_flags && __WASI_FDFLAGS_APPEND)) {
        assert(false);
        return 8;
    }

    nr_wasi_fd_t fd = byteswap<nr_wasi_fd_t>(outFd);

    WASI_CHECK(runtime->setVMData(&fd, out_fd_offset, sizeof(nr_wasi_fd_t)))

    return 0;
}

WASI_FUNCTION_IMPL(uint32_t, path_remove_directory,
                   (uint32_t a, uint32_t b, uint32_t c)) {
    assert(false);
    return 8;
}
WASI_FUNCTION_IMPL(uint32_t, path_rename,
                   (uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                    uint32_t f)) {
    assert(false);
    return 8;
}
WASI_FUNCTION_IMPL(uint32_t, path_unlink_file,
                   (uint32_t a, uint32_t b, uint32_t c)) {
    assert(false);
    return 8;
}

} // namespace core
} // namespace webrogue
