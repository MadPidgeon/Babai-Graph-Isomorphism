#include <string>

#include "luks.h"

int main() {
  Group S24(new SymmetricGroup(24));
  NaturalSetAction A(S24, 24, 2);
  auto S24_2 = A.anonymize();
  std::string u =
      "abababababababababababababababababababababababababababababababababababab"
      "abababababababababababababababababababababababababababababababababababab"
      "abababababababababababababababababababababababababababababababababababab"
      "abababababababababababababababababababababababababababababab";
  std::string v =
      "babababababababababababababababababababababababababababababababababababa"
      "babababababababababababababababababababababababababababababababababababa"
      "babababababababababababababababababababababababababababababababababababa"
      "babababababababababababababababababababababababababababababa";

  NaturalAction A2(S24_2);
  auto B = A2.systemOfImprimitivity();
  Group G = A2.anonymize();

  std::cout << StringIsomorphismCameronGroup(B, G, u, v) << std::endl;

  return 0;
}
