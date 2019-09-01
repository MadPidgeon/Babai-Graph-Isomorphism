#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch2.h"
#include "group.h"
#include "luks.h"

TEST_CASE("String Isomorphism: Example 1") {
    Group S4( new SymmetricGroup( 4 ) );
    std::string x = "aaab";
    std::string y = "baaa";

    // example 1:
    Group G( new Subgroup( S4, { {3,1,2,0}, {0,2,1,3} } ) );
    REQUIRE( !StringIsomorphism( G, x, y ).isEmpty());
}

TEST_CASE("String Isomorphism: Example 2") {
    Group S4( new SymmetricGroup( 4 ) );
    std::string x = "aaab";
    std::string y = "baaa";

    // example 2:
    Group H( new Subgroup( S4, { {1,2,3,0} } ) );
    std::cout << StringIsomorphism( H, x, y ) << std::endl;
    REQUIRE( !StringIsomorphism( H, x, y ).isEmpty());
}