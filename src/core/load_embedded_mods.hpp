#include "Config.hpp"

#if !defined(WEBROGUE_EMBEDDED_MODS)
#error No mods to embed
#endif

#define _stringize(x) #x
#define stringize(x) _stringize(x)

#include stringize(MOD_ZIP_LIST_HEADER)

void inline load_embedded_mods(webrogue::core::Config *config) {

#define mod_to_embed(name)                                                     \
    config->addWrmodData(name##_ewrmod, name##_ewrmod_size, stringize(name));
#include stringize(MOD_LIST_HEADER)
#undef mod_to_embed
}