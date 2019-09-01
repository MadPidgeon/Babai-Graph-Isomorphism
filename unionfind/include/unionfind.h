#pragma once
#include <iostream>
#include <vector>

class UnionFind {
	friend std::ostream& operator<<( std::ostream& os, const UnionFind& uf );
	int N;
	int* set;
	public:
	int find( int s ) const;
	bool cup( int, int );
	bool isUniform() const;
	template<typename T> T partitioning() const;
	void clear();
	UnionFind( int n );
	~UnionFind();
	UnionFind( const UnionFind& );
};

template<typename T> T UnionFind::partitioning() const {
	T P;
	std::vector<int> invs( N );
	for( int i = 0; i < N; i++ ) {
		int p = find( i );
		if( p == i ) {
			invs[i] = P.size();
			P.emplace_back();
		}
		P[invs[p]].push_back( i );
	}
	return P;
}
