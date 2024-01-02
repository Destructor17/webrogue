#include "webrogueMain.hpp"
#include "Config.hpp"
#include "ModsRuntime.hpp"
#include "ResourceStorage.hpp"
#include "sys/stat.h"
#include <dirent.h>
#include <fstream>
#include <memory>

namespace webrogue {
namespace core {
bool loadMods(ResourceStorage *mockFS, Config *config, ConsoleStream *wrout,
              ConsoleStream *wrerr) {
    for (auto modData : config->mods) {
        if (!mockFS->loadWrmodData(modData.data, modData.size, modData.name))
            return false;
    }
    if (config->loadsModsFromDataPath && config->hasDataPath) {
        static const std::string curDir = ".";
        static const std::string parDir = "..";
        DIR *dir;
        struct dirent *drnt;
        std::string modsPath = (config->dataPath + "/mods");
        dir = opendir(modsPath.c_str());
        if (!dir) {
            *wrerr << "unable to open directory \"" << modsPath << "\"\n";
            return false;
        }
        while ((drnt = readdir(dir)) != NULL) {
            std::string name(drnt->d_name);
            if (name != curDir && name != parDir && name != "core") {
                struct stat s;
                if (stat((config->dataPath + "/mods/" + name).c_str(), &s) !=
                    0) {
                    *wrerr << "stat \"" << config->dataPath + "/mods/" + name
                           << "\" failed!\n";
                    return false;
                }
                bool isDir = s.st_mode & S_IFDIR;
                std::string newPath = (config->dataPath + "/mods") + "/" + name;

                if (isDir) {
                    if (!mockFS->loadDir(newPath, name))
                        return false;
                } else {
                    const std::string wrmodExtension = ".wrmod";
                    if (name.size() > wrmodExtension.size() &&
                        std::equal(wrmodExtension.rbegin(),
                                   wrmodExtension.rend(), name.rbegin())) {
                        if (!mockFS->loadWrmodFile(
                                newPath,
                                name.substr(0, name.size() -
                                                   wrmodExtension.size())))
                            return false;
                    }
                }
            }
        }
        closedir(dir);

        *wrout << "all mods loaded\n";
    }
    return true;
}

int webrogueMain(std::shared_ptr<Output> output, runtime_maker_t runtimeMaker,
                 Config *config) {
    ConsoleWriter consoleWriter(output);
    ConsoleStream wrout(&consoleWriter, false);
    wrout.tie(nullptr);
    ConsoleStream wrerr(&consoleWriter, true);
    wrerr.tie(nullptr);
    ResourceStorage mockFS(&wrout, &wrerr);
    std::shared_ptr<ModsRuntime> runtime(
        runtimeMaker(&wrout, &wrerr, &mockFS, config));
    runtime->apiObject.output = output.get();
    runtime->apiObject.consoleWriter = &consoleWriter;
    output->begin();
    mockFS.interrupt = [&runtime]() {
        runtime->interrupt();
    };
    bool hasLoadingError = !loadMods(&mockFS, config, &wrout, &wrerr);
    if (!hasLoadingError) {
        std::string dbpath;
        if (config->hasDataPath) {
            wrout << "opening database...\n";
            dbpath = config->dataPath + "/webrogue.db";
        } else {
            wrout << "opening in-memory database...\n";
            dbpath = "memory";
        }
        runtime->apiObject.db = std::make_unique<DB>(dbpath);
        runtime->initMods();
        hasLoadingError |= !runtime->isInitialized;
    }

    while (true) {
        auto event = output->getEvent();
        if (event.type == webrogue_event_type::None)
            break;
    }
    consoleWriter.isShown = hasLoadingError;
    if (!hasLoadingError) {
        output->beginFrame();
        runtime->start();
        output->endFrame();
        config->onFrameEnd();
        output->end();
        return 0;
    }
    while (consoleWriter.present().type != webrogue_event_type::Close)
        consoleWriter.isShown = true;
    return 1;
}
} // namespace core
} // namespace webrogue
