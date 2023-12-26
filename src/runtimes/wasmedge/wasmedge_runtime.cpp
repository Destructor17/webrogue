#include "wasmedge_runtime.hpp"
#include "../../core/CompactLinking.hpp"

#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "wasmedge/wasmedge.h"

#include "../../core/ApiObject.hpp"
#include "../../core/ConsoleStream.hpp"
#include "wasmedge_templates.hpp"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>

#define _stringize(x) #x
#define stringize(x) _stringize(x)

namespace webrogue {
namespace runtimes {
namespace wasmedge {
bool readRealFile(std::vector<uint8_t> &out, std::string path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
        return false;
    file.unsetf(std::ios::skipws);
    file.seekg(0, std::ios_base::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios_base::beg);
    out.resize(0);
    out.reserve(fileSize);
    out.insert(out.begin(), std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());
    return true;
}

WasmEdgeModsRuntime::WasmEdgeModsRuntime(
    webrogue::core::ConsoleStream *wrout, webrogue::core::ConsoleStream *wrerr,
    webrogue::core::ResourceStorage *resourceStorage,
    webrogue::core::Config *config)
    : ModsRuntime(wrout, wrerr, resourceStorage, config) {
}

void WasmEdgeModsRuntime::initMods() {
    spdlog::set_level(spdlog::level::err);
    auto linkResult = core::get_compactly_linked_binnaries(
        this, resourceStorage, config, wrout, wrerr);
    WasmEdge_Result res;
    confCxt = WasmEdge_ConfigureCreate();
    std::string aotCachePath = config->dataPath + "/wasmedge_aot_cache";
    if (true) {
        *wrout << "precompiling...\n";
        WasmEdge_CompilerContext *compCtx = WasmEdge_CompilerCreate(confCxt);
        res = WasmEdge_CompilerCompileFromBuffer(compCtx, linkResult->data(),
                                                 linkResult->size(),
                                                 aotCachePath.c_str());
        if (!WasmEdge_ResultOK(res)) {
            assert(false);
            return;
        }
        WasmEdge_CompilerDelete(compCtx);
    }
    vmCxt = WasmEdge_VMCreate(confCxt, NULL);
    std::vector<uint8_t> wasm;

    res = WasmEdge_VMLoadWasmFromFile(vmCxt, aotCachePath.c_str());
    if (!WasmEdge_ResultOK(res)) {
        assert(false);
        return;
    }

    res = WasmEdge_VMValidate(vmCxt);
    if (!WasmEdge_ResultOK(res)) {
        assert(false);
        return;
    }
    { // nr_api
        WasmEdge_String hostName = WasmEdge_StringCreateByCString("webrogue");
        WasmEdge_ModuleInstanceContext *HostMod =
            WasmEdge_ModuleInstanceCreate(hostName);
        WasmEdge_StringDelete(hostName);

#define NR_API_FUNCTION(RET_TYPE, NAME, ARGS)                                  \
    link_helper<decltype(&core::ApiObject::NAME),                              \
                &core::ApiObject::NAME>::linkWasmedgeFunction("webrogue",      \
                                                              stringize(NAME), \
                                                              HostMod, this);
#include "../../../../mods/core/include/common/nr_api.h"

        WasmEdge_StoreContext *storeCxt = WasmEdge_StoreCreate();
        WasmEdge_ExecutorContext *execCxt = WasmEdge_ExecutorCreate(NULL, NULL);
        res = WasmEdge_ExecutorRegisterImport(execCxt, storeCxt, HostMod);
        if (!WasmEdge_ResultOK(res)) {
            assert(false);
            return;
        }

        res = WasmEdge_VMRegisterModuleFromImport(vmCxt, HostMod);
        if (!WasmEdge_ResultOK(res)) {
            assert(false);
            return;
        }

        WasmEdge_StoreDelete(storeCxt);   // ?
        WasmEdge_ExecutorDelete(execCxt); // ?
        // WasmEdge_ModuleInstanceDelete(HostMod);
    }
    { // wasi_api
        WasmEdge_String hostName =
            WasmEdge_StringCreateByCString("wasi_snapshot_preview1");
        WasmEdge_ModuleInstanceContext *HostMod =
            WasmEdge_ModuleInstanceCreate(hostName);
        WasmEdge_StringDelete(hostName);

#define NR_API_FUNCTION(RET_TYPE, NAME, ARGS)                                  \
    link_helper<decltype(&core::WASIObject::NAME), &core::WASIObject::NAME>::  \
        linkWasmedgeFunction("wasi_snapshot_preview1", stringize(NAME),        \
                             HostMod, this);
#include "../../core/wasi_functions.h"

        WasmEdge_StoreContext *storeCxt = WasmEdge_StoreCreate();
        WasmEdge_ExecutorContext *execCxt = WasmEdge_ExecutorCreate(NULL, NULL);
        res = WasmEdge_ExecutorRegisterImport(execCxt, storeCxt, HostMod);
        if (!WasmEdge_ResultOK(res)) {
            assert(false);
            return;
        }

        res = WasmEdge_VMRegisterModuleFromImport(vmCxt, HostMod);
        if (!WasmEdge_ResultOK(res)) {
            assert(false);
            return;
        }

        WasmEdge_StoreDelete(storeCxt);   // ?
        WasmEdge_ExecutorDelete(execCxt); // ?
        // WasmEdge_ModuleInstanceDelete(HostMod);
    }

    res = WasmEdge_VMInstantiate(vmCxt);
    if (!WasmEdge_ResultOK(res)) {
        assert(false);
        return;
    }

    *wrout << "initializing mods...\n";
    WasmEdge_String funcName =
        WasmEdge_StringCreateByCString("__wasm_call_ctors");
    res = WasmEdge_VMExecute(vmCxt, funcName, nullptr, 0, nullptr, 0);
    WasmEdge_StringDelete(funcName);
    if (!WasmEdge_ResultOK(res)) {
        assert(false);
        return;
    }

    for (std::string modName : resourceStorage->modNames) {
        std::string stdFuncName = "init_mod_" + modName;
        funcName = WasmEdge_StringCreateByCString(stdFuncName.c_str());

        res = WasmEdge_VMExecute(vmCxt, funcName, nullptr, 0, nullptr, 0);
        WasmEdge_StringDelete(funcName);
        if (!WasmEdge_ResultOK(res)) {
            assert(false);
            return;
        }
    }
    *wrout << "all mods initialized\n";

    isInitialized = true;
}
void WasmEdgeModsRuntime::start() {
    WasmEdge_String funcName = WasmEdge_StringCreateByCString("nr_start");

    WasmEdge_Result res =
        WasmEdge_VMExecute(vmCxt, funcName, nullptr, 0, nullptr, 0);
    WasmEdge_StringDelete(funcName);
    if (!WasmEdge_ResultOK(res)) {
        assert(false);
        return;
    }
}

bool WasmEdgeModsRuntime::getVMData(void *outPtr, uint64_t offset,
                                    int32_t size) {
    const WasmEdge_CallingFrameContext *callFrameCxt =
        (const WasmEdge_CallingFrameContext *)vmContext;
    WasmEdge_MemoryInstanceContext *memCxt =
        WasmEdge_CallingFrameGetMemoryInstance(callFrameCxt, 0);
    WasmEdge_Result res = WasmEdge_MemoryInstanceGetData(
        memCxt, (uint8_t *)(outPtr), offset, size);
    return WasmEdge_ResultOK(res);
}
bool WasmEdgeModsRuntime::setVMData(const void *inPtr, uint64_t offset,
                                    int32_t size) {
    const WasmEdge_CallingFrameContext *callFrameCxt =
        (const WasmEdge_CallingFrameContext *)vmContext;
    WasmEdge_MemoryInstanceContext *memCxt =
        WasmEdge_CallingFrameGetMemoryInstance(callFrameCxt, 0);
    WasmEdge_Result res = WasmEdge_MemoryInstanceSetData(
        memCxt, (uint8_t *)(inPtr), offset, size);
    return WasmEdge_ResultOK(res);
}

size_t WasmEdgeModsRuntime::voidptrSize() {
    return 4;
};

WasmEdgeModsRuntime::~WasmEdgeModsRuntime() {
    WasmEdge_VMDelete(vmCxt);
    WasmEdge_ConfigureDelete(confCxt);
}
} // namespace wasmedge
} // namespace runtimes
} // namespace webrogue

namespace webrogue {
namespace runtimes {
std::shared_ptr<webrogue::core::ModsRuntime> MAKE_DEFAULT_RUNTIME_EXPORT
makeDefaultRuntime(webrogue::core::ConsoleStream *wrout,
                   webrogue::core::ConsoleStream *wrerr,
                   webrogue::core::ResourceStorage *resourceStorage,
                   webrogue::core::Config *config) {
    auto result = std::make_shared<wasmedge::WasmEdgeModsRuntime>(
        wrout, wrerr, resourceStorage, config);
    return result;
}
} // namespace runtimes
} // namespace webrogue
