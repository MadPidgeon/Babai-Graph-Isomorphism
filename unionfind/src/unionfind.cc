#include <iostream>

#include "unionfind.h"

int UnionFind::find( int s ) const {
	int i = s, r = s;
	while( set[r] != r )
		r = set[r];
	while( set[s] != s ) {
		i = set[s];
		set[s] = r;
		s = i;
	}
	return s;
}

bool UnionFind::cup( int a, int b ) {
	int y = find(b);
	int x = find(a);
	if( y < x )
		std::swap( x, y );
	bool r = y != x;
	set[ y ] = x;
	return r;
}

void UnionFind::clear() {
	for( int i = 0; i < N; ++i )
		set[i] = i;
}

bool UnionFind::isUniform() const {
	for( int i = 0; i < N; ++i )
		if( find( i ) != 0 )
			return false;
	return true;
}

UnionFind::UnionFind( int n ) {
	N = n;
	set = new int[N];
	clear();
}

UnionFind::UnionFind( const UnionFind& other ) {
	N = other.N;
	set = new int[N];
	for( int i = 0; i < N; ++i )
		set[i] = other.set[i];
}

UnionFind::~UnionFind() {
	delete [] set;
}

std::ostream& operator<<( std::ostream& os, const UnionFind& uf ) {
	os << "{" << std::endl;
	for( int i = 0; i < uf.N; ++i )
		os << "   [" << i << "] " << int(uf.set[i]) << std::endl;
	os << "}" << std::endl;
	return os;
}
