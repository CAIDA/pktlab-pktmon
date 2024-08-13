#!/bin/bash
USAGE="Usage: bash $0 [WASI_SDK_PATH] [MONITOR_NAME]\n\nWASI_SDK_PATH:\tPath to wasi-sdk \nMONITOR_NAME:\tName of monitor, default to \"monitor\" \n"

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
    printf "Invalid number of args!\n" >&2
    printf "%b" "${USAGE}" >&2
    exit 1
fi

# Exit on error
set -e

# Set the monitor name
if [ "$#" -eq 2 ]; then
    export MONITOR_NAME="${2}"
else
    echo -e "Using default monitor name: monitor\n"
    export MONITOR_NAME="monitor"
fi

export WASI_SDK_PATH="${1}"

make -f local.make
make -f comp_prog_digest.make

echo -e "Build monitor complete."
./comp_prog_digest $MONITOR_NAME.wasm > $MONITOR_NAME.digest
