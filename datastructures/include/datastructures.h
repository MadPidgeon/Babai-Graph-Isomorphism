#pragma once
#include "ext.h"
#include "unionfind.h"
#include <deque>
#include <unordered_map>
#include <vector>

typedef std::deque<std::deque<int>> regular_mapping_t;
typedef std::unordered_map<int, int> inverse_mapping_t;

// coloured set
class ColouredSet {
  regular_mapping_t mapping;
  inverse_mapping_t inverse_mapping;

public:
  // returns the colour of x
  int getColour(int x) const;

  // returns all elements of the colour i
  const std::deque<int> &getColourClass(int i) const;

  // moves each element in S to a singleton colour class canonically
  template <typename T> void individualise(T S);

  // returns the bare-bones structure
  const regular_mapping_t &getMapping() const;

  // returns the colouring induced on the subsets elms
  ColouredSet substructure(std::deque<int> elms) const;

  // returns an iterable over the range of colours
  range colours() const;

  // constructor
  ColouredSet(std::deque<std::deque<int>>);
  ColouredSet() = default;
  ColouredSet(ColouredSet &&);
  ColouredSet &operator=(ColouredSet &&) = default;
  ColouredSet(const ColouredSet &) = default;
};

// coloured (equi-)partition
class ColouredPartition {
  std::deque<std::deque<std::deque<int>>> partition_mapping;
  std::unordered_map<int, std::pair<int, int>> inverse_partition_mapping;

public:
  // checks whether it is a coloured equi-partition
  bool is_equipartition() const;

  // refines the structure to a coloured equi-partition canonically
  void equipartition();

  // returns the bare-bones structure
  const std::deque<regular_mapping_t> &getMapping() const;

  // constructor
  ColouredPartition(std::initializer_list<std::deque<std::deque<int>>> L);
  ColouredPartition(ColouredSet);
  ColouredPartition(ColouredPartition &&) = default;
  ColouredPartition(const ColouredPartition &) = default;

  // cast to a coloured set
  explicit operator ColouredSet() const;
};

// hypergraph
class Hypergraph {
  std::deque<int> Omega;
  std::map<std::vector<int>, int> E; // edge colouring
public:
  // returns -1 if the hypergraph is not uniform, otherwise it returns the
  // degree
  int uniformityDegree() const;

  // constructor
  Hypergraph(std::deque<int> vertices, std::map<std::vector<int>, int> edges);
  Hypergraph(std::map<std::vector<int>, int> edges);

private:
  // computes Omega from E
  void gatherVertices();
};

// coloured bipartite graph
class ColouredBipartiteGraph {
public:
  enum side { LEFT = 0, RIGHT = 1 };
  typedef std::unordered_map<int, std::deque<int>>
      parameter; // only one direction ness
private:
  std::deque<int> Omega[2];
  std::unordered_map<int, std::pair<side, int>> agemO;
  std::deque<std::deque<int>> nbh[2];
  ColouredSet cs;
  int n1, n2;

public:
  // returns the partitioning into twin classes
  std::deque<std::deque<int>> twins(side s) const;

  // returns the symmetry defect
  double symmetryDefect(side s) const;

  // returns the colour of x
  int colour(int x) const;

  // returns the set of vertices at side s
  const std::deque<int> &vertices(side s) const;

  // returns the side of x
  side getSide(int x) const;

  // returns the neighbouring vertices of v
  std::deque<int> neighborhood(int v) const;

  // checks whether there is an edge between v and w, where v is a left vertex
  // and w a right vertex
  bool hasEdge(int v, int w) const;

  // return the neighbourhood hypergraph of the left vertices
  Hypergraph neighborhoodHypergraph() const;

  // returns the subgraph induced on vertex set W1 x W2
  ColouredBipartiteGraph substructure(std::deque<int> W1,
                                      std::deque<int> W2) const;

  // constructor
  ColouredBipartiteGraph(std::deque<int> V1, std::deque<int> V2, parameter E,
                         ColouredSet c);
  ColouredBipartiteGraph(std::deque<int> V1, std::deque<int> V2, parameter E);
  ColouredBipartiteGraph(ColouredBipartiteGraph &&) = default;
  ColouredBipartiteGraph(const ColouredBipartiteGraph &) = default;
  ColouredBipartiteGraph &operator=(ColouredBipartiteGraph &&);
  ColouredBipartiteGraph &operator=(const ColouredBipartiteGraph &) = default;

  // cast to coloured set
  explicit operator ColouredSet() const;

private:
  // execute operations as above on encoded data
  bool c_hasEdge(int x, int y) const;
  bool c_areTwins(side s, int x, int y) const;
  const std::deque<int> &c_neighborhood(side s, int x) const;
  int decode(side s, int x) const;
  std::pair<side, int> encode(int x) const;
};

// configuration
class RelationalStructure {
  std::deque<int> Omega;
  std::unordered_map<int, int> agemO;
  size_t k;           // arity
  std::vector<int> r; // k-dim relation matrix
  // int n;
  int colour_relations_end;
  int all_relations_end;
  struct Relation : public std::deque<std::vector<int>> {
    std::deque<int> vertices() const;
  };

public:
  // returns the arity of the relational structure
  size_t arity() const;

  std::vector<int> shape() const;

  // returns the domain (Omega) of the relational structure
  const std::deque<int> &domain() const;

  // returns a range of vertex colour indices (primary colours)
  range vertexColours() const;

  // returns a range of relation indices
  range relations() const;

  // returns a range of edge colours (non-primary colours)
  range edgeColours() const;

  // returns all tuples in relation i
  Relation relation(size_t i) const;

  // checks whether the relational structure is homogeneous
  bool isHomogeneous() const;

  // checks whether the binary configuration is homogeneous
  // WARNING: undefined behaviour if the configuration is not binary
  bool isPrimitive() const;

  // returns a relation index which is not a connected induced graph, together
  // with the connected components returns (-1,Undefined) if no such component
  // exists
  std::pair<int, UnionFind> witnessOfImprimitivity() const;

  // checks whether the structure is a clique
  bool isClique() const;

  // checks whether the binary structure is a UPCC
  bool isUPCC() const;

  // moves elements in S to a singleton colour-class canonically
  template <typename T> void individualise(T S);
  template <typename T> void c_individualise(T S);

  // return the index of the colour-class that contains (x,...,x)
  int vertexColour(int x) const;

  // returns the index of the colour-class that contains (a,b)
  // WARNING: undefined behaviour if the configuration is not binary
  int edgeColour(int a, int b) const;

  // returns the coloured bipartite graph on V1 x V2 induced by relation r
  ColouredBipartiteGraph inducedBipartiteGraph(std::deque<int> &&V1,
                                               std::deque<int> &&V2,
                                               int r) const;

  // construct a relational structure from a coloured bipartite graph
  RelationalStructure(const ColouredBipartiteGraph &G);

  // cast to a coloured set
  explicit operator ColouredSet() const;

  // apply Weisfeiler-Lehman refinement
  int refine();

  // returns the t-skeleton
  RelationalStructure skeleton(size_t t) const;

  // returns the t-skeleton of the substructure induced on C
  RelationalStructure skeletalSubstructure(size_t t, std::deque<int> C) const;

  // returns the twin equivalence classes in i
  // ???????????????????????????????????????????????????????????????????????//
  // std::deque<std::deque<int>> twins( int i ) const;

  // returns -1 if all colour classes are of size at most alpha |Omega|,
  // otherwise returns a violating colour class index
  int getNonAlphaPartition(double alpha);

  // constructor
  RelationalStructure(std::deque<int> &&C, std::vector<int> &&r, int k);

  // returns colour of encoded edge (x,y)
  int c_edgeColour(int x, int y) const;

  // encode element of Omega
  int encode(int x) const;

  // decode element to Omega
  int decode(int y) const;

private:
  // change relation indices to the form {0,...,r}
  void normalise();

  // returns colour of encoded vertex (x,...,x)
  int &c_vertexColour(int x);

  // applies iteration of Weisfeiler-Lehman refinement
  bool WeisfeilerLehman();
};

struct JohnsonScheme : all_tuples {
  std::deque<int> mapping; // the n-th subset of Gamma as measured by
                           // all_ordered_tuples mapping to V2
  std::map<int, int>
      inverse_mapping; // the elements of V2 mapping to the n-th subset of Gamma
  JohnsonScheme(int m, int t);
  std::map<std::vector<int>, int> completeMapping() const;
};

std::ostream &operator<<(std::ostream &os, const ColouredBipartiteGraph &G);
std::ostream &operator<<(std::ostream &os, const RelationalStructure &X);

template <typename T> void ColouredSet::individualise(T S) {
  for (int x : S) {
    int c = getColour(x);
    if (mapping.size() > 1) {
      mapping[c].erase(std::find(mapping[c].begin(), mapping[c].end(), x));
      inverse_mapping[x] = mapping.size();
      mapping.emplace_back(std::deque<int>({x}));
    }
  }
}

template <typename T> void RelationalStructure::individualise(T S) {
  for (auto &x : S)
    x = encode(x);
  c_individualise(S);
}

template <typename T> void RelationalStructure::c_individualise(T S) {
  int add = all_relations_end;
  for (int x : S) {
    std::vector<int> x_vec(arity(), x);
    size_t off = polynomial_evaluation(x_vec, domain().size());
    r[off] += add++;
  }
  normalise();
}