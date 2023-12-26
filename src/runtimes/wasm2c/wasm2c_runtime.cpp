#include "wasm2c_runtime.hpp"
#include "../../core/ModsRuntime.hpp"
#include <cstdint>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

namespace webrogue {
namespace runtimes {
namespace wasm2c {
Wasm2cModsRuntime::Wasm2cModsRuntime(
    webrogue::core::ConsoleStream *wrout, webrogue::core::ConsoleStream *wrerr,
    webrogue::core::ResourceStorage *resourceStorage,
    webrogue::core::Config *config)
    : ModsRuntime(wrout, wrerr, resourceStorage, config) {
}
void Wasm2cModsRuntime::initMods() {
    wasm_rt_init();
    wasm2c_linked_instantiate(&linked, (w2c_wasi__snapshot__preview1 *)this,
                              (w2c_webrogue *)this);
    w2c_linked_0x5F_wasm_call_ctors(&linked);
#define mod_to_embed(name) w2c_linked_init_mod_##name(&linked);
#include stringize(WASM2C_MOD_LIST_HEADER)
#undef mod_to_embed
    isInitialized = true;
};
void Wasm2cModsRuntime::start() {
    w2c_linked_nr_start(&linked);
};
bool Wasm2cModsRuntime::getVMData(void *outPtr, uint64_t offset, int32_t size) {
    if (offset < 0 || offset + size > linked.w2c_memory.size)
        return false;
    memcpy(outPtr, linked.w2c_memory.data + offset, size);
    return true;
}
bool Wasm2cModsRuntime::setVMData(const void *inPtr, uint64_t offset,
                                  int32_t size) {
    if (offset < 0 || offset + size > linked.w2c_memory.size)
        return false;
    memcpy(linked.w2c_memory.data + offset, inPtr, size);
    return true;
}
size_t Wasm2cModsRuntime::voidptrSize() {
    return 4;
};
} // namespace wasm2c
} // namespace runtimes
} // namespace webrogue

namespace webrogue {
namespace runtimes {

std::shared_ptr<webrogue::core::ModsRuntime> MAKE_DEFAULT_RUNTIME_EXPORT
makeDefaultRuntime(webrogue::core::ConsoleStream *wrout,
                   webrogue::core::ConsoleStream *wrerr,
                   webrogue::core::ResourceStorage *resourceStorage,
                   webrogue::core::Config *config) {
    return std::make_shared<wasm2c::Wasm2cModsRuntime>(wrout, wrerr,
                                                       resourceStorage, config);
}
} // namespace runtimes
} // namespace webrogue
