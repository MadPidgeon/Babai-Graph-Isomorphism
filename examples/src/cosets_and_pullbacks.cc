#include "action.h"
#include "group.h"
#include "permutation.h"
#include <iostream>

int main() {
  Group S12(new SymmetricGroup(12));
  NaturalSetAction A(S12, 12, 2);
  auto S12_2 = A.anonymize();

  auto B = A.systemOfImprimitivity();
  Group G = B.anonymize();
  Group H = B.kernel();

  std::cout << G << std::endl;
  std::cout << H << std::endl;

  return 0;
}