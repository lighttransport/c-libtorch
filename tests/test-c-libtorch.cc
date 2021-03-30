#include "acutest.h"

#include "c_libtorch.h"

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
}

TEST_LIST = {
  { "tutorial", test_tutorial },
  { "version", test_version },
  { "ones(f32, n1)", test_ones_f32_1 },
  { NULL, NULL }
};
