#pragma once

#include "permutation.h"
#include <deque>
#include <vector>

template <typename T = Permutation> class FHL;

template <typename T>
std::ostream &operator<<(std::ostream &os, const FHL<T> &fhl);

class PermutationPullback;
class SubgroupGenerator;
class PullbackStructure;

#include "ext.h"
#include "permutation.h"

class PermutationPullback {
  Permutation original;
  Permutation pullback;

public:
  Permutation getPullback() const;
  bool isIdentity() const;
  int degree() const;
  PermutationPullback inverse() const;
  int operator()(int) const;
  PermutationPullback operator*(const PermutationPullback &)const;
  PermutationPullback(Permutation);
  PermutationPullback(Permutation &&, Permutation &&);
};

// *************************************************************
// T should have the following:
//   - An implicit cast to and from Permutation;
//   - A well defined degree, inverse and identity check that
//     commutes with casts to Permutation;
//   - An operator() evaluation of integers that commutes with
//     casts to Permutation;
//   - Multiplication that commutes with cast to Permutation.
// Permutation itself trivially satisfies these conditions
// *************************************************************

template <typename T> class FHL {
protected:
  friend std::ostream &operator<<<>(std::ostream &os, const FHL<T> &fhl);
  mutable std::vector<std::vector<T>> V; // very important permutations
  size_t n, m;

  // takes a permutation and writes it as a product of coset representatives if
  // possible. if not, it adds the modified permutation to a list of coset
  // representatives when add=true.
  T filter(T sigma, bool add) const;

public:
  // clears all data in the structure
  void clear();

  // initialises the structure using S as generators with degree d
  void create(std::vector<T> S, size_t d);

  // checks whether sigma is an element of the group encoded by this structure
  bool contains(const T &sigma) const;

  // write sigma as a product of coset representatives
  T find(const T &sigma) const;

  // checks whether the group encoded by this structure is a giant
  bool isGiant() const;

  // computes the order of the group encoded by this structure
  __int128_t order() const;

  // returns a list of O(n^2) generators for the group encoded by this structure
  std::vector<T> listGenerators() const;

  // checks whether the structure is empty
  bool operator!() const;

  // constructs the structure
  FHL();

  // constructs the structure, equivalent to {FHL X(), X.create( S, d )}
  FHL(const std::vector<T> &S, size_t d);
};

template <typename T> void FHL<T>::clear() {
  n = m = 0;
  V.clear();
}

template <typename T> void FHL<T>::create(std::vector<T> generators, size_t s) {
  clear();
  n = s;
  m = n - 1;
  if (n > 0) {
    V.resize(m);
    for (size_t i = 0; i < m; ++i)
      V[i].resize(n - i - 1, T(Permutation(0)));
    std::deque<T> new_permutations;
    for (auto sigma : generators)
      new_permutations.push_back(filter(sigma, true));
    while (not new_permutations.empty()) {
      T sigma = std::move(new_permutations.front());
      T mu(Permutation(0));
      new_permutations.pop_front();
      for (const auto &W : V) {
        for (const auto &tau : W) {
          if (tau.degree() > 0) {
            T nu = tau.inverse();
            mu = filter(sigma * nu, true);
            if (not mu.isIdentity())
              new_permutations.push_back(std::move(mu));
            mu = filter(nu * sigma, true);
            if (not mu.isIdentity())
              new_permutations.push_back(std::move(mu));
          }
        }
      }
    }
  }
}

template <typename T> T FHL<T>::filter(T sigma, bool add) const {
  // Stabilise i
  for (size_t i = 0; i < m; ++i) {
    size_t p = sigma(i);
    if (p != i) {
      p -= i + 1;
      if (V[i][p].degree() == 0) {
        if (add)
          V[i][p] = sigma.inverse();
        break;
      } else
        sigma = V[i][p] * sigma;
    }
  }
  return sigma;
  // returns (1) if found, incomplete filtrate otherwise
}

template <typename T> bool FHL<T>::contains(const T &sigma) const {
  return filter(sigma, false).isIdentity();
}

template <typename T> T FHL<T>::find(const T &sigma) const {
  return filter(sigma, false);
}

template <typename T> FHL<T>::FHL(const std::vector<T> &generators, size_t s) {
  create(generators, s);
}

template <typename T> FHL<T>::FHL() { clear(); }

template <typename T> std::vector<T> FHL<T>::listGenerators() const {
  std::vector<T> gens;
  size_t i = 0;
  for (const auto &X : V)
    for (const auto &Y : X)
      if (not Y.isIdentity())
        ++i;
  gens.reserve(i);
  for (const auto &X : V)
    for (const auto &Y : X)
      if (not Y.isIdentity())
        gens.push_back(Y);
  return gens;
}

template <typename T> bool FHL<T>::isGiant() const {
  auto e = V.end() - 1;
  for (auto b = V.begin(); b != e; ++b)
    for (const T &sigma : *b)
      if (sigma.degree() == 0)
        return false;
  return true;
}

template <typename T> __int128_t FHL<T>::order() const {
  __int128_t r = 1;
  for (const auto &W : V) {
    __int128_t s = 1;
    for (const auto &sigma : W)
      if (sigma.degree() != 0)
        ++s;
    r *= s;
  }
  return r;
}

template <typename T> bool FHL<T>::operator!() const { return n == 0; }

template <typename T>
std::ostream &operator<<(std::ostream &os, const FHL<T> &fhl) {
  return os << fhl.V;
}

#include "group.h"

class PullbackStructure : public FHL<PermutationPullback> {
  Group domain;

public:
  Permutation operator()(Permutation) const;
  PullbackStructure(Group pullback_space,
                    const std::vector<PermutationPullback> &);
  PullbackStructure(Group pullback_space, std::vector<Permutation> originals,
                    std::vector<Permutation> pullbacks);
};

class SubgroupGenerator : public FHL<Permutation> {
  Group G;
  mutable std::deque<Permutation> representatives;
  std::function<bool(Permutation)> check;

  Permutation filter(Permutation sigma, bool add) const;
  void subcreate();

public:
  // analog of FHL
  void clear();
  void create(Group H, std::function<bool(Permutation)> func);
  void create(Group H, const FHL<Permutation> &P);
  bool contains(const Permutation &sigma) const;
  Permutation find(const Permutation &sigma) const;

  // returns the subgroup defined by the check function
  Subgroup subgroup() const;

  // returns all coset representatives of the quotient group
  std::deque<Permutation> cosetRepresentatives() const;

  // constructor
  SubgroupGenerator(Group H, std::function<bool(Permutation)> func);
  SubgroupGenerator(const FHL<Permutation> &P);
  SubgroupGenerator() = default;
};