#ifndef C_LIBTORCH_H_
#define C_LIBTORCH_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define C_TORCH_TENSOR_MAX_DIM  (8)

typedef enum {
  c_torch_kInvalid,
  c_torch_kUint8,
  c_torch_kInt8,
  c_torch_kInt16,
  c_torch_kInt32,
  c_torch_kInt64,
  c_torch_kFloat16,
  c_torch_kFloat32,
  c_torch_kFloat64
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

void c_torch_version(int *major, int *minor, int *patch);

// Returns 1: CUDA is availabe, 0: CUDA is not available
int c_torch_cuda_is_available();

// torch::ones()
c_at_Tensor *c_torch_ones(int ndim, int *shape, c_torch_DType dtype);

// Alias for torch::ones({sz0}, at::kFloat32);
c_at_Tensor *c_torch_ones_1d(int sz0, c_torch_DType dtype);

// Alias for torch::ones({sz0, sz1});
c_at_Tensor *c_torch_ones_2d(int sz0, int sz1, c_torch_DType dtype);

// Alias for torch::ones({sz0, sz1, sz2});
c_at_Tensor *c_torch_ones_3d(int sz0, int sz1, int sz3, c_torch_DType dtype);

// Alias for torch::ones({sz0, sz1, sz2, sz3});
c_at_Tensor *c_torch_ones_4d(int sz0, int sz1, int sz3, c_torch_DType dtype);

#ifdef __cplusplus
}
#endif


#endif // C_LIBTORCH_H_
