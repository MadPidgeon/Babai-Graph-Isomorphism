#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file

#include "catch2/catch.hpp"
#include <RelationalStructureTextRepresentation.h>

using namespace std;

TEST_CASE("Binary RelationalStructure") {
  // clang-format off
  RelationalStructure given = toRelationalStructure(
      {{0, 1, 1, 0},
       {1, 0, 1, 0},
       {1, 1, 0, 1},
       {0, 0, 1, 0}});
  vector<vector<int>> expected = toVectors(
      {{0, 1, 1, 2},
       {1, 0, 1, 2},
       {1, 1, 0, 1},
       {2, 2, 1, 0}});
  // clang-format on
  const vector<std::vector<int>> &actual =
      RelationalStructureTextRepresentation(given).matrixOfBinaryStructure();
  REQUIRE(actual == expected);
}