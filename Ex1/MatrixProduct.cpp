#include "Matrix.h"

int main() {
  int s1 = -1;
  int s2 = -1;
  int s3 = -1;

  std::cin >> s1;
  if (!s1)
    assert(false);
  std::cin >> s2;
  if (!s2)
    assert(false);
  std::cin >> s3;
  if (!s3)
    assert(false);

  Matrix a(s1, s2, 0);
  Matrix b(s2, s3, 0);

  for (int i = 0; i < s1 * s2; i++) {
    std::cin >> a.data[i];
    if (!a.data[i] && a.data[i] != 0)
      assert(false);
  }

  for (int i = 0; i < s2 * s3; i++) {
    std::cin >> b.data[i];
    if (!b.data[i] && b.data[i] != 0)
      assert(false);
  }

  auto other = a * b;
  std::cout << other << std::endl;
  return 0;
}
