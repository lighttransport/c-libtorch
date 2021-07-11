rm -rf build
mkdir build

cd build

# Path to TorchConfig cmake files. This script uses python to look up the path to TorchConfig.cmake.
# You can also manually specify the path to torch e.g. TORCH_INSTALLED_DIR=/opt/torch/share/cmake
TORCH_INSTALLED_DIR=`python -c "import site; print (site.getsitepackages()[0])"`/torch/share/cmake

echo "TORCH_DIR = " ${TORCH_INSTALLED_DIR}

# Specify _GLIBCXX_USE_CXX11_ABI=0 when you compile c-libtorch using clang with precompiled libtorch which is built with pre-C++11 ABI
# TORCH_CXX_FLAGS="-D_GLIBCXX_USE_CXX11_ABI=0"

# (optional) Explicitly specify the path to python
# -DPYTHON_EXECUTABLE=/path/to/bin/python

# (optional) Enable Address Sanitizer
#  -DSANITIZE_ADDRESS=1

Torch_DIR=${TORCH_INSTALLED_DIR} \
CXX=clang++ CC=clang cmake -DCMAKE_BUILD_TYPE=Debug \
  -DTORCH_CXX_FLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" \
  -DCMAKE_VERBOSE_MAKEFILE=1 \
  ..
