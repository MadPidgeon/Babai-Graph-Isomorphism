#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file

#include "catch2/catch.hpp"
#include <RelationStructureToVectors.h>
#include <RelationalStructureTextRepresentation.h>
#include <design_lemma.h>

using namespace std;

TEST_CASE("Design lemma for 4 node graph") {
  // clang-format off
  RelationalStructure given = toRelationalStructure(
      {{0, 1, 1, 0},
       {1, 0, 1, 0},
       {1, 1, 0, 1},
       {0, 0, 1, 0}});
  
  const Iso &iso = DesignLemma(
      given, given, .66666666,
      [](Either<RelationalStructure, ColouredPartition> A,
         Either<RelationalStructure, ColouredPartition> B) {
        if (B.isFirst())
          std::cout << "relational structure: " << A.getFirst()
                    << "~?~" << B.getFirst() << std::endl;
        else
          std::cout << "coloured 2/3-partition: "
                    << A.getSecond().getMapping() << "~?~"
                    << B.getSecond().getMapping() << std::endl;
        return Empty();
      });
  // clang-format on
  cout << iso;
}

TEST_CASE("Design lemma for 4 node permuted graph") {
  // clang-format off
  RelationalStructure given = toRelationalStructure(
      {{0, 1, 0, 0},
       {1, 0, 1, 1},
       {0, 1, 0, 1},
       {0, 1, 1, 0}});
  const Iso &iso = DesignLemma(
      given, given, .66666666,
      [](Either<RelationalStructure, ColouredPartition> A,
         Either<RelationalStructure, ColouredPartition> B) {
        if (B.isFirst())
          std::cout << "relational structure: " << A.getFirst()
                    << "~?~" << B.getFirst() << std::endl;
        else
          std::cout << "coloured 2/3-partition: "
                    << A.getSecond().getMapping() << "~?~"
                    << B.getSecond().getMapping() << std::endl;
        return Empty();
      });
  // clang-format on
  cout << iso;
}