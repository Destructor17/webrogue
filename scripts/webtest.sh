# docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk 
emcmake cmake -Bplatforms/Web/homepage/build -Splatforms/Web/homepage/ -DCMAKE_BUILD_TYPE=Release || exit 1
cmake --build platforms/Web/homepage/build/ --target pack_artifacts -j || exit 2
cd artifacts || exit 3
python3 -m http.server
