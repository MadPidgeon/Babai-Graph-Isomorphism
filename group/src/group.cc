#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

#include "group.h"
#include "permutation.h"
// #include "action.h"
#include "fhl.h"

Permutation _Group::one() const {
  int n = degree();
  std::vector<int> o(n);
  for (int i = 0; i < n; i++)
    o[i] = i;
  return Permutation(std::move(o));
}

bool _Group::hasSubgroup(Group H) const {
  for (const auto &gen : H->generators())
    if (!contains(gen))
      return false;
  return true;
}

std::vector<int> _Group::domain() const {
  std::vector<int> d(degree());
  for (int i = 0; i < degree(); i++)
    d[i] = i;
  return d;
}

Group _Group::stabilizer(int x) const {
  return Group(new Subgroup(
      share(), [x](const Permutation &sigma) { return sigma(x) == x; }));
}

Group _Group::share() const { return shared_from_this(); }

_Group::~_Group() {}

// ----------------------------------------------------------------------------

bool Subgroup::contains(const Permutation &alpha) const {
  if (!_fhl)
    _fhl.create(generators(), degree());
  return _fhl.contains(alpha);
}

Subgroup::Subgroup(Group G, std::vector<Permutation> gens) {
  swap(_supergroup, G);
  swap(_generators, gens);
}

Subgroup::Subgroup(Group G, std::function<bool(Permutation)> c)
    : Subgroup(SubgroupGenerator(G, c).subgroup()) {}

std::vector<Coset> _Group::allCosets(Group N) const {
  SubgroupGenerator sg(
      share(), [&](const Permutation &p) -> bool { return N->contains(p); });

  const auto &R = sg.cosetRepresentatives();
  std::vector<Coset> cs;
  cs.reserve(R.size());

  for (Permutation sigma : R)
    cs.emplace_back(share(), N, sigma, false);
  return cs;
}

bool Subgroup::isGiant() const {
  if (not _fhl)
    _fhl.create(generators(), degree());
  return _fhl.isGiant();
}

Subgroup::~Subgroup() {}

Group Subgroup::supergroup() const { return _supergroup; }

std::vector<Permutation> Subgroup::generators() const { return _generators; }

int Subgroup::degree() const { return supergroup()->degree(); }

__int128_t Subgroup::order() const {
  if (!_fhl)
    _fhl.create(generators(), degree());
  return _fhl.order();
}

Group Subgroup::join(std::deque<Permutation> &&P) const {
  std::vector<Permutation> new_generators;
  new_generators.reserve(generators().size() + P.size());
  new_generators.insert(new_generators.end(), _generators.cbegin(),
                        _generators.cend());
  for (int i = P.size() - 1; i >= 0; --i)
    new_generators.push_back(std::move(P[i]));
  return Group(new Subgroup(supergroup(), new_generators));
}

// ----------------------------------------------------------------------------

bool SymmetricGroup::contains(const Permutation &sigma) const {
  return degree() == sigma.degree();
}

int SymmetricGroup::degree() const { return _degree; }

__int128_t SymmetricGroup::order() const {
  return std::tgamma(_degree + 1) + .9;
}

Group SymmetricGroup::join(std::deque<Permutation> &&P) const {
  for (const Permutation &sigma : P)
    if (not contains(sigma))
      throw;
  return share();
}

std::vector<Permutation> SymmetricGroup::generators() const {
  std::vector<int> cycle(degree());
  std::vector<int> transposition(degree());
  for (int i = 0; i < degree(); i++) {
    cycle[i] = (i + 1) % degree();
    transposition[i] = i;
  }
  Permutation sigma(std::move(cycle));
  if (degree() <= 2)
    return std::vector<Permutation>({sigma});
  std::swap(transposition[0], transposition[1]);
  Permutation tau(std::move(transposition));
  return std::vector<Permutation>({sigma, tau});
}

SymmetricGroup::SymmetricGroup(int n) { _degree = n; }

SymmetricGroup::~SymmetricGroup() {}

bool SymmetricGroup::isGiant() const { return true; }
