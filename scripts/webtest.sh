# docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk 

rm -rf game_artifacts

emcmake cmake -Bplatforms/Web/game/build -Splatforms/Web/game/ -DCMAKE_BUILD_TYPE=Release || exit 1
cmake --build platforms/Web/game/build/ --target pack_artifacts -j || exit 2

mv artifacts game_artifacts || exit 2
mv game_artifacts/index.html game_artifacts/game.html || exit 2

emcmake cmake -Bplatforms/Web/homepage/build -Splatforms/Web/homepage/ -DCMAKE_BUILD_TYPE=Release || exit 2
cmake --build platforms/Web/homepage/build/ --target pack_artifacts -j || exit 2

mv game_artifacts/* artifacts || exit 2
rmdir game_artifacts || exit 2


python3 scripts/srv.py
# cd artifacts || exit 3
# python3 -m http.server
