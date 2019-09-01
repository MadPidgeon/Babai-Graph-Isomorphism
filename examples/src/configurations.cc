#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "datastructures.h"
#include "matrix.h"
#include "ext.h"
#include "unionfind.h"
#include "design_lemma.h"

using std::vector;

int main() {
	// chang graph
	matrix<int> relational_matrix({
		{0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{2,0,2,2,2,2,2,2,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1},
		{2,2,0,1,2,2,2,1,2,2,1,1,1,1,2,1,2,1,2,1,2,2,1,1,1,1,1,1},
		{2,2,1,0,2,2,1,2,1,1,2,2,1,2,1,2,1,1,1,2,1,1,2,2,1,1,1,1},
		{2,2,2,2,0,2,1,1,1,1,2,2,1,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1},
		{2,2,2,2,2,0,1,1,1,2,1,2,1,1,1,2,2,1,1,1,1,2,1,2,1,1,1,2},
		{2,2,2,1,1,1,0,2,2,2,1,1,2,2,2,1,1,2,2,1,1,1,1,1,1,2,1,1},
		{2,2,1,2,1,1,2,0,2,1,2,1,2,2,1,2,1,2,1,2,1,1,1,1,2,1,1,1},
		{2,1,2,1,1,1,2,2,0,2,2,1,2,1,1,1,1,1,2,2,2,2,1,1,2,1,1,1},
		{2,1,2,1,1,2,2,1,2,0,1,2,2,1,1,1,1,1,2,1,1,2,1,2,1,2,1,2},
		{2,1,1,2,2,1,1,2,2,1,0,2,2,1,1,1,1,1,1,2,2,1,2,1,2,1,2,1},
		{2,1,1,2,2,2,1,1,1,2,2,0,2,1,1,1,1,1,1,1,1,1,2,2,1,2,2,2},
		{2,1,1,1,1,1,2,2,2,2,2,2,0,1,1,1,1,2,1,1,1,1,1,1,2,2,2,2},
		{1,2,1,2,1,1,2,2,1,1,1,1,1,0,2,2,1,2,2,2,1,1,2,2,1,2,1,1},
		{1,2,2,1,2,1,2,1,1,1,1,1,1,2,0,1,2,2,2,1,2,1,2,1,1,2,2,1},
		{1,2,1,2,1,2,1,2,1,1,1,1,1,2,1,0,2,2,1,2,1,2,1,2,2,1,1,2},
		{1,2,2,1,2,2,1,1,1,1,1,1,1,1,2,2,0,2,1,1,2,2,1,1,2,1,2,2},
		{1,2,1,1,1,1,2,2,1,1,1,1,2,2,2,2,2,0,1,1,1,1,1,1,2,2,2,2},
		{1,1,2,1,1,1,2,1,2,2,1,1,1,2,2,1,1,1,0,2,2,2,2,2,1,2,1,1},
		{1,1,1,2,1,1,1,2,2,1,2,1,1,2,1,2,1,1,2,0,2,2,2,2,2,1,1,1},
		{1,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,2,1,2,2,0,2,2,1,2,1,2,1},
		{1,1,2,1,1,2,1,1,2,2,1,1,1,1,1,2,2,1,2,2,2,0,1,2,2,1,1,2},
		{1,1,1,2,2,1,1,1,1,1,2,2,1,2,2,1,1,1,2,2,2,1,0,2,1,2,2,1},
		{1,1,1,2,1,2,1,1,1,2,1,2,1,2,1,2,1,1,2,2,1,2,2,0,1,2,1,2},
		{1,1,1,1,1,1,1,2,2,1,2,1,2,1,1,2,2,2,1,2,2,2,1,1,0,1,2,2},
		{1,1,1,1,1,1,2,1,1,2,1,2,2,2,2,1,1,2,2,1,1,1,2,2,1,0,2,2},
		{1,1,1,1,2,1,1,1,1,1,2,2,2,1,2,1,2,2,1,1,2,1,2,1,2,2,0,2},
		{1,1,1,1,1,2,1,1,1,2,1,2,2,1,1,2,2,2,1,1,1,2,1,2,2,2,2,0}
	});
	std::deque<int> vertices( 28 );
	std::iota( vertices.begin(), vertices.end(), 1 );
	vector<int> relations(28*28);
	for( auto t : all_tuples( 28, 2 ) )
		relations[ polynomial_evaluation( t, 28 ) ] = relational_matrix.at( t[0], t[1] );
	RelationalStructure configuration( std::move( vertices ), std::move( relations ), 2 );
	configuration.refine();

	std::cout << configuration << std::endl;
	std::cout << std::boolalpha;
	std::cout << configuration.isHomogeneous() << std::endl;
	std::cout << configuration.isClique() << std::endl;
	std::cout << configuration.isUPCC() << std::endl;

	std::cout << "-------------------------------------" << std::endl;
	std::deque<int> vertices2( 4 );
	std::iota( vertices2.begin(), vertices2.end(), 1 );
	vector<int> relations2(4*4*4);
	for( auto t : all_tuples(4,3) ) {
		if( t[0] == t[1] and t[1] == t[2] )
			relations2[ polynomial_evaluation( t, 4 ) ] = 0;
		else if( t[0] == t[1] or t[1] == t[2] or t[0] == t[2] )
			relations2[ polynomial_evaluation( t, 4 ) ] = 1;
		else if( t[0] < 3 and t[1] < 3 and t[2] < 3 )
			relations2[ polynomial_evaluation( t, 4 ) ] = 2;
		else {
			int i = (t[1] == 3) + 2*(t[2] == 3);
			int a = t[ t[0] == 3 ];
			int b = t[ 2-( t[2] == 3 ) ];
			int r = ( (a - b + 3)%3 == 1 ) + 2*i;
			relations2[ polynomial_evaluation( t, 4 ) ] = r+3;
		}
	}
	RelationalStructure configuration2( std::move( vertices2 ), std::move( relations2 ), 3 );
	std::cout << configuration2 << std::endl << std::endl;
	configuration2.refine();
	std::cout << configuration2 << std::endl << std::endl;
	std::cout << configuration2.isHomogeneous() << std::endl;
	std::cout << configuration2.isClique() << std::endl;
	std::cout << configuration2.isUPCC() << std::endl;

	std::cout << DesignLemma( configuration2, configuration2, .66666666, []( Either<RelationalStructure,ColouredPartition> A, Either<RelationalStructure,ColouredPartition> B ) {
		if( B.isFirst() )
			std::cout << "relational structure: " << A.getFirst() << "~?~" << B.getFirst() << std::endl;
		else
			std::cout << "coloured 2/3-partition: " << A.getSecond().getMapping() << "~?~" << B.getSecond().getMapping() << std::endl;
		return Empty();
	} ) << std::endl;


	return 0;
}