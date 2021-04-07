#include <torch/all.h>

#include "c_libtorch.h"

namespace {

bool is_valid_dtype(c_torch_DType dtype) {
  if ((dtype == c_torch_kUint8)
  || (dtype == c_torch_kInt8)
  || (dtype == c_torch_kInt16)
  || (dtype == c_torch_kInt32)
  || (dtype == c_torch_kInt64)
  || (dtype == c_torch_kFloat16)
  || (dtype == c_torch_kFloat32)
  || (dtype == c_torch_kFloat64)
  ) {
    return true;
  }

  return false;

}

// Get torch native dtype.
constexpr auto get_dtype(c_torch_DType dtype) {
  if (dtype == c_torch_kFloat32) {
    return torch::kFloat32;
  }

  throw std::invalid_argument("Unknown dtype");
}

int test_c_libtorch() {
  return 0;
}

} // namespace

extern "C" {

struct TensorData
{
  at::Tensor tensor;
};


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

c_at_Tensor *c_torch_ones(int ndim, int *shape, c_torch_DType dtype) {
  if (shape == NULL) {
    return NULL;
  }

  if (!is_valid_dtype(dtype)) {
    return NULL;
  }

  c_at_Tensor *ct = reinterpret_cast<c_at_Tensor *>(malloc(sizeof(c_at_Tensor)));

  ct->dtype = dtype;
  ct->ndim = ndim;

  std::vector<int64_t> vshape;
  for (size_t i = 0; i < ndim; i++) {
    ct->shape[i] = shape[i];
    vshape.push_back(shape[i]);
  }

  TensorData *data = new TensorData();
  data->tensor = torch::ones(vshape, get_dtype(dtype));

  ct->data = data;

  return ct;

}

c_at_Tensor *c_torch_ones_1d(int sz, c_torch_DType dtype) {

  int shape[1];
  shape[0] = sz;

  return c_torch_ones(1, shape, dtype);
}

c_at_Tensor *c_torch_fft_fft(const c_at_Tensor *self, int64_t _n, int64_t dim, c_torch_fft_NormMode norm_mode) {
  c10::optional<int64_t> n = c10::nullopt;

  if (_n > 0) {
    n = _n;
  }

  c10::optional<std::string> norm = c10::nullopt;

  if (norm_mode == c_torch_fft_norm_forward) {
    norm = "forward";
  } else if (norm_mode == c_torch_fft_norm_backward) {
    norm = "backward";
  } else if (norm_mode == c_torch_fft_norm_ortho) {
    norm = "ortho";
  }

  at::Tensor t = torch::fft::fft(self->data->tensor, n, dim, norm);

  c_at_Tensor *ct = reinterpret_cast<c_at_Tensor *>(malloc(sizeof(c_at_Tensor)));

  ct->dtype = self->dtype;
  ct->ndim = 1;
  ct->shape[0] = t.sizes()[0];

  TensorData *data = new TensorData();
  data->tensor = t;

  ct->data = data;

  return ct;

}

} // extern "C"
