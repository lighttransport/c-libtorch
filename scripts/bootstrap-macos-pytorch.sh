rm -rf build
mkdir build

cd build

# Path to TorchConfig cmake files.
TORCH_INSTALLED_DIR=`python -c "import site; print (site.getsitepackages()[0])"`/torch/share/cmake

echo "TORCH_DIR = " ${TORCH_INSTALLED_DIR}

# (optional) Explicitly specify the path to python
# -DPYTHON_EXECUTABLE=/path/to/bin/python

Torch_DIR=${TORCH_INSTALLED_DIR} \
CXX=clang++ CC=clang cmake -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_VERBOSE_MAKEFILE=1 \
  ..
