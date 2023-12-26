#pragma once
#include "Config.hpp"
#include "ResourceStorage.hpp"
#include "VFS.hpp"
#include <cstdint>

namespace webrogue {
namespace core {
class ModsRuntime;

class WASIObject {
public:
    VFS vfs;
    ModsRuntime *runtime;
    WASIObject(ModsRuntime *pRuntime, ResourceStorage *resourceStorage,
               Config *config);

#define WASI_FUNCTION(RET_TYPE, NAME, ARGS) RET_TYPE NAME ARGS;
#include "wasi_functions.def"
#undef WASI_FUNCTION
};
} // namespace core
} // namespace webrogue
