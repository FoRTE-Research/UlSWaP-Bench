SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BENCHMARKS_ROOT=$SCRIPT_DIR/../..
PICOLIBC_TOP_DIR=$BENCHMARKS_ROOT/picolibc

SUBARCH=rv32im

BUILD_DIR=$PICOLIBC_TOP_DIR/build/spike-"$SUBARCH"
INSTALL_DIR=$PICOLIBC_TOP_DIR/install/spike-"$SUBARCH"

# Check if the build and install directories exist, if not create them
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

if [ ! -d "$INSTALL_DIR" ]; then
    mkdir -p "$INSTALL_DIR"
fi

CROSS_FILE=$SCRIPT_DIR/picolibc_meson_cross_config.txt

cd $BUILD_DIR

meson setup \
    --reconfigure \
    --cross-file "$CROSS_FILE" \
    -Dincludedir="include" \
    -Dlibdir="lib" \
    -Dspecsdir="specs" \
    -Dtests=false \
    -Dformat-default=float \
    -Dtests-enable-stack-protector=false \
    -Dtests-enable-full-malloc-stress=false \
    -Dmultilib=false \
    -Dprefix=$INSTALL_DIR \
    $PICOLIBC_TOP_DIR

ninja -j`nproc`
ninja install
