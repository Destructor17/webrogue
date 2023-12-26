cd $(dirname $0)
cmake -B cmake_build/ -S . -G Xcode -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 || exit 1
