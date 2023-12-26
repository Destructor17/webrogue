#include "embedded_runtime.hpp"

#include "../../core/ApiObject.hpp"
#include "../../core/ConsoleStream.hpp"
#include "shared_api_object.hpp"
#include <cstring>
#include <vector>

extern "C" void nr_start();

#define WR_API_FUNCTION(RET_TYPE, NAME, ARGS) extern "C" RET_TYPE NAME ARGS;
#include "../../../mods/core/include/common/wr_api_functions.def"
#undef WR_API_FUNCTION

#define _stringize(x) #x
#define stringize(x) _stringize(x)

#define mod_to_embed(name) extern "C" void init_mod_##name();
#include stringize(MOD_LIST_HEADER)
#undef mod_to_embed

namespace webrogue {
namespace runtimes {
namespace embedded {

EmbeddedModsRuntime::EmbeddedModsRuntime(
    webrogue::core::ConsoleStream *wrout, webrogue::core::ConsoleStream *wrerr,
    webrogue::core::ResourceStorage *resourceStorage,
    webrogue::core::Config *config)
    : ModsRuntime(wrout, wrerr, resourceStorage, config) {
}

void EmbeddedModsRuntime::initMods() {
    // TODO remove
    //  needed to mark api functions as used to prevent strange linking bug
    std::vector<void *> api_functions;
    {
#define WR_API_FUNCTION(RET_TYPE, NAME, ARGS)                                  \
    api_functions.push_back((void *)&NAME);
#include "../../../mods/core/include/common/wr_api_functions.def"
#undef WR_API_FUNCTION
    };
    *wrout << "initialization started\n";
#define mod_to_embed(name) init_mod_##name();
#include stringize(MOD_LIST_HEADER)
#undef mod_to_embed
    *wrout << "initialization finished\n";
    isInitialized = true;
}
void EmbeddedModsRuntime::start() {
    nr_start();
}

bool EmbeddedModsRuntime::getVMData(void *outPtr, uint64_t offset,
                                    int32_t size) {
    memcpy(outPtr, (void *)offset, size);
    return true;
}
bool EmbeddedModsRuntime::setVMData(const void *inPtr, uint64_t offset,
                                    int32_t size) {

    memcpy((void *)offset, inPtr, size);
    return true;
}

size_t EmbeddedModsRuntime::voidptrSize() {
    return sizeof(void *);
};
} // namespace embedded
} // namespace runtimes
} // namespace webrogue

namespace webrogue {
namespace runtimes {
std::shared_ptr<webrogue::core::ModsRuntime> MAKE_DEFAULT_RUNTIME_EXPORT
makeDefaultRuntime(webrogue::core::ConsoleStream *wrout,
                   webrogue::core::ConsoleStream *wrerr,
                   webrogue::core::ResourceStorage *resourceStorage,
                   webrogue::core::Config *config) {
    auto result = std::make_shared<embedded::EmbeddedModsRuntime>(
        wrout, wrerr, resourceStorage, config);
    webrogue::runtimes::embedded::sharedApiObject = &result->apiObject;
    return result;
}
} // namespace runtimes
} // namespace webrogue
