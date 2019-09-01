#include <stdexcept>
#include <iostream>
#include <ext.h>

#include "coset.h"
#include "group.h"

Group Coset::supergroup() const {
	return _G;
}

Group Coset::subgroup() const {
	return _H;
}

bool Coset::isRightCoset() const {
	return _right;
}

const Permutation& Coset::representative() const {
	return _sigma;
}

bool Coset::operator==( const Coset& other ) const {
	if( subgroup()->hasSubgroup(other.subgroup()) && other.subgroup()->hasSubgroup( subgroup() ) && isRightCoset() == other.isRightCoset() )
		return subgroup()->contains( representative().inverse() * other.representative() );
	throw std::range_error( "Cosets are incomparable" );
}

Coset::Coset( Group G, Group H, Permutation sigma, bool right ) : _sigma( std::move( sigma ) ) {
	_G = std::move( G );
	_H = std::move( H );
	if( _H == nullptr or !_G->hasSubgroup( _H ) )
		throw std::range_error( "Can't construct coset since argument is not a subgroup" );
	_right = right;
}

std::ostream& operator<<( std::ostream& os, const Coset& c ) {
	if( c.isRightCoset() )
		return os << c.subgroup()->generators() << c.representative();
	else
		return os << c.representative() << c.subgroup()->generators();
}


bool Iso::isEmpty() const {
	return isSecond();
}

const Coset& Iso::coset() const {
	return getFirst();
}

Coset operator*( const Permutation& sigma, const Coset& tauH ) {
	if( tauH.isRightCoset() )
		throw std::range_error( "Left multiplication of right cosets is not defined" );
	Permutation sigmatau = sigma * tauH.representative();
	return Coset( tauH.supergroup(), tauH.subgroup(), sigmatau, false );
}

Iso operator*( const Permutation& sigma, const Iso& tauH ) {
	if( tauH.isEmpty() )
		return tauH;
	else
		return sigma * tauH.coset();
}

void IsoJoiner::join( Iso I ) {
	if( I.isEmpty() )
		return;
	if( _subgroup == nullptr ) {
		_subgroup = I.coset().subgroup();
		_supergroup = I.coset().supergroup();
	}
	elements.push_back( I.coset().representative() );
}

IsoJoiner::operator Iso() {
	if( elements.size() == 0 )
		return Empty();
	Permutation sigma = std::move( elements.back() ); 
	Permutation sigma_inverse = sigma.inverse();
	elements.pop_back();
	for( Permutation& tau : elements )
		tau = sigma_inverse * tau;
	_subgroup = _subgroup->join( std::move( elements ) );
	return Coset( _supergroup, _subgroup, sigma, false );
}