#pragma once

#include <exception>
#include <ext.h>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

#include "group.h"
#include "multi.h"
#include "unionfind.h"

template <typename T> using action_set = std::vector<T>;

template <typename T, int k> using action_array = std::array<T, k>;

class NaturalAction;
class NaturalSetAction;
class RestrictedNaturalSetAction;

// Uses curiously recurring template pattern
// Defines the interface of the action class
template <typename A, typename value_type, typename domain_type> class Action {
  Group _group;
  mutable std::vector<std::vector<value_type>> _orbits;
  mutable Group _kernel;

public:
  // returns a shared reference to the group
  Group group() const;

  // computes the orbit under the group action containing the seed
  template <typename T = action_set<value_type>> T orbit(value_type seed) const;

  // returns all orbits under the group action (cached)
  const std::vector<std::vector<value_type>> &orbits() const;

  // computes all orbits under the group action
  std::vector<std::vector<value_type>> calculateOrbits() const;

  // converts the domain to integers and returns the induced group action on
  // those
  Group anonymize(std::map<value_type, int> *inv_map = nullptr) const;

  // checks whether the action is transitive
  bool isTransitive() const;

  // checks whether the action is trivial
  bool isTrivial() const;

  // computes the kernel of the action (stabiliser)
  Group kernel() const;

  // computes a system of imprimitivity on which the group acts (Atkinson)
  RestrictedNaturalSetAction systemOfImprimitivity() const;

  // defines the action
  value_type operator()(const Permutation &sigma, const value_type &x) const;

  // constructor
  Action(Group G);
};

// Uses curiously recurring template pattern
// Defines the interface of the point-wise action class
template <typename A, typename value_type, typename domain_type>
class PointAction : public Action<A, value_type, domain_type> {
public:
  // computes the orbits
  std::vector<std::vector<value_type>> calculateOrbits() const;

  // defines the action
  value_type act(const Permutation &sigma, value_type x) const;

  // computes a random block system
  RestrictedNaturalSetAction randomBlocksystem() const;

  // constructor
  PointAction(Group G);
};

// Defines the action of the group on all integers {0,...,n-1}
class NaturalAction : public PointAction<NaturalAction, int, range> {
public:
  typedef int value_type;
  typedef range domain_type;

  // returns the domain
  domain_type domain() const;

  Group anonymize() const;

  // constructor
  NaturalAction(Group G);
};

// Defines the action of the group on a different set of integers
class RestrictedNaturalAction
    : public PointAction<RestrictedNaturalAction, int, std::deque<int>> {
public:
  typedef int value_type;
  typedef std::deque<int> domain_type;

private:
  domain_type Omega;

public:
  // returns the domain
  const domain_type &domain() const;

  // constructor
  RestrictedNaturalAction(Group G, const domain_type &S);
  RestrictedNaturalAction(Group G, domain_type &&S);
};

// Uses curiously recurring template pattern
// Defines the interface of the action on tuples
template <typename A, typename value_type, typename domain_type, int k>
class ArrayAction : public Action<A, value_type, domain_type> {
public:
  value_type act(const Permutation &, value_type) const;
  ArrayAction(Group G);
};

// Defines the action of the group on all tuples in {0,...,n-1}^k
template <int k>
class NaturalArrayAction
    : public ArrayAction<NaturalArrayAction<k>, action_array<int, k>,
                         all_arrays<k>, k> {
public:
  typedef action_array<int, k> value_type;
  typedef all_arrays<k> domain_type;

  domain_type domain() const;
  std::vector<std::vector<value_type>> calculateOrbits() const;
  range subdomain() const;
  NaturalArrayAction(Group G);
};

// Uses curiously recurring template pattern
// Defines the interface of the action on sets of fixed size
template <typename A, typename value_type, typename domain_type>
class SetAction : public Action<A, value_type, domain_type> {
protected:
  int k;

public:
  value_type act(const Permutation &, const value_type &) const;
  SetAction(Group G, int set_size);
};

// Defines the action on all sets of integers of fixed size in {0,...,n-1}
class NaturalSetAction
    : public SetAction<NaturalSetAction, action_set<int>, all_ordered_tuples> {
  int n;

public:
  typedef action_set<int> value_type;
  typedef all_ordered_tuples domain_type;

  domain_type domain() const;
  range subdomain() const;

  NaturalSetAction(Group G, int subdomain_size, int set_size);
};

// Defines the action on some sets of integers of fixed size in {0,...,n-1}
class RestrictedNaturalSetAction
    : public SetAction<RestrictedNaturalSetAction, action_set<int>,
                       std::deque<action_set<int>>> {
public:
  typedef action_set<int> value_type;
  typedef std::deque<value_type> domain_type;

private:
  std::deque<action_set<int>> Omega;

public:
  const domain_type &domain() const;
  RestrictedNaturalSetAction randomBlocksystem() const;
  RestrictedNaturalSetAction(const NaturalAction &);
  RestrictedNaturalSetAction(Group G, const domain_type &D);
  RestrictedNaturalSetAction(Group G, domain_type &&D);
  RestrictedNaturalSetAction(const NaturalSetAction &);
};

// ----------------------------------------------------------------

template <typename A, typename value_type, typename domain_type>
Group Action<A, value_type, domain_type>::group() const {
  return _group;
}

template <typename A, typename value_type, typename domain_type>
const std::vector<std::vector<value_type>> &
Action<A, value_type, domain_type>::orbits() const {
  if (_orbits.size() == 0)
    _orbits = static_cast<const A *>(this)->calculateOrbits();
  return _orbits;
}

template <typename A, typename value_type, typename domain_type>
std::vector<std::vector<value_type>>
Action<A, value_type, domain_type>::calculateOrbits() const {
  const A *a = static_cast<const A *>(this);
  const auto &gens = group()->generators();
  size_t n = a->domain().size();
  size_t orbit_count = n;
  std::map<value_type, int> elements;
  UnionFind uf(n);
  int i = 0;
  for (const auto &x : a->domain())
    elements[x] = i++;
  i = 0;
  for (const auto &x : a->domain()) {
    for (const auto &g : gens)
      orbit_count -= uf.cup(i, elements[a->act(g, x)]);
    i++;
  }
  std::vector<std::vector<value_type>> r(orbit_count);
  std::map<int, int> indices;
  i = 0;
  orbit_count = 0;
  for (const auto &x : a->domain()) {
    int j = uf.find(i);
    if (i == j)
      indices[j] = orbit_count++;
    r[indices[j]].push_back(x);
    i++;
  }
  return r;
}

template <typename A, typename value_type, typename domain_type>
bool Action<A, value_type, domain_type>::isTransitive() const {
  return orbits().size() == 1;
}

template <typename A, typename value_type, typename domain_type>
bool Action<A, value_type, domain_type>::isTrivial() const {
  return static_cast<const A *>(this)->domain().size() == 1;
}

template <typename A, typename value_type, typename domain_type>
value_type
Action<A, value_type, domain_type>::operator()(const Permutation &sigma,
                                               const value_type &x) const {
  return static_cast<const A *>(this)->act(sigma, x);
}

template <typename A, typename value_type, typename domain_type>
Action<A, value_type, domain_type>::Action(Group G) : _group(G) {}

template <typename A, typename value_type, typename domain_type>
RestrictedNaturalSetAction
Action<A, value_type, domain_type>::systemOfImprimitivity() const {
  RestrictedNaturalSetAction X(*static_cast<const A *>(this));
  RestrictedNaturalSetAction Y = X.randomBlocksystem();
  while (not Y.isTrivial()) {
    X = std::move(Y);
    Y = X.randomBlocksystem();
  }
  return X;
}

template <typename A, typename value_type, typename domain_type>
template <typename T>
T Action<A, value_type, domain_type>::orbit(value_type seed) const {
  std::stack<value_type> to_do;
  std::set<value_type> done;
  to_do.emplace(std::move(seed));
  while (not to_do.empty()) {
    value_type x = to_do.top();
    to_do.pop();
    for (const auto g : group()->generators()) {
      value_type y = operator()(g, x);
      if (not done.count(y)) {
        done.insert(y);
        to_do.emplace(std::move(y));
      }
    }
  }
  return T(done.begin(), done.end());
}

template <typename A, typename value_type, typename domain_type>
Group Action<A, value_type, domain_type>::kernel() const {
  // warning: brute-force computation. Needs step by set computation for
  // polynomial time
  if (_kernel)
    return _kernel;
  const auto &d = static_cast<const A *>(this)->domain();
  _kernel.reset(
      new Subgroup(group(), [d, this](const Permutation &sigma) -> bool {
        for (const auto &x : d)
          if (operator()(sigma, x) != x)
            return false;
        return true;
      }));
  return _kernel;
}

template <typename A, typename value_type, typename domain_type>
Group Action<A, value_type, domain_type>::anonymize(
    std::map<value_type, int> *inverse_map) const {
  int n = static_cast<const A *>(this)->domain().size();
  Group S_n(new SymmetricGroup(n));
  bool own_map = false;
  if (not inverse_map)
    inverse_map = new std::map<value_type, int>();
  std::vector<Permutation> generators;
  int i = 0;
  for (const auto &x : static_cast<const A *>(this)->domain())
    (*inverse_map)[x] = i++;
  for (const Permutation &sigma : group()->generators()) {
    std::vector<int> generator(n);
    i = 0;
    for (const auto &x : static_cast<const A *>(this)->domain())
      generator[i++] = (*inverse_map)[operator()(sigma, x)];
    generators.emplace_back(std::move(generator));
  }
  if (own_map)
    delete inverse_map;
  return Group(new Subgroup(S_n, generators));
}

// ----------------------------------------------------------------

template <typename A, typename value_type, typename domain_type>
std::vector<std::vector<value_type>>
PointAction<A, value_type, domain_type>::calculateOrbits() const {
  size_t n = static_cast<const A *>(this)->domain().size();
  const auto &gens = this->group()->generators();
  UnionFind uf(n);
  std::vector<std::vector<int>> r;
  std::map<int, int> x;
  for (size_t i = 0; i < n; i++)
    for (const auto &g : gens)
      uf.cup(i, g(i));
  for (auto i : static_cast<const A *>(this)->domain()) {
    int a = uf.find(i);
    if (x.count(a) == 0) {
      x[a] = r.size();
      r.push_back({int(i)});
    } else
      r[x[a]].push_back(i);
  }
  return r;
}

template <typename A, typename value_type, typename domain_type>
RestrictedNaturalSetAction
PointAction<A, value_type, domain_type>::randomBlocksystem() const {
  size_t N = static_cast<const A *>(this)->domain().size();
  size_t block_count;
  std::stack<value_type> C;
  UnionFind f(N);
  std::vector<value_type> P_omega;
  auto beg = static_cast<const A *>(this)->domain().cbegin();
  auto end = static_cast<const A *>(this)->domain().cend();
  int naught = *(beg++);
  for (; beg != end; beg++) {
    int omega = *beg;
    C.push(omega);
    f.clear();
    f.cup(naught, omega);
    block_count = N - 1;
    while (!C.empty()) {
      int beta = C.top();
      C.pop();
      int alpha = f.find(beta);
      for (const auto &g : this->group()->generators()) {
        int gamma = act(g, alpha);
        int delta = act(g, beta);
        if (f.find(gamma) != f.find(delta)) {
          C.push(std::max(f.find(gamma), f.find(delta)));
          block_count -= f.cup(gamma, delta);
        }
      }
    }
    P_omega.resize(N / block_count);
    int j = 0;
    for (int i : static_cast<const A *>(this)->domain())
      if (f.find(i) == 0)
        P_omega[j++] = i;
    if (block_count != 1)
      break;
  }
  RestrictedNaturalSetAction::domain_type block_system =
      NaturalSetAction(this->group(), N, P_omega.size())
          .orbit<RestrictedNaturalSetAction::domain_type>(P_omega);
  return RestrictedNaturalSetAction(this->group(), std::move(block_system));
}

template <typename A, typename value_type, typename domain_type>
value_type
PointAction<A, value_type, domain_type>::act(const Permutation &sigma,
                                             value_type x) const {
  return sigma(x);
}

template <typename A, typename value_type, typename domain_type>
PointAction<A, value_type, domain_type>::PointAction(Group G)
    : Action<A, value_type, domain_type>(G) {}

// ----------------------------------------------------------------

template <typename A, typename value_type, typename domain_type>
value_type
SetAction<A, value_type, domain_type>::act(const Permutation &sigma,
                                           const value_type &x) const {
  value_type r(k);
  for (int i = 0; i < k; ++i)
    r[i] = sigma(x[i]);
  std::sort(r.begin(), r.end());
  return r;
}

template <typename A, typename value_type, typename domain_type>
SetAction<A, value_type, domain_type>::SetAction(Group G, int set_size)
    : Action<A, value_type, domain_type>(G) {
  k = set_size;
}

// ----------------------------------------------------------------

template <typename A, typename value_type, typename domain_type, int k>
value_type
ArrayAction<A, value_type, domain_type, k>::act(const Permutation &sigma,
                                                value_type x) const {
  for (int i = 0; i < k; i++)
    x[i] = sigma(x[i]);
  return x;
}

template <typename A, typename value_type, typename domain_type, int k>
ArrayAction<A, value_type, domain_type, k>::ArrayAction(Group G)
    : Action<A, value_type, domain_type>(G) {}

// ----------------------------------------------------------------

template <int k>
NaturalArrayAction<k>::NaturalArrayAction(Group G)
    : ArrayAction<NaturalArrayAction<k>, NaturalArrayAction<k>::value_type,
                  NaturalArrayAction<k>::domain_type, k>(G) {}

template <int k> range NaturalArrayAction<k>::subdomain() const {
  return range(0, this->group()->degree());
}

template <int k>
typename NaturalArrayAction<k>::domain_type
NaturalArrayAction<k>::domain() const {
  return domain_type(subdomain().size());
}

template <int k>
std::vector<std::vector<typename NaturalArrayAction<k>::value_type>>
NaturalArrayAction<k>::calculateOrbits() const {
  size_t n = subdomain().size();
  size_t nk = domain().size();
  const auto &gens = this->group()->generators();
  UnionFind uf(nk);
  std::vector<std::vector<value_type>> r;
  value_type y;
  size_t c = nk;
  int i = 0;
  for (const auto &x : domain()) {
    for (const auto &sigma : gens) {
      y = this->act(sigma, x);
      c -= uf.cup(polynomial_evaluation(y, n), i);
    }
    i++;
  }
  std::unordered_map<int, int> indices;
  i = 0;
  int j = 0;
  r.resize(c);
  for (const auto &x : domain()) {
    int a = uf.find(i++);
    if (indices.count(a) == 0)
      indices[a] = j++;
    r[indices[a]].push_back(x);
  }
  return r;
}
