#include "archive_reader.h"
#include "stream.h"
#include "synthesizer.h"
#include "write_file.h"

#include <cstdlib>
#include <string>

#define xstr(s) str(s)
#define str(s) #s

using namespace wabt;
Result run() {
    // system("pwd");
    ArchiveReader archive_reader;
    archive_reader.addRequiredFunction("init_mod_basecontent");
    archive_reader.addRequiredFunction("init_mod_core");
    archive_reader.addRequiredFunction("nr_start");
    system("rm -f actual");
    CHECK_RESULT(archive_reader.addFile("mods/basecontent/mod.a"));
    CHECK_RESULT(archive_reader.addFile("mods/core/mod.a"));
    CHECK_RESULT(archive_reader.addFile("mods/core/stdlibs.a"));
    archive_reader.synthesize();
    Synthesizer synthesizer;
    WASMModule output_module;
    auto symbols = archive_reader.symbols;
    CHECK_RESULT(synthesizer.synthesize(
        &archive_reader.modules, &archive_reader.symbols, &output_module));
    MemoryStream stream;
    // // FileStream stream{};
    CHECK_RESULT(write_file(&output_module, &stream));
    stream.Flush();
    stream.WriteToFile("src/linker/test_src/t1_my.wasm");

    string objdump_command = std::string(xstr(WASM_OBJDUMP_PATH)) +
                             " -x -r -d src/linker/test_src/t1_my.wasm "
                             ">src/linker/test_src/t1_my.dump";
    system(objdump_command.c_str());
    return Result::Ok;
}

int main(int argc, char *argv[]) {
    if (failed(run())) {
        return 1;
    }
    return 0;
}