powershell.exe -noprofile -c "rm -r -fo .\artifacts\; exit 0"
mkdir .\artifacts\
$BuildConfig = $args[0]
echo "Build config: $BuildConfig"
cmake -S platforms/Windows/ -B platforms/Windows/build -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL
cmake --build platforms/Windows/build -t pack_artifacts --config $BuildConfig --parallel
mkdir .\artifacts\mods
cp .\mods\core\core.wrmod .\artifacts\mods\
cp .\mods\basecontent\basecontent.wrmod .\artifacts\mods\
