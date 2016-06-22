#include <vector>
#include <initializer_list>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <ext.h>

#include "permutation.h"


int Permutation::degree() const {
	return _map.size();
}

bool Permutation::isIdentity() const {
	for( int i = 0; i < degree(); i++ )
		if( (*this)(i) != i )
			return false;
	return true;
}

int Permutation::order() const {
	if( _order != -1 )
		return _order;
	Permutation p = (*this);
	_order = 1;
	while( !p.isIdentity() ) {
		p *= (*this);
		_order++;
	}
	return _order;
}

Permutation Permutation::project( const std::vector<int>& Delta ) const {
	std::vector<int> mapping( degree(), -1 );
	for( size_t i = 0; i < Delta.size(); ++i )
		mapping[ Delta[i] ] = i;
	std::vector<int> perm( Delta.size() );
	for( size_t i = 0; i < Delta.size(); ++i )
		perm[i] = mapping[ (*this)( Delta[i] ) ];
	return Permutation( std::move( perm ) );
}

bool Permutation::operator<( const Permutation& other ) const {
	if( degree() != other.degree() )
		throw std::range_error( "Permutations not compatible" );
	for( int i = 0; i < degree(); i++ )
		if( (*this)(i) < other(i) )
			return true;
		else if( (*this)(i) > other(i) )
			return false;
	return false;
}

bool Permutation::operator==( const Permutation& other ) const {
	if( degree() != other.degree() )
		throw std::range_error( "Permutations not compatible" );
	return _map == other._map;
}

bool Permutation::operator!=( const Permutation& other ) const {
	return !( *this == other);
}

int Permutation::operator()( int k ) const {
	return _map[k];
}

const std::vector<int>& Permutation::getArrayNotation() const {
	return _map;
}

std::vector<std::vector<int>> Permutation::getCycleNotation() const {
	std::vector<std::vector<int>> cycles;
	std::vector<bool> done( degree(), false );
	for( int i = 0; i < degree(); i++ ) {
		if( not done[i] ) {
			int j = i;
			std::vector<int> cycle;
			do {
				done[j] = true;
				cycle.push_back( j );
				j = (*this)(j);
			} while( i != j );
			if( cycle.size() != 1 )
				cycles.emplace_back( std::move( cycle ) );
			else
				cycle.clear();
		}
	}
	return cycles;
}

Permutation Permutation::operator*( const Permutation& sigma ) const {
	if( degree() != sigma.degree() )
		throw std::range_error( "Permutations not compatible" );
	std::vector<int> v( degree() );
	for( int i = 0; i < degree(); i++ )
		v[i] = (*this)(sigma(i));
	return Permutation( std::move(v) );
}

Permutation& Permutation::operator*=( const Permutation& sigma ) {
	Permutation p = (*this) * sigma;
	_map = move( p._map );
	_order = -1;
	return *this;
}

Permutation Permutation::operator^( int k ) const {
	Permutation p = (*this);
	std::vector<int> v( degree() );
	for( int i = 0; i < degree(); i++ )
		v[i] = i;
	Permutation t( std::move( v ) );
	if( k < 0 ) {
		p = p.inverse();
		k = -k;
	}
	for( int i = 0; i < k; i++ )
		t *= p;
	return t;
}

Permutation& Permutation::operator^=( int k ) {
	Permutation p = (*this) ^ k;
	_map = move( p._map );
	_order = -1;
	return *this;
}

Permutation Permutation::inverse() const {
	std::vector<int> v( degree() );
	for( int i = 0; i < degree(); i++ )
		v[ (*this)(i) ] = i;
	return Permutation( std::move( v ) );
}

Permutation::Permutation( int n ) : _order( 1 ) {
	if( n <= 0 )
		n = 0;
	_map.resize( n );
	for( int i = 0; i < n; i++ )
		_map[i] = i;
}

Permutation::Permutation( std::vector<int>&& m ) : _map( m ), _order( -1 )  {
}

Permutation::Permutation( std::initializer_list<int> l ) : _map( l ), _order( -1 ) {
}

std::ostream& operator<<( std::ostream& os, const Permutation& sigma ) {
	auto cycles = sigma.getCycleNotation();
	if( cycles.size() == 0 )
		return os << "()";
	else for( auto& cycle : cycles ) {
		os << "( ";
		for( int x : cycle )
			os << x << " ";
		os << ")";
	}
	return os;
}

// ----------------------------------------------------------------------------

all_permutations::iterator::iterator( int n ) : _n(n), _p(_n) {
}

all_permutations::iterator::iterator( const self_type& other ) : _n( other._n ), _p( other._p ) {
}

all_permutations::iterator::self_type all_permutations::iterator::operator++(int) { 
	self_type i = *this; 
	++(*this); 
	return i; 
}

all_permutations::iterator::self_type& all_permutations::iterator::operator++() {
	if( !std::next_permutation( _p._map.begin(), _p._map.end() ) )
		_n = -1;
	return *this;
}

all_permutations::iterator::reference all_permutations::iterator::operator*() { 
	return _p; 
}

all_permutations::iterator::pointer all_permutations::iterator::operator->() { 
	return &_p; 
}

bool all_permutations::iterator::operator==(const self_type& rhs) { 
	return _n == rhs._n && _p == rhs._p;; 
}

bool all_permutations::iterator::operator!=(const self_type& rhs) { 
	return _n != rhs._n || _p != rhs._p; 
}

all_permutations::iterator all_permutations::begin() { 
	return iterator( _n ); 
}

all_permutations::iterator all_permutations::end() { 
	return iterator( -1 ); 
}

all_permutations::all_permutations( int n ) : _n(n) {
}