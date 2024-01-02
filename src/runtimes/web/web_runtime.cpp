#include "web_runtime.hpp"

#include "../../core/ApiObject.hpp"
#include "../../core/CompactLinking.hpp"
#include "imported_func_wrapper.hpp"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <unistd.h>

#define _stringize(x) #x
#define stringize(x) _stringize(x)

namespace webrogue {
namespace runtimes {
namespace web {

WebModsRuntime::WebModsRuntime(webrogue::core::ConsoleStream *wrout,
                               webrogue::core::ConsoleStream *wrerr,
                               webrogue::core::ResourceStorage *resourceStorage,
                               webrogue::core::Config *config)
    : ModsRuntime(wrout, wrerr, resourceStorage, config){};

extern "C" {
extern void readModMem(uint32_t modPtr, uint32_t size, void *hostPtr);
extern void writeModMem(uint32_t modPtr, uint32_t size, const void *hostPtr);
extern void initWasmModule(const uint8_t *mys, int siz);
extern void execFunc(const char *funcName);
extern void continueFuncExecution();
extern bool isExecutionFinished();
extern int getImportedFuncId();
}

void WebModsRuntime::initMods() {
    linkedWasm = getCompactlyLinkedBinnaries(
        this, resourceStorage, config,
        [this]() {
            interrupt();
        },
        wrout, wrerr);
    if (!linkedWasm) {
        *wrerr << "linking failed\n";
        return;
    }

    initWrapper(&apiObject, &wasiObject);

    initWasmModule(linkedWasm->data(), linkedWasm->size());

    execAsyncFunc("__wasm_call_ctors");

    for (auto modName : resourceStorage->modNames) {
        execAsyncFunc("init_mod_" + modName);
    }

    isInitialized = true;
}

void WebModsRuntime::execAsyncFunc(std::string funcName) {

    execFunc(funcName.c_str());
    while (!isExecutionFinished()) {
        callImportedFunc(getImportedFuncId());
        continueFuncExecution();
    }
}

void WebModsRuntime::start() {
    execAsyncFunc("nr_start");
}
bool WebModsRuntime::getVMData(void *outPtr, uint64_t offset, int32_t size) {
    readModMem(offset, size, outPtr);
    return true;
}
bool WebModsRuntime::setVMData(const void *inPtr, uint64_t offset,
                               int32_t size) {
    writeModMem(offset, size, inPtr);
    return true;
}

size_t WebModsRuntime::voidptrSize() {
    return 4;
};
WebModsRuntime::~WebModsRuntime() {
}
} // namespace web
} // namespace runtimes
} // namespace webrogue

namespace webrogue {
namespace runtimes {
std::shared_ptr<webrogue::core::ModsRuntime> MAKE_DEFAULT_RUNTIME_EXPORT
makeDefaultRuntime(webrogue::core::ConsoleStream *wrout,
                   webrogue::core::ConsoleStream *wrerr,
                   webrogue::core::ResourceStorage *resourceStorage,
                   webrogue::core::Config *config) {
    return std::make_shared<web::WebModsRuntime>(wrout, wrerr, resourceStorage,
                                                 config);
}
} // namespace runtimes
} // namespace webrogue

#ifndef __EMSCRIPTEN__
extern "C" {
extern void readModMem(uint32_t modPtr, uint32_t size, void *hostPtr) {
}
extern void writeModMem(uint32_t modPtr, uint32_t size, const void *hostPtr) {
}
extern void initWasmModule(const uint8_t *mys, int siz) {
}
extern void execFunc(const char *funcName) {
}
extern bool isExecutionFinished() {
    return false;
}
extern int getImportedFuncId(){};

extern int32_t getArgInt32(uint32_t argNum) {
}
extern uint32_t getArgUInt32(uint32_t argNum) {
}
extern int64_t getArgInt64(uint32_t argNum) {
    if (argNum == 1)
        return 2;
}
extern uint64_t getArgUInt64(uint32_t argNum) {
    if (argNum == 0)
        return 1;
}
extern float getArgFloat(uint32_t argNum) {
}
extern double getArgDouble(uint32_t argNum) {
}
extern void writeInt32Result(int32_t result) {
}
extern void writeUInt32Result(uint32_t result) {
}
extern void writeInt64Result(int64_t result) {
}
extern void writeUInt64Result(uint64_t result) {
}
extern void writeFloatResult(float result) {
}
extern void writeDoubleResult(double result) {
}
extern void continueFuncExecution() {
}
extern void setImportFuncNames(const char *jsonPtr) {
    std::cout << jsonPtr << "\n";
}
}
#endif
