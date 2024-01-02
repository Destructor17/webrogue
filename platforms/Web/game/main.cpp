#include "../../../src/core/webrogueMain.hpp"
#include "../../../src/outputs/sdl/SDLOutput.hpp"
#include <cstdint>
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

void draw() {
}

bool fetched;
webrogue::core::Config *sharedConfig;
std::string fetchedModName;

std::list<std::vector<uint8_t>> downloadedMods;

void downloadSucceeded(emscripten_fetch_t *fetch) {
    downloadedMods.push_back(
        {(const uint8_t *)(fetch->data),
         (const uint8_t *)(fetch->data + fetch->numBytes)});

    sharedConfig->addWrmodData(downloadedMods.rbegin()->data(),
                               downloadedMods.rbegin()->size(), fetchedModName);
    fetched = true;
    emscripten_fetch_close(fetch);
}

void downloadFailed(emscripten_fetch_t *fetch) {
    printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url,
           fetch->status);
    emscripten_fetch_close(fetch); // Also free data on failure.
}

int main(int argc, char *argv[]) {
    webrogue::core::Config config;
    sharedConfig = &config;
    emscripten_set_main_loop(draw, 0, 0);
    config.loadsModsFromDataPath = false;
    config.setDataPath("/webrogue");

    config.onFrameEnd = []() {
        EM_ASM(FS.syncfs(function(err){}););
    };

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;

    fetched = false;
    fetchedModName = "core";
    emscripten_fetch(&attr, "mods/core.wrmod");
    while (!fetched)
        emscripten_sleep(10);

    fetched = false;
    fetchedModName = "basecontent";
    emscripten_fetch(&attr, "mods/basecontent.wrmod");
    while (!fetched)
        emscripten_sleep(10);

    return webrogue::core::webrogueMain(
        std::make_shared<webrogue::outputs::sdl::SDLOutput>(),
        webrogue::runtimes::makeDefaultRuntime, &config);
}
