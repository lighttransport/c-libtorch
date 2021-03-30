#ifndef C_LIBTORCH_H_
#define C_LIBTORCH_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#ifdef C_TORCH_COMPILE_LIBRARY
#define C_TORCH_EXPORT __declspec(dllexport)
#else
#define C_TORCH_EXPORT __declspec(dllimport)
#endif 
#else
#define C_TORCH_EXPORT __attribute__((visibility("default")))
#endif


#define C_TORCH_TENSOR_MAX_DIM  (8)

typedef enum {
  c_at_kUint8,
  c_at_kInt8,
  c_at_kInt16,
  c_at_kInt32,
  c_at_kInt64,
  c_at_kFloat16,
  c_at_kFloat32,
  c_at_kFloat64
} c_torch_DType;

struct TensorData;  // Opaque

typedef struct {
  c_torch_DType dtype;
  int ndim;
  int shape[C_TORCH_TENSOR_MAX_DIM];
  TensorData *data;
} c_at_Tensor;

// Delete Tensor object.
int delete_c_at_Tensor(c_at_Tensor *obj);

int test_c_libtorch();

C_TORCH_EXPORT void c_torch_version(int *major, int *minor, int *patch);

// non-zero: CUDA is availabe
C_TORCH_EXPORT int c_torch_cuda_is_available();

// torch::ones()
C_TORCH_EXPORT c_at_Tensor *c_torch_ones(int ndim, int *shape, c_torch_DType dtype);

// Alias for torch::ones({sz0}, at::kFloat32);
C_TORCH_EXPORT c_at_Tensor *c_torch_ones_1(int sz0, c_torch_DType dtype);

// Alias for torch::ones({sz0, sz1});
C_TORCH_EXPORT c_at_Tensor *c_torch_ones_2(int sz0, int sz1, c_torch_DType dtype);

// Alias for torch::ones({sz0, sz1, sz2});
C_TORCH_EXPORT c_at_Tensor *c_torch_ones_3(int sz0, int sz1, int sz3, c_torch_DType dtype);

// Alias for torch::ones({sz0, sz1, sz2, sz3});
C_TORCH_EXPORT c_at_Tensor *c_torch_ones_4(int sz0, int sz1, int sz3, c_torch_DType dtype);

#ifdef __cplusplus
}
#endif


#endif // C_LIBTORCH_H_
