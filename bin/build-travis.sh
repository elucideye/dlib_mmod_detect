#!/bin/bash -e
#
# Sample input parameteters:
# TOOLCHAIN=libcxx
# CONFIG=MinSizeRel
# INSTALL=--strip

if [ ${#} != 3 ]; then
    echo 2>&1 "usage: drishti_build.sh TOOLCHAIN CONFIG INSTALL"
    exit 1
fi

TOOLCHAIN="${1}"
CONFIG="${2}"
INSTALL="${3}"

# Note: '--ios-{multiarch,combined}' do nothing for non-iOS builds
ARGS=(
    --toolchain "${TOOLCHAIN}"
    --config-all "${CONFIG}"
    --verbose
    --jobs 2
    "${INSTALL}"
)

polly.py ${ARGS[@]} --reconfig
