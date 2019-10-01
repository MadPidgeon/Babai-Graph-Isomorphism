#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file

#include "datastructures.h"
#include "catch2/catch.hpp"
#include <RelationalStructureTextRepresentation.h>

using namespace std;

typedef vector<vector<int>> Matrix;

Matrix toVectors(initializer_list<initializer_list<int>> matrix) {
  Matrix result(matrix.size());
  int i = 0;
  initializer_list<initializer_list<int>>::const_iterator row = matrix.begin();
  while (row != matrix.end()) {
    result[i] = *row;
    row++;
    i++;
  }
  return result;
}

vector<int> relationsList(const Matrix &adjacencyMatrix) {
  size_t size = adjacencyMatrix.size();
  vector<int> relations(size * size);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      vector<int> t({(int)i, (int)j});
      if (i == j) {
        relations[polynomial_evaluation(t, size)] = 0;
      } else if (adjacencyMatrix[i][j] != 0) {
        relations[polynomial_evaluation(t, size)] = 1;
      } else {
        relations[polynomial_evaluation(t, size)] = 2;
      }
    }
  }
  return relations;
}

RelationalStructure toRelationalStructure(const Matrix &adjacencyMatrix) {
  int size = adjacencyMatrix.size();
  std::deque<int> vertices(size);
  iota(vertices.begin(), vertices.end(), 1);
  vector<int> relations = relationsList(adjacencyMatrix);
  RelationalStructure configuration(std::move(vertices), std::move(relations),
                                    2);
  return configuration;
}

RelationalStructure
toRelationalStructure(initializer_list<initializer_list<int>> adjacencyMatrix) {
  return toRelationalStructure(toVectors(adjacencyMatrix));
}

TEST_CASE("Relations list") {
  // clang-format off
  const vector<int> &actual = relationsList(
      {{0, 1, 1, 0},
       {1, 0, 1, 0},
       {1, 1, 0, 1},
       {0, 0, 1, 0}});
  // clang-format on
  const vector<int> expected{0, 1, 1, 2, 1, 0, 1, 2, 1, 1, 0, 1, 2, 2, 1, 0};
  REQUIRE(actual.size() == 16);
  REQUIRE(actual == expected);
  //  REQUIRE(given.relations().)
  //  REQUIRE(given.relation())
}

TEST_CASE("Binary RelationalStructure") {
  // clang-format off
  RelationalStructure given = toRelationalStructure(
      {{0, 1, 1, 0},
       {1, 0, 1, 0},
       {1, 1, 0, 1},
       {0, 0, 1, 0}});
  // clang-format on
  RelationalStructureTextRepresentation(given).printMatrixOfBinaryStructure();
  REQUIRE(given.arity() == 2);
}