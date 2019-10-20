//
// Created by Brutski_D on 20.10.2019.
//

#ifndef BABAI_GRAPH_ISOMORPHISM_RELATIONSTRUCTURETOVECTORS_H
#define BABAI_GRAPH_ISOMORPHISM_RELATIONSTRUCTURETOVECTORS_H

#include "datastructures.h"
#include <vector>
typedef std::vector<std::vector<int>> Matrix;

Matrix toVectors(std::initializer_list<std::initializer_list<int>> matrix);
std::vector<int> relationsList(const Matrix &adjacencyMatrix);
RelationalStructure toRelationalStructure(const Matrix &adjacencyMatrix);
RelationalStructure toRelationalStructure(
    std::initializer_list<std::initializer_list<int>> adjacencyMatrix);
#endif // BABAI_GRAPH_ISOMORPHISM_RELATIONSTRUCTURETOVECTORS_H
