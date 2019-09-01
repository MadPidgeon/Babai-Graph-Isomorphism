#include "action.h"
#include "coset.h"
#include "datastructures.h"

ColouredPartition ImprimitiveToColouring( RelationalStructure X1, RelationalStructure X2, std::deque<std::deque<int>> part ) {
	std::deque<std::deque<int>> A;
	std::deque<int> B;
	for( std::deque<int> p : part ) {
		for( int& x : p )
			x = X2.decode( x );
		A.push_back( std::move(p) );
	}
	for( int x : X1.domain() )
		if( std::find( X2.domain().begin(), X2.domain().end(), x ) == X2.domain().end() )
			B.push_back( x );
	return ColouredPartition( { A, {B} } );
}

template<typename T>
Iso DesignLemma( RelationalStructure X, RelationalStructure Y, double alpha, T f ) {
	int n = X.domain().size();
	int k = X.arity();
	IsoJoiner J;
	for( int i = 0; i < k; ++i ) {
		for( const auto Sx : all_different_tuples( n, i ) ) {
			std::cout << Sx << std::endl;
			RelationalStructure X2 = X;
			X2.c_individualise( Sx );
			X2.refine();
			int cx = X2.getNonAlphaPartition( alpha );
			if( cx == -1 ) {
				ColouredPartition Cx = (ColouredSet) X2;
				// Cx.equipartition();
				std::cout << i << std::endl;
				for( const auto Sy : all_different_tuples( n, i ) ) {
					RelationalStructure Y2 = Y;
					Y2.c_individualise( Sy );
					Y2.refine();
					if( Y2.getNonAlphaPartition( alpha ) == -1 ) {
						ColouredPartition Cy = (ColouredSet) Y2;
						// Cy.equipartition();
						J.join( f( Cx, Cy ) );
					}
				}
				return Iso( J );
			}
			RelationalStructure X3 = X2.skeletalSubstructure( 2, X2.relation( cx ).vertices() );
			auto P = X3.witnessOfImprimitivity();
			if( P.first > 0 ) {
				ColouredPartition Cx = ImprimitiveToColouring( X, X3, P.second.template partitioning<std::deque<std::deque<int>>>() );
				// Cx.equipartition();
				for( const auto Sy : all_different_tuples( n, i ) ) {
					RelationalStructure Y2 = Y;
					Y2.individualise( Sy );
					Y2.refine();
					int cy = Y2.getNonAlphaPartition( alpha );
					if( cy >= 0 ) {
						RelationalStructure Y3 = Y2.skeletalSubstructure( 2, Y2.relation( cy ).vertices() );
						auto Q = Y3.witnessOfImprimitivity();
						if( Q.first > 0 ) {
							ColouredPartition Cy = ImprimitiveToColouring( Y, Y3, Q.second.template partitioning<std::deque<std::deque<int>>>() );
							// Cy.equipartition();
							J.join( f( Cx, Cy ) );
						}
					}
				}
				return Iso( J );
			} else if( not X3.isClique() ) {
				for( const auto Sy : all_different_tuples( n, i ) ) {
					RelationalStructure Y2 = Y;
					Y2.individualise( Sy );
					Y2.refine();
					int cy = Y2.getNonAlphaPartition( alpha );
					if( cy >= 0 ) {
						RelationalStructure Y3 = Y2.skeletalSubstructure( 2, Y2.relation( cy ).vertices() );
						if( Y3.isUPCC() )
							J.join( f( X3, Y3 ) );
					}
				}
				return Iso( J );
			}
		}
	}
	throw std::range_error("Insufficient symmetry defect!");
}