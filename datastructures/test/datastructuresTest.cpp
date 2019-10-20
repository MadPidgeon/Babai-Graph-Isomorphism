#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file

#include "datastructures.h"
#include "catch2/catch.hpp"
#include <RelationStructureToVectors.h>
#include <RelationalStructureTextRepresentation.h>

using namespace std;



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

TEST_CASE("Individualize") {

  // clang-format off
  RelationalStructure given = toRelationalStructure(
      {{0, 1, 1, 0},
       {1, 0, 1, 0},
       {1, 1, 0, 1},
       {0, 0, 1, 0}});
  // clang-format on

  const RelationalStructureTextRepresentation &textRepresentation =
      RelationalStructureTextRepresentation(given);
  SECTION("First node") {
    // clang-format off
    vector<vector<int>> expected = toVectors(
        {{1, 2, 2, 3},
         {2, 0, 2, 3},
         {2, 2, 0, 2},
         {3, 3, 2, 0}});
    // clang-format on
    given.individualise(vector<int>{1});
    const vector<std::vector<int>> &actual =
        textRepresentation.matrixOfBinaryStructure();
    REQUIRE(actual == expected);
  }

  SECTION("Second node") {
    // clang-format off
    vector<vector<int>> expected = toVectors(
        {{0, 2, 2, 3},
         {2, 1, 2, 3},
         {2, 2, 0, 2},
         {3, 3, 2, 0}});
    // clang-format on
    given.individualise(vector<int>{2});
    const vector<std::vector<int>> &actual =
        textRepresentation.matrixOfBinaryStructure();
    REQUIRE(actual == expected);
  }
  SECTION("Two nodes assigns uniq color for each") {
    // clang-format off
    vector<vector<int>> expected = toVectors(
        {{0, 3, 3, 4},
         {3, 1, 3, 4},
         {3, 3, 2, 3},
         {4, 4, 3, 0}});
    // clang-format on
    given.individualise(vector<int>{2, 3});
    const vector<std::vector<int>> &actual =
        textRepresentation.matrixOfBinaryStructure();
    REQUIRE(actual == expected);
  }
}

TEST_CASE("Refine") {

  // clang-format off
  RelationalStructure given = toRelationalStructure(
      {{0, 1, 1, 0},
       {1, 0, 1, 0},
       {1, 1, 0, 1},
       {0, 0, 1, 0}});
  // clang-format on
  const RelationalStructureTextRepresentation &givenRepresentation =
      RelationalStructureTextRepresentation(given);

  SECTION("Without individualization") {
    // clang-format off
    vector<vector<int>> expected = toVectors(
        {{1, 5, 3, 9},
         {5, 1, 3, 9},
         {4, 4, 0, 7},
         {8, 8, 6, 2}});
    // clang-format on
    given.refine();
    givenRepresentation.printMatrixOfBinaryStructure();
    const vector<std::vector<int>> &actual =
        givenRepresentation.matrixOfBinaryStructure();
    REQUIRE(actual == expected);
  }

  SECTION("twice should be idempotent") {
    // clang-format off
    vector<vector<int>> expected = toVectors(
        {{1, 5, 3, 9},
         {5, 1, 3, 9},
         {4, 4, 0, 7},
         {8, 8, 6, 2}});
    // clang-format on
    given.refine();
    given.refine();
    const vector<std::vector<int>> &actual =
        givenRepresentation.matrixOfBinaryStructure();
    REQUIRE(actual == expected);
  }
}