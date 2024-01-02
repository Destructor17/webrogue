#include "wamr_runtime.hpp"
#include "../../../external/wamr/core/iwasm/include/wasm_export.h"
#include "../../core/CompactLinking.hpp"
#include "../../core/ModsRuntime.hpp"
#include "wamr_templates.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>

#define _stringize(x) #x
#define stringize(x) _stringize(x)
namespace webrogue {
namespace runtimes {
namespace wamr {

WamrModsRuntime::WamrModsRuntime(
    webrogue::core::ConsoleStream *wrout, webrogue::core::ConsoleStream *wrerr,
    webrogue::core::ResourceStorage *resourceStorage,
    webrogue::core::Config *config)
    : ModsRuntime(wrout, wrerr, resourceStorage, config) {
}

NativeSymbol native_symbols[] = {
#define WR_API_FUNCTION(RET_TYPE, NAME, ARGS)                                  \
                                                                               \
    {stringize(NAME),                                                          \
     (void *)(wrap_helper<decltype(&core::ApiObject::NAME)>::wrapFn<           \
              &core::ApiObject::NAME>),                                        \
     wrap_helper<decltype(&core::ApiObject::NAME)>::getSignature()},
#include "../../../mods/core/include/common/wr_api_functions.def"
#undef WR_API_FUNCTION
};

NativeSymbol wasi_symbols[] = {
#define WASI_FUNCTION(RET_TYPE, NAME, ARGS)                                    \
                                                                               \
    {stringize(NAME),                                                          \
     (void *)(wrap_helper<decltype(&core::WASIObject::NAME)>::wrapFn<          \
              &core::WASIObject::NAME>),                                       \
     wrap_helper<decltype(&core::WASIObject::NAME)>::getSignature()},
#include "../../core/wasi_functions.def"
#undef WASI_FUNCTION
};

void WamrModsRuntime::initMods() {
    auto linked = core::getCompactlyLinkedBinnaries(
        this, resourceStorage, config,
        [this]() {
            interrupt();
        },
        wrout, wrerr);
    if (!linked || linked->size() == 0) {
        *wrerr << "linking failed\n";
        return;
    }
    *wrout << "initializind runtime for mods...\n";
    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
    init_args.fast_jit_code_cache_size = linked->size() * 8;
    if (wasm_runtime_is_running_mode_supported(Mode_Fast_JIT))
        init_args.running_mode = Mode_Fast_JIT;
    else
        init_args.running_mode = Mode_Interp;
    init_args.mem_alloc_type = Alloc_With_System_Allocator;
    if (!wasm_runtime_full_init(&init_args))
        return;
    initializedWasmRuntime = true;
    char error_buf[128];
    wasm_function_inst_t func;
    *wrout << "loading linked mods...\n";

    if (!wasm_runtime_register_natives("wasi_snapshot_preview1", wasi_symbols,
                                       sizeof(wasi_symbols) /
                                           sizeof(NativeSymbol))) {
        return;
    }
    if (!wasm_runtime_register_natives("webrogue", native_symbols,
                                       sizeof(native_symbols) /
                                           sizeof(NativeSymbol))) {
        return;
    }
    module = wasm_runtime_load(linked->data(), linked->size(), error_buf,
                               sizeof(error_buf));
    if (!module) {
        *wrerr << "Error while loading linked module: " << error_buf << "\n";
        return;
    }
    moduleInst = wasm_runtime_instantiate(module, stackSize, heapSize,
                                          error_buf, sizeof(error_buf));
    if (!moduleInst) {
        *wrerr << error_buf;
        return;
    }
    execEnv = wasm_runtime_create_exec_env(moduleInst, stackSize);
    wasm_runtime_set_user_data(execEnv, this);
    *wrout << "initializing mods...\n";
    func = wasm_runtime_lookup_function(moduleInst, "__wasm_call_ctors", "()");
    if (!wasm_runtime_call_wasm_v(execEnv, func, 0, nullptr, 0)) {
        return;
    }
    for (std::string modName : resourceStorage->modNames) {
        std::string funcName = "init_mod_" + modName;
        func = wasm_runtime_lookup_function(moduleInst, funcName.c_str(), "()");
        if (!wasm_runtime_call_wasm_v(execEnv, func, 0, nullptr, 0)) {
            return;
        }
    }
    *wrout << "all mods initialized\n";
    startFunc = wasm_runtime_lookup_function(moduleInst, "nr_start", "()");

    isInitialized = true;
};
void WamrModsRuntime::start() {
    if (!isInitialized)
        return;
    bool ret = wasm_runtime_call_wasm_v(execEnv, startFunc, 0, nullptr, 0);
    if (!ret) {
        *wrerr << "Error while executing nr_start: "
               << wasm_runtime_get_exception(moduleInst) << "\n";
        wasm_runtime_clear_exception(moduleInst);
        isInitialized = false;
    }
};

bool WamrModsRuntime::getVMData(void *outPtr, uint64_t offset, int32_t size) {
    uint32_t currentMemSize;
    uint8_t *currentMem =
        (uint8_t *)wasm_runtime_addr_app_to_native(moduleInst, 0);

    wasm_runtime_get_app_addr_range(moduleInst, 0, nullptr, &currentMemSize);
    bool memOk = offset >= 0 && (offset + size) <= currentMemSize;
    if (!memOk)
        return false;
    memcpy(outPtr, currentMem + offset, size);
    return true;
}
bool WamrModsRuntime::setVMData(const void *inPtr, uint64_t offset,
                                int32_t size) {
    uint32_t currentMemSize;
    uint8_t *currentMem =
        (uint8_t *)wasm_runtime_addr_app_to_native(moduleInst, 0);

    wasm_runtime_get_app_addr_range(moduleInst, 0, nullptr, &currentMemSize);
    bool memOk = offset >= 0 && (offset + size) <= currentMemSize;
    if (!memOk)
        return false;
    memcpy(currentMem + offset, inPtr, size);
    return true;
}

size_t WamrModsRuntime::voidptrSize() {
    return 4;
};

WamrModsRuntime::~WamrModsRuntime() {
    if (execEnv)
        wasm_runtime_destroy_exec_env(execEnv);
    if (moduleInst)
        wasm_runtime_deinstantiate(moduleInst);
    if (module)
        wasm_runtime_unload(module);
    if (initializedWasmRuntime)
        wasm_runtime_destroy();
}
} // namespace wamr
} // namespace runtimes
} // namespace webrogue

namespace webrogue {
namespace runtimes {
std::shared_ptr<webrogue::core::ModsRuntime> MAKE_DEFAULT_RUNTIME_EXPORT
makeDefaultRuntime(webrogue::core::ConsoleStream *wrout,
                   webrogue::core::ConsoleStream *wrerr,
                   webrogue::core::ResourceStorage *resourceStorage,
                   webrogue::core::Config *config) {
    return std::make_shared<wamr::WamrModsRuntime>(wrout, wrerr,
                                                   resourceStorage, config);
}
} // namespace runtimes
} // namespace webrogue
