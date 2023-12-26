#include "ModsRuntime.hpp"

#include "ApiObject.hpp"
#include "WASIObject.hpp"
namespace webrogue {
namespace core {
ModsRuntime::ModsRuntime(ConsoleStream *wrout, ConsoleStream *wrerr,
                         ResourceStorage *resourceStorage, Config *config)
    : wrout(wrout), wrerr(wrerr), resourceStorage(resourceStorage),
      config(config), apiObject(this, config),
      wasiObject(this, resourceStorage, config) {
}

ModsRuntime::~ModsRuntime() {
}

void ModsRuntime::onFrameEnd() {
    wasiObject.vfs.commit();
    config->onFrameEnd();
}
} // namespace core
} // namespace webrogue
