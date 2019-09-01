#pragma once
#include <new>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <array>
#include <memory>
#include <deque>
#include <unordered_map>
#include "matrix.h"

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::set<T>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "{" << (*x);
		while( (++x) != s.end() )
			os << "," << (*x);
		os << "}";
	}
	return os;
}

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "[" << (*x);
		while( (++x) != s.end() )
			os << "," << (*x);
		os << "]";
	}
	return os;
}

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::deque<T>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "\u00AB" << (*x);
		while( (++x) != s.end() )
			os << "," << (*x);
		os << "\u00BB";
	}
	return os;
}

template<typename T,size_t k>
std::ostream& operator<<( std::ostream& os, const std::array<T,k>& s ) {
	if( s.empty() )
		os << "()";
	else {
		auto x = s.begin();
		os << "(" << (*x);
		while( (++x) != s.end() )
			os << "," << (*x);
		os << ")";
	}
	return os;
}

template<typename T, typename U>
std::ostream& operator<<( std::ostream& os, const std::map<T,U>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "{" << x->first << " -> " << x->second;
		while( (++x) != s.end() )
			os << "," << x->first << " -> " << x->second;
		os << "}";
	}
	return os;
}

template<typename T, typename U>
std::ostream& operator<<( std::ostream& os, const std::unordered_map<T,U>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "{" << x->first << " -> " << x->second;
		while( (++x) != s.end() )
			os << "," << x->first << " -> " << x->second;
		os << "}";
	}
	return os;
}

template<typename U, typename V> class Either {
	int which;
	union {
		U first;
		V second;
	};
public:
	bool isFirst() const;
	bool isSecond() const;
	const U& getFirst() const;
	const V& getSecond() const;
	Either( U u );
	Either( V v );
	Either( const Either<U,V>& );
	~Either();
};

template<typename U, typename V>
bool Either<U,V>::isFirst() const {
	return which == 0;
}

template<typename U, typename V>
bool Either<U,V>::isSecond() const {
	return which == 1;
}

template<typename U, typename V>
const U& Either<U,V>::getFirst() const {
	return first;
}

template<typename U, typename V>
const V& Either<U,V>::getSecond() const {
	return second;
}

template<typename U, typename V>
Either<U,V>::Either( U u ) {
	which = 0;
	new (&first) U( u );
}

template<typename U, typename V>
Either<U,V>::Either( V v ) {
	which = 1;
	new (&second) V( v );
}

template<typename U, typename V>
Either<U,V>::Either( const Either<U,V>& other ) {
	if( other.isFirst() ) {
		which = 0;
		new (&first) U( other.getFirst() );
	} else {
		which = 1;
		new (&second) V( other.getSecond() );
	} 
}

template<typename U, typename V>
Either<U,V>::~Either() {
	if( isFirst() )
		first.~U();
	else
		second.~V();
}

template<typename T>
std::vector<T> to_vector( const std::set<T>& s ) {
	std::vector<T> r;
	r.reserve( s.size() );
	for( const auto& x : s )
		r.emplace_back( x );
	return r;
}

struct Empty {
};

std::ostream& operator<<( std::ostream& os, Empty e );

template<typename U, typename V>
std::ostream& operator<<( std::ostream& os, const Either<U,V>& e ) {
	if( e.isFirst() )
		return os << e.getFirst();
	else
		return os << e.getSecond();
}

class all_tuples {
	int _n;
	int _r;
public:
	class iterator {
		int _n;
		int _r;
		std::vector<int> _tuple;
	public:
		typedef iterator self_type;
		typedef std::vector<int> value_type;
		typedef std::vector<int>& reference;
		typedef std::vector<int>* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n, int r );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin() const;
	iterator end() const;
	size_t size() const;
	all_tuples( int n, int r );
};


class range {
	int _n;
	int _m;
public:
	class iterator {
		int _i;
	public:
		typedef iterator self_type;
		typedef int value_type;
		typedef int& reference;
		typedef int* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int i );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin();
	iterator end();
	size_t size() const;
	range( int n, int m );
	explicit operator std::deque<int>() const;
};

template<int _r>
class all_arrays {
	int _n;
public:
	class iterator {
		int _n;
		std::array<int,_r> _array;
	public:
		typedef iterator self_type;
		typedef std::array<int,_r> value_type;
		typedef std::array<int,_r>& reference;
		typedef std::array<int,_r>* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin();
	iterator end();
	size_t size() const;
	all_arrays( int n );
};

template<int _r>
all_arrays<_r>::iterator::iterator( int n ) : _n(n) {
	_array.fill( 0 );
}

template<int _r>
all_arrays<_r>::iterator::iterator( const self_type& other ) {
	_n = other._n;
	_array = other._array;
}

template<int _r>
typename all_arrays<_r>::iterator::self_type all_arrays<_r>::iterator::operator++(int) { 
	self_type i = *this; 
	++(*this); 
	return i; 
}

template<int _r>
typename all_arrays<_r>::iterator::self_type& all_arrays<_r>::iterator::operator++() {
	for( int i = _r-1; i >= 0; i-- ) {
		_array[i] += 1;
		if( _array[i] == _n )
			_array[i] = 0;
		else
			return *this;
	}
	_n = -1;
	return *this;
}

template<int _r>
typename all_arrays<_r>::iterator::reference all_arrays<_r>::iterator::operator*() { 
	return _array; 
}

template<int _r>
typename all_arrays<_r>::iterator::pointer all_arrays<_r>::iterator::operator->() { 
	return &_array; 
}

template<int _r>
bool all_arrays<_r>::iterator::operator==(const self_type& rhs) { 
	return _n == rhs._n && _array == rhs._array;; 
}

template<int _r>
bool all_arrays<_r>::iterator::operator!=(const self_type& rhs) { 
	return _n != rhs._n || _array != rhs._array; 
}

template<int _r>
typename all_arrays<_r>::iterator all_arrays<_r>::begin() { 
	return iterator( _n ); 
}

template<int _r>
typename all_arrays<_r>::iterator all_arrays<_r>::end() { 
	return iterator( -1 ); 
}

template<int _r>
size_t all_arrays<_r>::size() const { 
	return pow(_n,_r);
}


template<int _r>
all_arrays<_r>::all_arrays( int n ) : _n(n) {}

class all_ordered_tuples {
	int _n;
	int _r;
public:
	class iterator {
		int _n;
		int _r;
		std::vector<int> _tuple;
	public:
		typedef iterator self_type;
		typedef std::vector<int> value_type;
		typedef std::vector<int>& reference;
		typedef std::vector<int>* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n, int r );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin() const;
	iterator end() const;
	range parent_set() const;
	size_t size() const;
	size_t arity() const;
	all_ordered_tuples( int n, int r );
};

class all_different_tuples {
	int _n;
	int _r;
public:
	class iterator {
		int _n;
		int _r;
		std::vector<int> _tuple;
	public:
		typedef iterator self_type;
		typedef std::vector<int> value_type;
		typedef std::vector<int>& reference;
		typedef std::vector<int>* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n, int r );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin() const;
	iterator end() const;
	range parent_set() const;
	size_t size() const;
	size_t arity() const;
	all_different_tuples( int n, int r );
};


int pow(int,int);

template<typename T>
std::vector<int> index_sort( const T& data ) {
	int n = data.size();
	std::vector<int> mapping( n );
	for( int i = 0; i < n; i++ )
		mapping[i] = i;
	std::sort( mapping.begin(), mapping.end(), [&data]( int lhs, int rhs ) {
			return data[lhs] < data[rhs];
		} );
	return mapping;
}

std::vector<int> inverse_mapping( const std::vector<int>& d );

template<typename T>
std::vector<int> rank( const T& data ) {
	auto indexing = index_sort( data );
	return inverse_mapping( indexing );
}

template<typename T> bool size_compare( const T& lhs, const T& rhs ) {
	return lhs.size() < rhs.size();
}

template<typename T> std::set<T> flatten( const std::set<std::set<T>>& s ) {
	std::set<T> t;
	for( const auto& u : s )
		t.insert( u.begin(), u.end() );
	return t;
}

template<typename T> std::vector<T> flatten( const std::vector<std::vector<T>>& s ) {
	std::vector<T> t;
	for( const auto& u : s )
		t.insert( t.end(), u.begin(), u.end() );
	return t;
}

template<typename T>
int polynomial_evaluation( const T& coeff, int x ) {
	int r = 0;
	for( int c : coeff )
		r = c + x*r;
	return r;
}

uint binom( int n, int k );

template<typename T>
class any_const_iterator {
	typedef T value_type;
	struct concept {
		virtual void next(int n) = 0;
		virtual const value_type& deref() const = 0;
		virtual bool equal(const void* other) const = 0;
		virtual std::unique_ptr<concept> clone() const = 0;
		virtual const std::type_info& type() const = 0;
		virtual const void* address() const = 0;
		virtual ~concept() = default;
	};
	template<typename Iter>
	struct model : concept {
		model(Iter iter) : _iter(iter) {}
		model(const model& o) : _iter(o._iter) {}
		model(model&& o) : _iter(std::move(o._iter)) {}

		void next(int n) override { _iter = std::next(_iter, n); }
		const value_type& deref() const override { return *_iter; }
		bool equal(const void* rp) const override { return _iter == static_cast<const model*>(rp)->_iter; }
		std::unique_ptr<concept> clone() const override { return std::make_unique<model>(*this); }
		const std::type_info& type() const override { return typeid(_iter); }
		const void* address() const override { return this; }

		Iter _iter;
	};

	std::unique_ptr<concept> _impl;
public:
	// interface

	// todo: constrain Iter to be something that iterates value_type
	template<typename Iter>
	any_const_iterator(Iter iter) : _impl(std::make_unique<model<Iter>>(iter)) {
		// static_assert(std::is_same<iter::value_type, value_type>::value, "iterator doesn't iterate over required type");
	}

	any_const_iterator(any_const_iterator&& r) : _impl(std::move( r._impl )) {};

	any_const_iterator(const any_const_iterator& r) : _impl(r._impl->clone()) {};

	const value_type& operator*() const {
		return _impl->deref();
	}

	const value_type* operator->() const {
		return &_impl->deref();
	}

	any_const_iterator& operator++() {
		_impl->next(1);
		return *this;
	}

	bool operator==(const any_const_iterator& r) const {
		return _impl->type() == r._impl->type()
		and _impl->equal(r._impl->address());
	}

	bool operator!=(const any_const_iterator& r) const {
		return not(*this == r);
	}
};

template<typename T>
size_t largest( const T& S ) {
	size_t r = 0, i = 0, m = 0, t;
	for( const auto& s : S ) {
		t = s.size();
		if( t > m ) {
			r = i;
			m = t;
		}
		++i;
	}
	return r;
}

std::vector<int> bipartiteMatching( const matrix<bool>& M );

template<typename T>
std::deque<T> split( const T& S, size_t k ) {
	size_t n = S.size();
	size_t p = n % k;
	size_t q = n / k;
	T U;
	std::deque<T> V;
	auto i = S.cbegin();
	for( size_t j = 0; j < k; ++j ) {
		for( size_t h = 0; h < q; ++h )
			U.push_back( *(i++) );
		if( j < p )
			U.push_back( *(i++) );
		V.push_back( std::move( U ) );
	}
	return V;
}

template<typename T>
std::deque<T> join( const std::deque<T>& A, const std::deque<T>& B ) {
	std::deque<T> result;
	auto i = A.cbegin();
	for( const auto& y : B ) {
		if( i != A.cend() ) {
			while( *i < y ) {
				result.push_back( *(i++) );
				if( i == A.cend() )
					break;
			}
			if( i != A.cend() and *i == y )
				++i;
		}
		result.push_back( y );
	}
	while( i != A.cend() )
		result.push_back( *(i++) );
	return result;
}

template<typename T>
size_t intersection_size( const T& a, const T& b ) {
	auto i = b.cbegin();
	size_t r = 0;
	for( int x : a ) {
		while( (*i) < x )
			++i;
		if( (*i) == x )
			++r;
	}
	return r;
}