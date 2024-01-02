# docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk 
emcmake cmake -Bplatforms/Web/game/build -Splatforms/Web/game/ -DCMAKE_BUILD_TYPE=Debug || exit 1
cmake --build platforms/Web/game/build/ --target pack_artifacts -j || exit 2
python3 scripts/srv.py
# cd artifacts || exit 3
# python3 -m http.server
