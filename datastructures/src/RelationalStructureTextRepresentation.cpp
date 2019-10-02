//
// Created by Brutski_D on 01.10.2019.
//

#include "RelationalStructureTextRepresentation.h"
#include <cassert>
#include <cmath>

using namespace std;

void print(const vector<int> &ints) {
  printf("{");
  auto it = ints.begin();
  while (it + 1 != ints.end()) {
    printf("%d, ", *it);
    it++;
  }
  if (it != ints.end()) {
    printf("%d", *it);
  }
  printf("}");
}

size_t relationalStructureSize(const RelationalStructure &binaryStructure) {
  size_t result = 0;
  for (auto &relation : binaryStructure.relations()) {
    result += binaryStructure.relation(relation).size();
  }
  return result;
}

void RelationalStructureTextRepresentation::printRelations() const {
  for (auto &i : relationalStructure.relations()) {
    printf("%d\n", i);
    for (auto &tuple : relationalStructure.relation(i)) {
      print(tuple);
      printf("\n");
    }
  }
}
void RelationalStructureTextRepresentation::printMatrixOfBinaryStructure()
    const {
  const size_t arity = relationalStructure.arity();
  assert(arity == 2);
  size_t size = relationalStructureSize(relationalStructure);
  size_t dimensionalSize = sqrt(size);
  vector<vector<int>> matrixView(dimensionalSize);
  for (size_t i = 0; i < dimensionalSize; i++) {
    matrixView[i] = vector<int>(dimensionalSize);
  }
  for (auto &i : relationalStructure.relations()) {
    for (auto &tuple : relationalStructure.relation(i)) {
      matrixView[tuple[0] - 1][tuple[1] - 1] = i;
    }
  }
  printf("{");
  for (const auto &row : matrixView) {
    print(row);
    printf(",\n");
  }
  printf("}\n");
}
RelationalStructureTextRepresentation::RelationalStructureTextRepresentation(
    const RelationalStructure& relationalStructure) : relationalStructure(relationalStructure) {}
