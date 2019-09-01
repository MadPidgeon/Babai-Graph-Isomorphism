#include <string>

#include "luks.h"

int main() {
	Group S4( new SymmetricGroup( 4 ) );
	std::string x = "aaab";
	std::string y = "baaa";

	// example 1: 
	Group G( new Subgroup( S4, { {3,1,2,0}, {0,2,1,3} } ) );
	std::cout << StringIsomorphism( G, x, y ) << std::endl;

	std::cout << "-------------------------------------" << std::endl;
	// example 2:
	Group H( new Subgroup( S4, { {1,2,3,0} } ) );
	std::cout << StringIsomorphism( H, x, y ) << std::endl;
	// note: the output is a coset representative and a list of generators for a group.
	// if the list of generators is empty, it means the group is trivial, not empty.

	return 0;
}

