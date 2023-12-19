#!/bin/bash

exec > >(tee -a build.log)
exec 2>&1


log() {
    echo "[$(date)] : $@"
}


echo "---------------------------------------------"
log "Starting build process..."
echo ---------------------------------------------

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_FILE="$(basename $SCRIPT_DIR)"

SOURCE="src/*.cpp"
INCLUDE="inc"

VENDOR_INC="inc/vendor/inc"
VENDOR_SRC="inc/vendor/src/*.c*"

if [ ! -d "bin" ]; then
    log "Creating bin directory..."
    mkdir bin
else
    log "Cleaning bin directory..."
    rm -rf bin/*
fi

CC="g++"
CFLAGS="-Wall -Wextra -Werror -std=c++20 -I${INCLUDE} -I${VENDOR_INC}"
LDFLAGS="pkg-config --static --libs glfw3"

OUTPUT="bin/${SCRIPT_FILE}"

log "Building ${SCRIPT_FILE}..."
echo "------------[ COMPILER OUTPUT ]--------------"
$CC $CFLAGS $VENDOR_SRC $SOURCE -o $OUTPUT $($LDFLAGS)

if [ $? -eq 0 ]; then
    log "Build successful! ... "
    log "run with: ./${OUTPUT}"
else
    log "Build failed! ... "
fi

echo "---------------------------------------------"
log "exit with code : $?"
exit $?


