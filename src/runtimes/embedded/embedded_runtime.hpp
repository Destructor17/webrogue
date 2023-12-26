#pragma once

#include "../../core/ModsRuntime.hpp"

namespace webrogue {
namespace runtimes {
namespace embedded {
class EmbeddedModsRuntime : public webrogue::core::ModsRuntime {
public:
    EmbeddedModsRuntime(webrogue::core::ConsoleStream *wrout,
                        webrogue::core::ConsoleStream *wrerr,
                        webrogue::core::ResourceStorage *resourceStorage,
                        webrogue::core::Config *config);
    void initMods() override;
    void start() override;
    bool getVMData(void *outPtr, uint64_t offset, int32_t size) override;
    bool setVMData(const void *inPtr, uint64_t offset, int32_t size) override;
    size_t voidptrSize() override;
};
} // namespace embedded
} // namespace runtimes
} // namespace webrogue
