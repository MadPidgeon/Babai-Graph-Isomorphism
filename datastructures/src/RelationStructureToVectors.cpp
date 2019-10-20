//
// Created by Brutski_D on 20.10.2019.
//

#include "RelationStructureToVectors.h"
#include <numeric>

using namespace std;

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
  deque<int> vertices(size);
  iota(vertices.begin(), vertices.end(), 1);
  vector<int> relations = relationsList(adjacencyMatrix);
  RelationalStructure configuration(move(vertices), move(relations),
                                    2);
  return configuration;
}

RelationalStructure
toRelationalStructure(initializer_list<initializer_list<int>> adjacencyMatrix) {
  return toRelationalStructure(toVectors(adjacencyMatrix));
}
