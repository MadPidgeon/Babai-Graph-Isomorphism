#include <iostream>
#include "../permutation.h"
#include "../group.h"
#include "../action.h"

int main() {
	Group S12( new SymmetricGroup( 12 ) );
	NaturalSetAction A( S12, 12, 2 );
	auto S12_2 = A.anonymize();

	auto B = A.systemOfImprimitivity();
	Group G = B.anonymize();
	Group H = B.kernel();

	std::cout << G << std::endl;
	std::cout << H << std::endl;

	return 0;
}