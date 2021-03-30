#include <torch/all.h>

#include "c_libtorch.h"

struct TensorData
{
  at::Tensor t;
};

int test_c_libtorch() {
  return 0;
}

extern "C" {

void c_torch_version(int *major, int *minor, int *patch)
{
  if (major) {
    (*major) = TORCH_VERSION_MAJOR;
  }
  if (minor) {
    (*minor) = TORCH_VERSION_MINOR;
  }
  if (patch) {
    (*patch) = TORCH_VERSION_PATCH;
  }

}

int delete_c_at_Tensor(c_at_Tensor *obj) {
  if (obj == NULL) {
    return -1;
  }

  delete obj->data;

  free(obj);

  return 0; // success
}

c_at_Tensor *c_torch_ones_1(int sz, c_torch_DType dtype) {

  c_at_Tensor *ct = reinterpret_cast<c_at_Tensor *>(malloc(sizeof(c_at_Tensor)));

  ct->dtype = dtype;
  ct->ndim = 1;
  ct->shape[0] = sz;

  TensorData *data = new TensorData();
  data->t = torch::ones({sz}, torch::kFloat32);

  ct->data = data;

  return ct;
}

} // extern "C"
