#pragma once
#include <deque>
#include <string>
#include <vector>

#include "action.h"
#include "coset.h"
#include "fhl.h"
#include "group.h"

using std::string;

string stringAction(const Permutation &sigma, const string &x);
Iso StringIsomorphism(Group G, string x, string y);
Iso StringIsomorphismNonAutomorphism(Group G, string x, string y);
Iso StringIsomorphismTransitive(Group G, string x, string y);
Iso StringIsomorphismCameronGroup(RestrictedNaturalSetAction A, Group H,
                                  string x, string y);

template <typename T> string stringRestrict(const string &x, const T &Delta) {
  string y;
  y.reserve(Delta.size());
  for (int i : Delta)
    y.push_back(x[i]);
  return y;
}

template <typename T> Iso ShiftIdentity(Coset C, string x, string y, T f);
template <typename T>
Iso WeakReduction(Group G, Group H, string x, string y, T f);
template <typename T>
Iso ChainRule(Group G, string x, string y, std::vector<std::vector<int>> orbits,
              T f);

// applies the shift identity to the result of f
template <typename T> Iso ShiftIdentity(Coset C, string x, string y, T f) {
  return C.representative() *
         f(C.subgroup(), x, stringAction(C.representative().inverse(), y));
}

// applies weak reduction from G to H
template <typename T>
Iso WeakReduction(Group G, Group H, string x, string y, T f) {
#ifdef DEBUG
  std::cout << "WeakReduction( " << G->generators() << "," << H->generators()
            << "," << x << "," << y << "):" << std::endl;
#endif

  IsoJoiner J;
  for (Coset C : G->allCosets(H))
    J.join(ShiftIdentity(C, x, y, f));
  return Iso(J);
}

// applies the chain rule to the orbits
template <typename T>
Iso ChainRule(Group G, string x, string y, std::vector<std::vector<int>> orbits,
              T f) {
#ifdef DEBUG
  std::cout << "StringIsomorphismChainRule( " << G->generators() << "," << x
            << "," << y << "," << orbits << "):" << std::endl;
#endif

  auto mu = G->one();
  Group F = G;
  for (const auto &Delta : orbits) {
    // get generators
    auto perm = F->generators();
    std::deque<int> almostDelta(Delta.begin(), Delta.end());

    // project onto orbit
    for (auto &sigma : perm)
      sigma = sigma.project(Delta);
    Group H(new Subgroup(Group(new SymmetricGroup(Delta.size())), perm));

    // apply procedure to orbit
    Iso I = f(H, stringRestrict(x, Delta), stringRestrict(y, Delta));

    // invert projection
    if (I.isEmpty())
      return Empty();
    PullbackStructure P(F, perm, F->generators());
    auto tau = P(I.coset().representative());
    auto perm2 = I.coset().subgroup()->generators();
    std::deque<Permutation> perm3;
    for (auto &sigma : perm2)
      perm3.push_back(P(sigma));
    RestrictedNaturalAction A(F, almostDelta);
    Group J = A.kernel();

    // update F, y and mu
    F = J->join(std::move(perm3));
    y = stringAction(tau.inverse(), y);
    mu = mu * tau;
  }

  // return result
  Coset R(G, F, mu, false);
  return R;
}