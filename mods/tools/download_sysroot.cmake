file(DOWNLOAD
   	"https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/wasi-sysroot-20.0.tar.gz"
    ${CMAKE_CURRENT_LIST_DIR}/wasi-sysroot.tar.gz
	EXPECTED_HASH SHA512=ad4ad629d02f01f3d2eb977dd0bc43091b0f11ed1b5dd9fdb3580e4cf49c132f6cb4982ae80eabf638f0d08d0c4c7df40cceb2be8f9d2c29abc35b8564ffda42
	SHOW_PROGRESS
)
file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_LIST_DIR}/wasi-sysroot.tar.gz DESTINATION ${CMAKE_CURRENT_LIST_DIR})
