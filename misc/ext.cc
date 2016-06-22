#include <iostream>
#include "ext.h"

std::ostream& operator<<( std::ostream& os, Empty ) {
	return os << "Ø";
}

range::iterator::iterator( int i ) : _i( i ) {
}

range::iterator::iterator( const self_type& other ) {
	_i = other._i;
}

range::iterator::self_type range::iterator::operator++(int) { 
	self_type r = *this; 
	++(*this); 
	return r; 
}

range::iterator::self_type& range::iterator::operator++() {
	++_i;
	return *this;
}

range::iterator::reference range::iterator::operator*() { 
	return _i;
}

range::iterator::pointer range::iterator::operator->() { 
	return &_i; 
}

bool range::iterator::operator==(const self_type& rhs) { 
	return _i == rhs._i; 
}

bool range::iterator::operator!=(const self_type& rhs) { 
	return _i != rhs._i; 
}

size_t range::size() const {
	return _m-_n;
}

range::iterator range::begin() { 
	return iterator( _n ); 
}

range::iterator range::end() { 
	return iterator( _m ); 
}

range::operator std::deque<int>() const {
	std::deque<int> S( _n );
	std::iota( S.begin(), S.end(), 0 );
	return S;
}


range::range( int n, int m ) : _n(n), _m(m) {}

all_tuples::iterator::iterator( int n, int r ) : _n(n), _r(r), _tuple( _r, 0 ) {
}

all_tuples::iterator::iterator( const self_type& other ) {
	_n = other._n;
	_r = other._r;
	_tuple = other._tuple;
}

size_t all_tuples::size() const {
	return pow( _n, _r );
}


all_tuples::iterator::self_type all_tuples::iterator::operator++(int) { 
	self_type i = *this; 
	++(*this); 
	return i; 
}

all_tuples::iterator::self_type& all_tuples::iterator::operator++() {
	for( int i = _r-1; i >= 0; i-- ) {
		_tuple[i] += 1;
		if( _tuple[i] == _n )
			_tuple[i] = 0;
		else
			return *this;
	}
	_n = -1;
	return *this;
}

all_tuples::iterator::reference all_tuples::iterator::operator*() { 
	return _tuple; 
}

all_tuples::iterator::pointer all_tuples::iterator::operator->() { 
	return &_tuple; 
}

bool all_tuples::iterator::operator==(const self_type& rhs) { 
	return _n == rhs._n && _tuple == rhs._tuple;; 
}

bool all_tuples::iterator::operator!=(const self_type& rhs) { 
	return _n != rhs._n || _tuple != rhs._tuple; 
}

all_tuples::iterator all_tuples::begin() const { 
	return iterator( _n, _r ); 
}

all_tuples::iterator all_tuples::end() const { 
	return iterator( -1, _r ); 
}

all_tuples::all_tuples( int n, int r ) : _n(n), _r(r) {}

all_ordered_tuples::iterator::iterator( int n, int r ) : _n(n), _r(r), _tuple( _r ) {
	for( int i = 0; i < _r; i++ )
		_tuple[i] = i;
}

all_ordered_tuples::iterator::iterator( const self_type& other ) {
	_n = other._n;
	_r = other._r;
	_tuple = other._tuple;
}

all_ordered_tuples::iterator::self_type all_ordered_tuples::iterator::operator++(int) { 
	self_type i = *this; 
	++(*this); 
	return i; 
}

all_ordered_tuples::iterator::self_type& all_ordered_tuples::iterator::operator++() {
	if( _tuple[_r-1] < _n-1 )
		_tuple[_r-1]++;
	else {
		int i = _r-1;
		while( i --> 0 )
			if( _tuple[i+1] != _tuple[i]+1 )
				break;
		if( i == -1 )
			_n = -1;
		else {
			_tuple[i]++;
			for( int j = i+1; j < _r; j++ )
				_tuple[j] = _tuple[j-1]+1;
		}
	}
	return *this;
}

all_ordered_tuples::iterator::reference all_ordered_tuples::iterator::operator*() { 
	return _tuple; 
}

all_ordered_tuples::iterator::pointer all_ordered_tuples::iterator::operator->() { 
	return &_tuple; 
}

bool all_ordered_tuples::iterator::operator==(const self_type& rhs) { 
	return _n == rhs._n && ( _n == -1 || _tuple == rhs._tuple );
}

bool all_ordered_tuples::iterator::operator!=(const self_type& rhs) { 
	return _n != rhs._n || ( _n != -1 && _tuple != rhs._tuple ); 
}

all_ordered_tuples::iterator all_ordered_tuples::begin() const { 
	return iterator( _n, _r ); 
}

all_ordered_tuples::iterator all_ordered_tuples::end() const { 
	return iterator( -1, _r ); 
}

size_t all_ordered_tuples::size() const {
	return binom( _n, _r );
}

size_t all_ordered_tuples::arity() const {
	return _r;
}

range all_ordered_tuples::parent_set() const {
	return range( 0, _n );
}

all_ordered_tuples::all_ordered_tuples( int n, int r ) : _n(n), _r(r) {}

all_different_tuples::iterator::iterator( int n, int r ) : _n(n), _r(r), _tuple( _r ) {
	for( int i = 0; i < _r; i++ )
		_tuple[i] = i;
}

all_different_tuples::iterator::iterator( const self_type& other ) {
	_n = other._n;
	_r = other._r;
	_tuple = other._tuple;
}

all_different_tuples::iterator::self_type all_different_tuples::iterator::operator++(int) { 
	self_type i = *this; 
	++(*this); 
	return i; 
}

all_different_tuples::iterator::self_type& all_different_tuples::iterator::operator++() {
	if( _r == 0 ) {
		_n = -1;
		return *this;
	}
	if( std::next_permutation( _tuple.begin(), _tuple.end() ) )
		return *this;
	if( _tuple[_r-1] < _n-1 )
		_tuple[_r-1]++;
	else {
		int i = _r-1;
		while( i --> 0 )
			if( _tuple[i+1] != _tuple[i]+1 )
				break;
		if( i == -1 )
			_n = -1;
		else {
			_tuple[i]++;
			for( int j = i+1; j < _r; j++ )
				_tuple[j] = _tuple[j-1]+1;
		}
	}
	return *this;
}

all_different_tuples::iterator::reference all_different_tuples::iterator::operator*() { 
	return _tuple; 
}

all_different_tuples::iterator::pointer all_different_tuples::iterator::operator->() { 
	return &_tuple; 
}

bool all_different_tuples::iterator::operator==(const self_type& rhs) { 
	return _n == rhs._n && ( _n == -1 || _tuple == rhs._tuple );
}

bool all_different_tuples::iterator::operator!=(const self_type& rhs) { 
	return _n != rhs._n || ( _n != -1 && _tuple != rhs._tuple ); 
}

all_different_tuples::iterator all_different_tuples::begin() const { 
	return iterator( _n, _r ); 
}

all_different_tuples::iterator all_different_tuples::end() const { 
	return iterator( -1, _r ); 
}

size_t all_different_tuples::size() const {
	int v = 1;
	for( int i = _n; i > _r; --i )
		v *= i;
	return v;
}

size_t all_different_tuples::arity() const {
	return _r;
}

range all_different_tuples::parent_set() const {
	return range( 0, _n );
}

all_different_tuples::all_different_tuples( int n, int r ) : _n(n), _r(r) {}

int pow( int n, int k ) {
	int r = 1;
	while( k --> 0 )
		r *= n;
	return r;
}

std::vector<int> inverse_mapping( const std::vector<int>& d ) {
	std::vector<int> r( d.size(), 0 );
	for( size_t i = 0; i < d.size(); i++ )
		r[d[i]] = i;
	return r;
}

uint binom( int n, int k ) {
	uint ans = 1;
	k = std::max( k, n-k );
	for( int j = 1; j <= k; j++, n-- ) {
		if( n%j==0 )
			ans *= n/j;
		else if( ans%j == 0 )
			ans = ans/j*n;
		else
			ans = (ans*n)/j;
	}
	return ans;
}

bool bipartiteMatching_Subroutine( const matrix<bool>& M, int p, std::vector<bool>& seen, std::vector<int>& match ) {
	for( int q : range( 0, M.height() ) ) {
		if( M.at(p,q) and not seen.at(q) ) {
			seen.at(q) = true;
			if( match.at(q) < 0 or bipartiteMatching_Subroutine( M, match.at(q), seen, match ) ) {
				match.at(q) = p;
				return true;
			}
		}
	}
	return false;
}

std::vector<int> bipartiteMatching( const matrix<bool>& M ) {
	std::vector<int> match( M.width(), -1 );
	for( int p : range( 0, M.height() ) ) {
		std::vector<bool> seen( M.width(), false );
		bipartiteMatching_Subroutine( M, p, seen, match );
	}
	return match;
}