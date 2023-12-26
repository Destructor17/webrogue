cd $(dirname $0)
sh configure.sh || exit 12  || exit 12 
rm -rf ../../artifacts

XCODEBUILD_FLAGS="-project cmake_build/webrogue_macos.xcodeproj -scheme webrogue -configuration Debug"

xcodebuild $XCODEBUILD_FLAGS || exit 2
XC_BUILD_DIR=$(xcodebuild $XCODEBUILD_FLAGS -showBuildSettings | grep -m 1 "BUILT_PRODUCTS_DIR" | grep -oEi "\/.*" || exit 3)
mkdir ../../artifacts
cp -r $XC_BUILD_DIR/Webrogue.app ../../artifacts
