rm -rf build
mkdir build

cd build

TORCH_INSTALLED_DIR=~/local/libtorch/share/cmake/Torch

echo "TORCH_DIR = " ${TORCH_INSTALLED_DIR}

# Specify _GLIBCXX_USE_CXX11_ABI=0 when you compile c-libtorch using clang with precompiled libtorch which is built with pre-C++11 ABI
# TORCH_CXX_FLAGS="-D_GLIBCXX_USE_CXX11_ABI=0"

# (optional) Enable Address Sanitizer
#  -DSANITIZE_ADDRESS=1

Torch_DIR=${TORCH_INSTALLED_DIR} \
CXX=clang++ CC=clang cmake -DCMAKE_BUILD_TYPE=Debug \
  -DC_LIBTORCH_PYTHON_ENABLED=Off \
  -DCMAKE_VERBOSE_MAKEFILE=1 \
  ..
