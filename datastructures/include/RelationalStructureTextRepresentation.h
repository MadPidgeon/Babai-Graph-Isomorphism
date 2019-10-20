//
// Created by Brutski_D on 01.10.2019.
//

#ifndef BABAI_GRAPH_ISOMORPHISM_RELATIONALSTRUCTURETEXTREPRESENTATION_H
#define BABAI_GRAPH_ISOMORPHISM_RELATIONALSTRUCTURETEXTREPRESENTATION_H

#include "datastructures.h"
class RelationalStructureTextRepresentation {
  const RelationalStructure &relationalStructure;

public:
  explicit RelationalStructureTextRepresentation(
      const RelationalStructure &relationalStructure);
  void printRelations() const;
  void printMatrixOfBinaryStructure() const;
  std::vector<std::vector<int>> matrixOfBinaryStructure() const;
};

#endif // BABAI_GRAPH_ISOMORPHISM_RELATIONALSTRUCTURETEXTREPRESENTATION_H
