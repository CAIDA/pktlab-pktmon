#!/bin/bash
USAGE="Usage: bash $0 [PKTLAB_PKG_PATH] [WASI_SDK_PATH] [MONITOR_NAME]\n\nPKTLAB_PKG_PATH:\tPath to pktlab software package (or pktlab-libpktlab repo)\nWASI_SDK_PATH:\tPath to wasi-sdk \nMONITOR_NAME:\tName of monitor, default to \"monitor\" \n"

if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
    printf "Invalid number of args!\n" >&2
    printf "%b" "${USAGE}" >&2
    exit 1
fi

# Exit on error
set -e

# Set the monitor name
if [ "$#" -eq 3 ]; then
    export MONITOR_NAME="${3}"
else
    printf "%b" "Using default monitor name: monitor\n"
    export MONITOR_NAME="monitor"
fi

# find pktlab libraries
# support repo as well
PKTLAB_PKG_PATH="${1}"
if [ -f "${PKTLAB_PKG_PATH}/lib/libpktlab.ssl.a" ]; then
    export PKTLAB_PKG_LIB_PATH="${PKTLAB_PKG_PATH}/lib/libpktlab.ssl.a"
elif [ -f "${PKTLAB_PKG_PATH}/libpktlab.a" ]; then # repo case
    export PKTLAB_PKG_LIB_PATH="${PKTLAB_PKG_PATH}/libpktlab.a"
elif [ -f "${PKTLAB_PKG_PATH}/lib/libpktlab.a" ]; then # nothing else to use, so ...
    export PKTLAB_PKG_LIB_PATH="${PKTLAB_PKG_PATH}/lib/libpktlab.a"
else
    printf "%b" "Cannot find libpktlab.ssl.a/libpktlab.a, aborting\n" >&2
    exit 1
fi

# should always be under include/ even for repo case
export PKTLAB_PKG_INC_PATH="${PKTLAB_PKG_PATH}/include"
export WASI_SDK_PATH="${2}"

make -f local.make
make -f comp_prog_digest.make

printf "%b" "Build monitor complete.\n"
./comp_prog_digest "$MONITOR_NAME.wasm" > "$MONITOR_NAME.digest"
