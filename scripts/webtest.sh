# docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk 

set -ex

sh platforms/Web/build_and_pack.sh Release

python3 scripts/srv.py
# cd artifacts
# python3 -m http.server
