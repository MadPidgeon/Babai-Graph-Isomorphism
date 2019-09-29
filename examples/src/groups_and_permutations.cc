#include "group.h"
#include "permutation.h"
#include <cmath>
#include <iostream>

int main() {
  // define some permutations from array notation
  Permutation sigma({1, 2, 3, 4, 5, 6, 7, 0});
  Permutation tau({2, 3, 4, 5, 6, 7, 0, 1});
  Permutation mu({1, 0, 2, 3, 4, 5, 6, 7});

  // define some group
  Group S8(new SymmetricGroup(8));
  Group G(new Subgroup(S8, {sigma}));
  Group H(
      new Subgroup(S8, {{1, 2, 3, 0, 4, 5, 6, 7}, {0, 1, 2, 3, 5, 6, 7, 4}}));
  Group I(new Subgroup(S8, {{1, 2, 3, 0, 4, 5, 6, 7},
                            {0, 1, 2, 3, 5, 6, 7, 4},
                            {4, 5, 6, 7, 0, 1, 2, 3}}));

  // print the action
  std::cout << sigma(5) << std::endl;
  std::cout << mu(1) << std::endl;

  std::cout << "------------------------------" << std::endl;
  // print permutations in cycle notation
  std::cout << sigma << std::endl;
  std::cout << tau << std::endl;

  std::cout << "------------------------------" << std::endl;
  // print the orders of group
  std::cout << long(S8->order()) << "=" << std::tgamma(9) << std::endl;
  std::cout << int(G->order()) << std::endl;
  std::cout << int(H->order()) << std::endl;
  std::cout << int(I->order()) << std::endl;

  std::cout << "------------------------------" << std::endl;
  // check whether group contain elements
  std::cout << std::boolalpha;
  std::cout << G->contains(tau) << std::endl;
  std::cout << G->contains(mu) << std::endl;
  std::cout << G->contains(G->one()) << std::endl;

  std::cout << "------------------------------" << std::endl;
  // basic permutation operations
  std::cout << ((sigma ^ 2) == tau) << std::endl;
  std::cout << (mu.inverse() == mu) << std::endl;

  std::cout << "------------------------------" << std::endl;
  // check whether group contains subgroup
  std::cout << I->hasSubgroup(H) << std::endl;

  return 0;
}