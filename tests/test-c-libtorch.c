#include "acutest.h"

#include "c_libtorch.h"

#define ASSET_PATH "../tests/"

void test_tutorial() {
  TEST_CHECK_(1 == 1, "bora");
}

void test_version() {
  int major, minor, patch;

  c_torch_version(&major, &minor, &patch);

  printf("version: %d.%d.%d\n", major, minor, patch);

}

void test_ones_f32_1() {
  c_at_Tensor *tp = c_torch_ones_1d(9, c_torch_kFloat32);
  delete_c_at_Tensor(tp);
}


void test_fft_f32() {
  c_at_Tensor *tp = c_torch_ones_1d(9, c_torch_kFloat32);

  c_at_Tensor *ft = c_torch_fft_fft(tp, /* n */-1, C_TORCH_DEFAULT_DIM, c_torch_fft_norm_none);

  delete_c_at_Tensor(tp);
  delete_c_at_Tensor(ft);
}

void test_jit_load() {
  c_torch_jit_script_Module *module = c_torch_jit_load(ASSET_PATH"jit-test.pt");

  TEST_CHECK_(module != NULL, "Failed to load module");

  delete_c_torch_jit_script_Module(module);
}

void test_cuda() {
  int has_cuda = c_torch_cuda_is_available();
  printf("cuda? %d\n", has_cuda);	
}

TEST_LIST = {
  { "tutorial", test_tutorial },
  { "version", test_version },
  { "ones(f32, n1)", test_ones_f32_1 },
  { "fft(f32)", test_fft_f32 },
  { "cuda", test_cuda },
  { "jit_load()", test_jit_load },
  { NULL, NULL }
};
