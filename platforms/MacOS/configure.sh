cd $(dirname $0)
cmake -B cmake_build/ -S . -G Xcode || exit 1
