#include "compact_linker.h"
#include "archive_reader.h"
#include "synthesizer.h"
#include "write_file.h"
#include <iostream>

vector<uint8_t> compact_link(vector<string> requiredFunctions,
                             vector<LinkableFile> files) {
    wabt::ArchiveReader archiveReader;

    for (auto requiredFunction : requiredFunctions)
        archiveReader.addRequiredFunction(requiredFunction.c_str());

    for (auto &file : files) {
        if (!wabt::succeeded(
                archiveReader.addFileData(&file.data, file.filename.c_str())))
            return {};
    }

    if (!wabt::succeeded(archiveReader.synthesize()))
        return {};
    wabt::Synthesizer synthesizer;
    synthesizer.stackSize = 5242880;
    wabt::WASMModule outputModule;
    if (!wabt::succeeded(synthesizer.synthesize(
            &archiveReader.modules, &archiveReader.symbols, &outputModule)))
        return {};
    wabt::MemoryStream stream;
    // // FileStream stream{};
    if (!wabt::succeeded(write_file(&outputModule, &stream)))
        return {};
    stream.Flush();
    return stream.output_buffer().data;
}