#pragma once
#include <iostream>
#include <vector>
#include <initializer_list>
#include <stdexcept>

template<typename T>
class matrix {
	int w, h;
	std::vector<T> data;
public:
	int width() const;
	int height() const;
	typename std::vector<T>::reference at( int row, int column );
	typename std::vector<T>::const_reference at( int row, int column ) const;
	void resize( int height, int width );
	void print( std::ostream& os ) const;
	void clear();
	bool operator<( const matrix<T>& other ) const;
	matrix();
	matrix( int width, int height );
	matrix( std::initializer_list<std::initializer_list<T>> list );
	~matrix();
};

template<typename T> 
int matrix<T>::width() const {
	return w;
}

template<typename T> 
int matrix<T>::height() const {
	return h;
}

template<typename T> 
typename std::vector<T>::reference matrix<T>::at( int row, int column ) {
	return data.at( row*width() + column );
}

template<typename T> 
typename std::vector<T>::const_reference matrix<T>::at( int row, int column ) const {
	return data.at( row*width() + column );
}

template<typename T> 
void matrix<T>::resize( int height, int width ) {
	h = height;
	w = width;
	clear();
}

template<typename T> 
void matrix<T>::print( std::ostream& os ) const {
	os << "[";
	for( int i = 0; i < height(); i++ ) {
		os << "[" << at(i,0);
		for( int j = 1; j < width(); j++ )
			os << "," << at(i,j);
		os << "]," << std::endl;
	}
	os << "]";
}

template<typename T> 
void matrix<T>::clear() {
	data.assign( w*h, 0 );
}

template<typename T> 
matrix<T>::matrix() {
	w = h = 0;
}

template<typename T> 
matrix<T>::matrix( int height, int width ) {
	resize( height, width );
}

template<typename T> 
matrix<T>::matrix( std::initializer_list<std::initializer_list<T>> list ) {
	h = list.size();
	if( h == 0 ) {
		w = 0;
		resize( 0, 0 );
		return;
	}
	w = list.begin()->size();
	for( auto& row : list ) {
		if( size_t(w) != row.size() )
			throw std::out_of_range( "Irregular matrix width" );
	}
	resize( h, w );
	int i = 0;
	for( auto& row : list ) {
		int j = 0;
		for( auto& element : row )
			at( i, j++ ) = element;
		i++;	
	}
}

template<typename T> 
bool matrix<T>::operator<( const matrix<T>& other ) const {
	if( width() < other.width() )
		return true;
	if( width() > other.width() )
		return false;
	return data < other.data;
}

template<typename T>
matrix<T>::~matrix() { }

template<typename T>
std::ostream& operator<<( std::ostream& os, const matrix<T>& M ) {
	M.print( os );
	return os;
}