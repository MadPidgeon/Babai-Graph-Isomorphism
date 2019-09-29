#pragma once
#include "ext.h"
#include <iostream>

class Coset;

#include "group.h"
#include "permutation.h"

// encodes a coset
class Coset {
  Group _G;
  Group _H;
  bool _right;
  Permutation _sigma;

public:
  // returns a shared reference to the group the coset is contained in, i.e. G
  // in G/H
  Group supergroup() const;

  // returns a shared reference to the divisor group, i.e. H in G/H
  Group subgroup() const;

  // checks whether the coset is a right coset
  bool isRightCoset() const;

  // returns a reference to a representative of the coset
  const Permutation &representative() const;

  // checks whether cosets are equal
  bool operator==(const Coset &) const;

  // constructs a coset
  Coset(Group G, Group H, Permutation sigma, bool right = true);
};

// prints the coset c to the output stream os
std::ostream &operator<<(std::ostream &os, const Coset &c);

// defines multiplication of permutations and cosets
Coset operator*(const Permutation &sigma, const Coset &tauH);

// encodes a set of isomorphisms, which is either a coset or empty
struct Iso : public Either<Coset, Empty> {
  // checks whether it is empty
  bool isEmpty() const;

  // returns the coset
  // WARNING: undefined behaviour if Iso is empty
  const Coset &coset() const;

  using Either::Either;
};

// defines multiplication of permutation and Iso sets
Iso operator*(const Permutation &sigma, const Iso &tauH);

// implements the union of cosets
class IsoJoiner {
  Group _supergroup;
  Group _subgroup;
  std::deque<Permutation> elements;

public:
  void join(Iso I);
  explicit operator Iso();
};