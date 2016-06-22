#include <deque>
#include <vector>
#include <string>

#include "luks.h"
#include "group.h"
#include "action.h"
#include "coset.h"
#include "fhl.h"
#include "permutation.h"
#include "cameron.h"

// defines an action on strings
string stringAction( const Permutation& sigma, const string& x ) {
	string y( x.size(), ' ' );
	for( size_t i = 0; i < x.size(); ++i )
		y[sigma(i)] = x[i];
	return y;
}

// computes the G-isomorphisms from x to y
Iso StringIsomorphism( Group G, string x, string y ) {
	#ifdef DEBUG
	std::cout << "StringIsomorphism(" << G->generators() << "," << x << "," << y << "):" << std::endl;
	#endif

	bool isAut = true;
	for( auto sigma : G->generators() ) {
		if( stringAction( sigma, x ) != x ) {
			isAut = false;
			break;
		}
	}
	if( isAut ) {
		if( x == y )
			return Coset( G, G, G->one(), false );
		else
			return Empty();
	} else 
		return StringIsomorphismNonAutomorphism( G, x, y );
}

// computes the G-isomorphisms from x to y if G is not a subset of Aut(X)
Iso StringIsomorphismNonAutomorphism( Group G, string x, string y ) {
	#ifdef DEBUG
	std::cout << "StringIsomorphismNonautomorphism:" << std::endl;
	#endif

	NaturalAction A( G );
	auto Delta = A.orbits();
	if( Delta.size() == 1 )
		return StringIsomorphismTransitive( G, x, y );
	else
		return ChainRule( G, x, y, Delta, StringIsomorphism );
}

double cameron_bound( double m ) {
	return std::exp2( 7 * std::log2(m) * std::log2(m) * std::log2(std::log2(m)) );
}

// computes the G-isomorphisms from x to y if G is transitive
Iso StringIsomorphismTransitive( Group G, string x, string y ) {
	#ifdef DEBUG
	std::cout << "StringIsomorphismTransitive( " << G->generators() << "," << x << "," << y << "):" << std::endl;
	#endif

	NaturalAction C( G );
	auto A = C.systemOfImprimitivity();
	auto B = A.domain();
	int m = B.size();
	auto H = A.anonymize();
	if( H->degree() <= 24 or H->order() < cameron_bound( m ) ) { 
		auto N = A.kernel();
		return WeakReduction( G, N, x, y, StringIsomorphism );
	} else {
		return StringIsomorphismCameronGroup( A, H, x, y );
	}
}

// computes G-isomorphisms assuming H is a Cameron group
Iso StringIsomorphismCameronGroup( RestrictedNaturalSetAction A, Group H, string x, string y ) {
	#ifdef DEBUG
	std::cout << "StringIsomorphismCameronGroup( " << H->generators() << "," << x << "," << y << "):" << std::endl;
	#endif

	return CameronIdentification( A, x, y, Empty() );
}