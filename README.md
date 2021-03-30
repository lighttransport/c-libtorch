# Experimental C binding for libtorch

Experimental C binding for libtorch C++ interface.

For faster compilation with libtorch library, embedding libtorch to other languages through ffi, etc.

This project implements minimal and partial(features required for our usecases) C binding for libtorch API. There is no plan to implement solid and full C binding for libtorch.

## Supported platform

* [x] Linux x86-64
* Windows
  * [x] Visual Studio 2019
  * [ ] llvm-mingw
  * [ ] MinGW
* [ ] macOS(Limited support)
  * Big Sur x86/arm64
* Optional: CUDA

## Supported libtorch version

* libtorch 1.8.0

### Status

* [x] Minimal float32/float64 tensor.
* [ ] Consider CUDA(Acclerator) devices.

## Requirements

* CMake 3.2 or later
* C11 + C++14 compiler
* (optional)CUDA and cuDNN

## Build

### Install pytorch/libtorch

Prepare libtorch libraries. You can install pytorch or download prebuilt libtorch package, or build it from source code. If your pytorch/libtorch build uses CUDA, you'll need to install CUDA SDK and cuDNN package also.

### Linux

Note that pytorch packages from https://pytorch.org/get-started/locally/ (conda or pip package) contains libtorch headers and libraries, and these are compiled with gcc + pre-C++11 ABI for Linux platform.

Edit path to libtorch in `scripts/bootstrap-linux.sh`.

If your libtorch is compiled with pre-C++11 ABI(e.g. pytorch build), you need to specify `_GLIBCXX_USE_CXX11_ABI=0` when you compile c-libtorch with clang.

Then,

```
$ ./scripts/bootstrap-linux.sh
$ cd build
$ make
```

### Windows

Visual Studio 2019 is supported at the momenet.

Run `vcsetup2019.bat`.

Solution file will be generated in `build` directory.

To run unit test, you'll need to set PATH to pytorch/libtordh dlls(or copy dlls to your working directory).

### macOS(Big Sur or later)

CPU build only.

Assume some dependencies(e.g. protobuf) are installed through macports or homebrew.

If you want to build c-libtorch with pytorch package, you may need to delete linking with `libopenblas.dylib` in
`TORCH_DIR/python3.9/site-packages/torch/share/cmake/Caffe2/Caffe2Targets.cmake`

Then,

```
$ ./scripts/bootstrap-macos.sh
$ cd build
$ make
```

### Memory management

Currently c-libtorch does not have a policy for memory management.
C struct simply wraps C++ object(Assume no `detach()`, `clone()` operation for Tensor).

### API note

### Variable

C API has `c_` prefix. C++ namespaces are concatenated with `_`

Example: `at::Tensor` -> `c_at_Tensor`

### Function signature

C API has `c_` prefix. C++ namespaces are concatenated with `_`

Example: `torch::cuda::is_available` -> `c_torch_cuda_is_available`


## TODO

* [ ] TensorFlow C API style memory management.
* [ ] Better error handling.

## License

c-libtorch itself is licensed under MIT license.

### Third-party licenses

* libtorch, pytorch: BSD-like license.
* acutest: MIT license.
