addToLibrary({
    setImportFuncNames: function (jsonPtr) {
        let namesJson = UTF8ToString(jsonPtr);
        Module.importedFuncNames = JSON.parse(namesJson)
    },
    readModMem: function (modPtr, size, hostPtr) {
        Asyncify.handleSleep(wakeUp => {
            Module.gotMemorySlice = function (memorySlice) {
                HEAP8.set(new Int8Array(memorySlice), hostPtr);
                wakeUp();
            };
            Module.workerSharedArray[1] = BigInt(modPtr)
            Module.workerSharedArray[2] = BigInt(size)
            Atomics.store(Module.workerSharedArray, 0, BigInt(2));
            Atomics.notify(Module.workerSharedArray, 0);
        });
    },
    writeModMem: function (modPtr, size, hostPtr) {
        Asyncify.handleSleep(wakeUp => {
            let dataToWrite = HEAP8.slice(hostPtr, hostPtr + size);
            Module.gotMemorySlice = function (memorySlice) {
                new Int8Array(memorySlice).set(dataToWrite);
                Module.sliceWrote = wakeUp;
                Atomics.store(Module.workerSharedArray, 0, BigInt(3));
                Atomics.notify(Module.workerSharedArray, 0);
            };
            Module.workerSharedArray[1] = BigInt(modPtr)
            Module.workerSharedArray[2] = BigInt(size)
            Atomics.store(Module.workerSharedArray, 0, BigInt(2));
            Atomics.notify(Module.workerSharedArray, 0);
        });
    },
    execFunc: function (funcNamePtr) {
        Asyncify.handleSleep(wakeUp => {
            Module.modsExecFinished = wakeUp;
            let funcName = UTF8ToString(funcNamePtr);
            Module.modsWorker.postMessage(["exec", funcName]);
        });
    },
    isExecutionFinished: function () {
        return Module.executionFinished;
    },
    getImportedFuncId: function () {
        return Module.importedFuncId;
    },
    getArgInt32: function (argNum) {
        return Module.importedFuncArgs[argNum]
    },
    getArgInt64: function (argNum) {
        return Module.importedFuncArgs[argNum]
    },
    getArgUInt32: function (argNum) {
        return Module.importedFuncArgs[argNum]
    },
    getArgUInt64: function (argNum) {
        return Module.importedFuncArgs[argNum]
    },
    getArgFloat: function (argNum) {
        return Module.importedFuncArgs[argNum]
    },
    getArgDouble: function (argNum) {
        return Module.importedFuncArgs[argNum]
    },
    writeInt32Result: function (result) {
        Module.workerSharedArray[1] = BigInt(result)
    },
    writeUInt32Result: function (result) {
        Module.workerSharedArray[1] = BigInt(result)
    },
    writeInt64Result: function (result) {
        Module.workerSharedArray[1] = result
    },
    writeUInt64Result: function (result) {
        Module.workerSharedArray[1] = result
    },
    writeFloatResult: function (result) {
        let buffer = new ArrayBuffer(8);
        (new Float64Array(buffer))[0] = result;
        Module.workerSharedArray[1] = (new BigInt64Array(buffer))[0];
    },
    writeDoubleResult: function (result) {
        let buffer = new ArrayBuffer(8);
        (new Float64Array(buffer))[0] = result;
        Module.workerSharedArray[1] = (new BigInt64Array(buffer))[0];
    },
    continueFuncExecution: function () {
        Asyncify.handleSleep(wakeUp => {
            Module.modsExecFinished = wakeUp;
            Atomics.store(Module.workerSharedArray, 0, BigInt(1));
            Atomics.notify(Module.workerSharedArray, 0);
        });
    },
    initWasmModule: function (pointer, size) {
        Asyncify.handleSleep(wakeUp => {
            Module.workerSharedBuffer = new SharedArrayBuffer(256);
            Module.workerSharedArray = new BigInt64Array(Module.workerSharedBuffer);
            var modsWasmData = HEAPU8.subarray(pointer, pointer + size);

            Module.modsWorker.onmessage = function (message) {
                let command = message.data[0]
                if (command == 0) { // instantiated
                    wakeUp()
                } else if (command == 1) { // exec_finished
                    Module.executionFinished = true
                    {
                        let modsExecFinished = Module.modsExecFinished;
                        Module.modsExecFinished = undefined;
                        modsExecFinished();
                    }
                } else if (command == 2) { // exec_imported
                    Module.importedFuncId = message.data[1]
                    Module.importedFuncArgs = message.data[2]
                    Module.executionFinished = false
                    {
                        let modsExecFinished = Module.modsExecFinished;
                        Module.modsExecFinished = undefined;
                        modsExecFinished();
                    }
                } else if (command == 3) { // memory_slice
                    Module.gotMemorySlice(message.data[1])
                } else if (command == 4) { // memory_slice_wrote
                    Module.sliceWrote()
                } else {
                    console.error("host: unknown command ", command)
                }
            }
            Module.modsWorker.postMessage(["instantiate", modsWasmData, Module.importedFuncNames, Module.workerSharedBuffer])
        })
    }
});