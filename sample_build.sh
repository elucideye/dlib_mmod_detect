#!/bin/bash

# Simple build script illustrating build command on OS X platform.
# 1) Replace 'xcode' below with an appropriate toolchain from the polly repository.
# 2) Update CUDNN_HOME to correct path for your system.
#
# See: https://github.com/ruslo/polly

TOOLCHAIN=xcode
CONFIG=Release
CUDNN_HOME=${HOME}/pkg/cuDNN/v7.0.4 polly.py --toolchain ${TOOLCHAIN} --config-all ${CONFIG} --install ${*}
