# docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk 

cd $(dirname $0)

set -ex

BUILD_TYPE=$1

emcmake cmake -B game/build -S game/ -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build game/build/ --target pack_artifacts -j

rm -rf game_artifacts
mv ../../artifacts game_artifacts

emcmake cmake -B homepage/build_wasm -S homepage/ -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DWEBROGUE_ASMJS=0
cmake --build homepage/build_wasm/ --target webrogue_homepage_wasm -j

emcmake cmake -B homepage/build_asmjs -S homepage/ -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DWEBROGUE_ASMJS=1
cmake --build homepage/build_asmjs/ --target webrogue_homepage_asmjs -j

mkdir ../../artifacts

mv game_artifacts ../../artifacts/game


cp -r homepage/index.html homepage/noscript ../../artifacts
cp homepage/build_asmjs/webrogue_homepage_asmjs.js homepage/build_asmjs/webrogue_homepage_asmjs.js.mem ../../artifacts
cp homepage/build_wasm/webrogue_homepage_wasm.js homepage/build_wasm/webrogue_homepage_wasm.wasm ../../artifacts

# cd artifacts
# python3 -m http.server
