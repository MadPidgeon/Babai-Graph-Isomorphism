#pragma once

#include <deque>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <set>
#include <vector>

class all_permutations;

// describes a permutation of the elements {0,...,n-1}
class Permutation {
  friend class all_permutations;

  std::vector<int> _map;
  mutable int _order;

public:
  // returns the degree of the permutation
  int degree() const;

  // returns the order of the permutation
  int order() const;

  // checks whether the permutation is the identity
  bool isIdentity() const;

  // returns its representation in array notation
  const std::vector<int> &getArrayNotation() const;

  // returns its representation in cycle notation
  std::vector<std::vector<int>> getCycleNotation() const;

  // defines a lexicographical ordering on the permutations
  bool operator<(const Permutation &) const;
  bool operator==(const Permutation &) const;
  bool operator!=(const Permutation &) const;

  // defines multiplication of permutations
  Permutation &operator*=(const Permutation &);
  Permutation operator*(const Permutation &)const;

  // defines taking integer powers of permutations
  Permutation &operator^=(int);
  Permutation operator^(int) const;

  // returns the inverse of the permutation
  Permutation inverse() const;

  // returns the restriction of the permutation to the domain Delta
  // WARNING: undefined behaviour when Delta is not invariant under the
  // permutation
  Permutation project(const std::vector<int> &Delta) const;

  // defines the action of the permutation on the integers {0,...,n-1}
  int operator()(int) const;

  // constructs the identity permutation on n elements
  Permutation(int n);

  // constructs a permutation from array notation
  Permutation(std::vector<int> &&);
  Permutation(std::initializer_list<int>);
};

// iterable over all permutations of {0,...,n-1}
class all_permutations {
  int _n;

public:
  class iterator {
    int _n;
    Permutation _p;

  public:
    typedef iterator self_type;
    typedef Permutation value_type;
    typedef Permutation &reference;
    typedef Permutation *pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef size_t difference_type;
    iterator(int n);
    iterator(const self_type &other);
    self_type &operator++();
    self_type operator++(int);
    reference operator*();
    pointer operator->();
    bool operator==(const self_type &rhs);
    bool operator!=(const self_type &rhs);
  };
  iterator begin();
  iterator end();
  all_permutations(int n);
};

// print a permutation in cycle notation to an output stream
std::ostream &operator<<(std::ostream &os, const Permutation &cycles);