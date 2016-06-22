#include <deque>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "datastructures.h"
#include "unionfind.h"

// ColouredSet

int ColouredSet::getColour( int x ) const {
	return inverse_mapping.at(x);
}

const std::deque<int>& ColouredSet::getColourClass( int i ) const {
	return mapping[i];
}

range ColouredSet::colours() const {
	return range( 0, mapping.size() );
}

const regular_mapping_t& ColouredSet::getMapping() const {
	return mapping;
}

ColouredSet ColouredSet::substructure( std::deque<int> elms ) const {
	regular_mapping_t m;
	for( const auto& c : mapping ) {
		std::deque<int> new_c;
		for( int i : c )
			if( std::find( elms.begin(), elms.end(), i ) != elms.end() )
				new_c.push_back( i );
		if( new_c.size() != 0 )
			m.push_back( std::move( new_c ) );
	}
	return ColouredSet( std::move( m ) );
}

ColouredSet::ColouredSet( std::deque<std::deque<int>> m ) {
	mapping = std::move( m );
	for( size_t i = 0; i < mapping.size(); i++ )
		for( int v : mapping[i] )
			inverse_mapping[v] = i;
}

ColouredSet::ColouredSet( ColouredSet&& other ) {
	mapping = std::move( other.mapping );
	inverse_mapping = std::move( other.inverse_mapping );
}

// ColouredPartition

ColouredPartition::ColouredPartition( std::initializer_list<std::deque<std::deque<int>>> c ) {
	for( auto&& x : c ) {
		for( size_t i = 0; i < x.size(); i++ )
			for( size_t j = 0; j < x[i].size(); j++ )
				inverse_partition_mapping[ x[i][j] ] = std::make_pair( i, j );
		partition_mapping.emplace_back( std::move( x ) );
	}
}

ColouredPartition::ColouredPartition( ColouredSet S ) {
	for( int i : S.colours() ) {
		partition_mapping.push_back( std::deque<std::deque<int>>( 1, S.getColourClass( i ) ) );
		for( int x : S.getColourClass(i) )
			inverse_partition_mapping[x] = std::make_pair( i, 0 );
	}
}

const std::deque<regular_mapping_t>& ColouredPartition::getMapping() const {
	return partition_mapping;
}

// Hypergraph

int Hypergraph::uniformityDegree() const {
	size_t d = E.begin()->first.size();
	for( const auto& e : E )
		if( e.first.size() != d )
			return -1;
	return d;
}

Hypergraph::Hypergraph( std::deque<int> vertices, std::map<std::vector<int>,int> edges ) {
	Omega = std::move( vertices );
	E = std::move( edges );
}

Hypergraph::Hypergraph( std::map<std::vector<int>,int> edges ) {
	E = std::move( edges );
	gatherVertices();
}

void Hypergraph::gatherVertices() {
	std::set<int> S;
	for( auto& e : E )
		for( int x : e.first )
			S.insert( x );
	for( int x : S )
		Omega.push_back( x );
}

// ColouredBipartiteGraph

ColouredBipartiteGraph::ColouredBipartiteGraph( std::deque<int> V1, std::deque<int> V2, ColouredBipartiteGraph::parameter E, ColouredSet S ) : cs( S ) {
	Omega[LEFT] = std::move( V1 );
	Omega[RIGHT] = std::move( V2 );
	int i = 0;
	for( int x : Omega[LEFT] )
		agemO[ x ] = std::make_pair( LEFT, i++ );
	i = 0;
	for( int x : Omega[RIGHT] )
		agemO[ x ] = std::make_pair( RIGHT, i++ );
	i = 0;
	nbh[LEFT].resize( Omega[LEFT].size() );
	nbh[RIGHT].resize( Omega[RIGHT].size() );
	for( int x : Omega[LEFT] ) {
		for( int y : E[x] ) {
			int j = agemO[y].second;
			nbh[LEFT][i].push_back( j );
			nbh[RIGHT][j].push_back( i );
		}
		i++;
	}
	for( auto& L : nbh[LEFT] )
		std::sort( L.begin(), L.end() );
	for( auto& L : nbh[RIGHT] )
		std::sort( L.begin(), L.end() );
}

ColouredBipartiteGraph::ColouredBipartiteGraph( std::deque<int> V1, std::deque<int> V2, ColouredBipartiteGraph::parameter E ) : ColouredBipartiteGraph(V1,V2,E,ColouredSet(std::deque<std::deque<int>>({V1,V2}))) {
}

ColouredBipartiteGraph ColouredBipartiteGraph::substructure( std::deque<int> W1, std::deque<int> W2 ) const {
	parameter E;
	for( int x : W1 ) {
		std::deque<int> nbh;
		for( int y : neighborhood( x ) )
			if( std::find( W2.begin(), W2.end(), y ) != W2.end() )
				nbh.push_back( y );
		E[x] = std::move( nbh );
	}
	std::deque<int> U = W1;
	U.insert( U.end(), W2.begin(), W2.end() );
	return ColouredBipartiteGraph( W1, W2, E, cs.substructure( U ) );
}


std::deque<std::deque<int>> ColouredBipartiteGraph::twins( side s ) const {
	std::vector<bool> taken( Omega[int(s)].size(), false );
	std::deque<std::deque<int>> r;
	for( int x : range( 0, Omega[int(s)].size() ) ) {
		if( not taken[x] ) {
			std::deque<int> equi_class( 1, decode( s, x ) );
			for( int y : range( x+1, Omega[int(s)].size() ) ) {
				if( c_areTwins( s, x, y ) ) {
					taken[y] = true;
					equi_class.push_back( decode( s, y ) );
				}
			}
			r.emplace_back( std::move( equi_class ) );
		}
	}
	return r;
}

double ColouredBipartiteGraph::symmetryDefect( side s ) const {
	size_t max_size = 0;
	auto twin_partition = twins( s );
	for( const auto& t : twin_partition )
		max_size = std::max( max_size, t.size() );
	return 1 - ( double(max_size) / vertices( s ).size() ); 
}

ColouredBipartiteGraph::operator ColouredSet() const {
	return cs;
}

const std::deque<int>& ColouredBipartiteGraph::vertices( side s ) const {
	if( s == LEFT )
		return Omega[0];
	else
		return Omega[1];
}

bool ColouredBipartiteGraph::c_areTwins( side s, int x, int y ) const {
	return colour( decode(s,x) ) == colour( decode(s,y) ) and c_neighborhood(s,x) == c_neighborhood(s,y);
}

const std::deque<int>& ColouredBipartiteGraph::c_neighborhood( side s, int x ) const {
	return nbh[int(s)][x];
}

std::deque<int> ColouredBipartiteGraph::neighborhood( int v ) const {
	auto p = encode( v );
	std::deque<int> r = c_neighborhood( p.first, p.second );
	for( int& x : r )
		x = decode( side( 1-p.first ), x );
	return r;
}

int ColouredBipartiteGraph::colour( int x ) const {
	return cs.getColour( x );
} 

bool ColouredBipartiteGraph::c_hasEdge( int x, int y ) const {
	return std::find( nbh[RIGHT][y].begin(), nbh[RIGHT][y].end(), x ) != nbh[RIGHT][y].end();
}

bool ColouredBipartiteGraph::hasEdge( int v, int w ) const {
	return c_hasEdge( encode(v).second, encode(w).second );
}

int ColouredBipartiteGraph::decode( side s, int x ) const {
	return Omega[int(s)][x];
}

std::pair<ColouredBipartiteGraph::side,int> ColouredBipartiteGraph::encode( int x ) const {
	try {
		return agemO.at(x);
	} catch( const std::out_of_range& error ) {
		throw std::out_of_range( std::to_string(x) + " is not a member of the BipartiteGraph" );
	}
}

ColouredBipartiteGraph::side ColouredBipartiteGraph::getSide( int x ) const {
	return encode(x).first;
}

Hypergraph ColouredBipartiteGraph::neighborhoodHypergraph() const {
	std::map<std::vector<int>,int> E;
	for( int x : range(0, Omega[0].size() ) ) {
		std::vector<int> e;
		e.reserve( nbh[0][x].size() );
		for( int p : nbh[0][x] )
			e.push_back( decode( RIGHT, p ) );
		std::sort( e.begin(), e.end() );
		E.emplace( std::move( e ), 0 ); // perhaps a better colouring
	}
	return Hypergraph( Omega[1], E );
}


ColouredBipartiteGraph& ColouredBipartiteGraph::operator=( ColouredBipartiteGraph&& other ) {
	Omega[0] = std::move( other.Omega[0] );
	Omega[1] = std::move( other.Omega[1] );
	agemO = std::move( other.agemO );
	nbh[0] = std::move( other.nbh[0] );
	nbh[1] = std::move( other.nbh[1] );
	cs = std::move( other.cs );
	n1 = other.n1;
	n2 = other.n2;
	return *this;
}

std::ostream& operator<<( std::ostream& os, const ColouredBipartiteGraph& G ) {
	os << "(" << G.vertices( ColouredBipartiteGraph::LEFT ) 
	   << "," << G.vertices( ColouredBipartiteGraph::RIGHT )
	   << ",{";
	for( int x : G.vertices( ColouredBipartiteGraph::LEFT ) )
		for( int y : G.neighborhood( x ) )
			os << "(" << x << "," << y << ")";
	os << "})";
	return os;
}

// RelationalStructure

size_t RelationalStructure::arity() const {
	return k;
}

bool RelationalStructure::isHomogeneous() const {
	return vertexColours().size() == 1;
}

bool RelationalStructure::isClique() const {
	return relations().size() == (1ULL<<(arity()-1));
}

bool RelationalStructure::isPrimitive() const {
	return isHomogeneous() and witnessOfImprimitivity().first <= 0;
}

std::pair<int,UnionFind> RelationalStructure::witnessOfImprimitivity() const {
	if( arity() != 2 )
		throw;
	if( not isHomogeneous() )
		return std::make_pair( -1, UnionFind(1) );
	auto cols = edgeColours();
	int offset = vertexColours().size();
	std::vector<UnionFind> uf( cols.size(), UnionFind( domain().size() ) );
	int n = Omega.size();
	for( int x : range(0,n) )
		for( int y : range(0,n) )
			if( c_edgeColour( x, y ) >= offset )
				uf[ c_edgeColour( x, y ) - offset ].cup( x, y );
	for( int c : cols )
		if( not uf[c-offset].isUniform() )
			return std::make_pair( c, uf[c-offset] );
	return std::make_pair( -1, UnionFind(1) );
}

bool RelationalStructure::isUPCC() const {
	return isPrimitive() and not isClique();
}

int RelationalStructure::encode( int x ) const {
	try {
		return agemO.at(x);
	} catch( const std::out_of_range& error ) {
		throw std::out_of_range( std::to_string(x) + " is not a member of the RelationalStructure" );
	}
}

int RelationalStructure::decode( int y ) const {
	return Omega.at( y );
}

std::deque<int> RelationalStructure::Relation::vertices() const {
	std::deque<int> r;
	for( const auto& p : (*this) )
		r.push_back( p[0] );
	return r;
}

ColouredBipartiteGraph RelationalStructure::inducedBipartiteGraph( std::deque<int>&& V1, std::deque<int>&& V2, int r ) const {
	if( arity() != 2 )
		throw;
	int n = Omega.size();
	std::deque<std::deque<int>> colours;
	std::map<int,std::deque<int>> colours_inv;
	for( int x : V1 )
		colours_inv[ vertexColour( x ) ].push_back( x );
	for( int x : V2 )
		colours_inv[ vertexColour( x ) + n ].push_back( x );
	int i = 0;
	for( auto& d : colours_inv )
		colours[i++] = std::move( d.second );
	ColouredBipartiteGraph::parameter E;
	for( int x : V1 )
		for( int y : V2 )
			if( edgeColour( x, y ) == r )
				E[x].push_back( y );
	return ColouredBipartiteGraph( V1, V2, E, ColouredSet( std::move( colours ) ) );
}

RelationalStructure::RelationalStructure( const ColouredBipartiteGraph& G ) {
	k = 2;
	Omega.insert( Omega.end(), G.vertices( ColouredBipartiteGraph::LEFT ).begin(), G.vertices( ColouredBipartiteGraph::LEFT ).end() );
	Omega.insert( Omega.end(), G.vertices( ColouredBipartiteGraph::RIGHT ).begin(), G.vertices( ColouredBipartiteGraph::RIGHT ).end() );
	std::sort( Omega.begin(), Omega.end() );
	size_t n = Omega.size();
	for( size_t i = 0; i < n; i++ )
		agemO[ Omega[i] ] = i;
	r.resize( all_tuples( n, k ).size() );
	for( auto p : all_tuples( n, k ) ) { // could be refined
		if( p[0] == p[1] )
			r[ polynomial_evaluation( p, n ) ] = ( G.getSide( decode( p[0] ) ) == ColouredBipartiteGraph::LEFT ) ? 0 : 1;
		else {
			ColouredBipartiteGraph::side a[2] = { G.getSide( decode( p[0] ) ), G.getSide( decode( p[1] ) ) };
			if( a[0] != a[1] )
				if( a[0] == ColouredBipartiteGraph::LEFT )
					r[ polynomial_evaluation( p, n ) ] = G.hasEdge( decode( p[0] ), decode( p[1] ) ) ? 3 : 2;
				else
					r[ polynomial_evaluation( p, n ) ] = G.hasEdge( decode( p[1] ), decode( p[0] ) ) ? 3 : 2;
			else
				r[ polynomial_evaluation( p, n ) ] = 2;
		}
	}
	colour_relations_end = 2;
	all_relations_end = 4; // does not work on empty graph
}

RelationalStructure::RelationalStructure( std::deque<int>&& C, std::vector<int>&& r2, int k2 ) {
	Omega = std::move( C );
	int n = Omega.size();
	int counter = 0;
	for( int i : Omega )
		agemO[i] = counter++;
	k = k2;
	r = std::move( r2 );
	all_relations_end = colour_relations_end = 0;
	for( int x : r )
		all_relations_end = std::max( all_relations_end, x );
	std::vector<int> p( k );
	for( int i : range( 0, n ) ) {
		for( int& x : p )
			x = i;
		colour_relations_end = std::max( colour_relations_end, r[ polynomial_evaluation( p, n ) ] );
	}
	all_relations_end++;
	colour_relations_end++;
}

RelationalStructure::operator ColouredSet() const {
	std::deque<std::deque<int>> S( vertexColours().size() );
	for( int i : range( 0, Omega.size() ) )
		S[ r[ polynomial_evaluation( std::vector<int>( arity(), i ), Omega.size() ) ] ].push_back( Omega[i] );
	return ColouredSet( std::move( S ) );
}

int RelationalStructure::refine() {
	int i = 0;
	while( WeisfeilerLehman() ) i++;
	return i;
}

RelationalStructure RelationalStructure::skeletalSubstructure( size_t t, std::deque<int> C ) const {
	int n = Omega.size();
	int new_n = C.size();
	std::vector<int> r2( all_tuples( new_n, t ).size() );
	std::vector<int> used_cols( relations().size(), -1 );
	for( auto p : all_tuples( new_n, t ) ) {
		std::vector<int> q = p;
		q.resize( arity(), q.back() );
		used_cols[ r2[ polynomial_evaluation( p, new_n ) ] = r[ polynomial_evaluation( q, n ) ] ] = 0;
	}
	size_t new_cols = 0;
	for( int& x : used_cols )
		if( x == 0 )
			x = new_cols++;
	for( int& x : r2 )
		x = used_cols[x];
	return RelationalStructure( std::move( C ), std::move( r2 ), t );
}

RelationalStructure RelationalStructure::skeleton( size_t t ) const {
	return skeletalSubstructure( t, Omega );
}

/*
std::deque<std::deque<int>> RelationalStructure::twins( int i ) const {
	int n = Omega.size();
	std::vector<int> q;
	UnionFind uf( n );
	for( int x : range( 0, n ) ) {
		if( c_vertexColour( x ) != i  )
			continue;
		for( int y : range( 0, x ) ) {
			if( c_vertexColour( y ) != i  )
				continue;
			bool add = true;
			for( auto p : all_tuples( Omega.size(), arity() ) ) {
				q = p;
				for( int& j : q )
					if( j == x )
						j = y;
					else if ( j == y )
						y = x;
				if( r[ polynomial_evaluation( p, n ) ] != r[ polynomial_evaluation( q, n ) ] ) {
					add = false;
					break;
				}
			}
			if( add )
				uf.cup( x, y );
		}
	}
	std::unordered_map<int,int> rel_map;
	std::deque<std::deque<int>> r_val;
	for( int j : range( 0, n ) ) {
		if( c_vertexColour( j ) == i ) {
			if( uf.find( j ) == j ) {
				rel_map[j] = r_val.size();
				r_val.push_back({j});
			} else
				r_val[ rel_map[ uf.find( j ) ] ].push_back( j );
		}
	}
	return r_val;
}*/

int RelationalStructure::getNonAlphaPartition( double alpha ) {
	int n = Omega.size();
	std::vector<int> p( arity() );
	std::vector<int> col_count( vertexColours().size(), 0 );
	for( int i : range( 0, n ) ) {
		for( int& x : p )
			x = i;
		col_count[r[polynomial_evaluation(p,n)]]++;
	}
	double m = alpha*n;
	for( int i : vertexColours() )
		if( col_count[i] > m )
			return i;
	return -1;
}


const std::deque<int>& RelationalStructure::domain() const {
	return Omega;
} 

range RelationalStructure::vertexColours() const {
	return range( 0, colour_relations_end );
}

range RelationalStructure::edgeColours() const {
	return range( colour_relations_end, all_relations_end );
}

range RelationalStructure::relations() const {
	return range( 0, all_relations_end );
}

RelationalStructure::Relation RelationalStructure::relation( size_t i ) const {
	int n = Omega.size();
	Relation rel;
	for( auto& p : all_tuples( n, arity() ) ) {
		if( r[polynomial_evaluation( p, n )] == signed(i) ) {
			std::vector<int> q = p;
			for( int& x : q )
				x = decode( x );
			rel.push_back( std::move(q) );
		}
	}
	return rel;
}


int RelationalStructure::c_edgeColour( int x, int y ) const {
	return r[polynomial_evaluation( std::vector<int>( {x,y} ), Omega.size() )];
}

int RelationalStructure::edgeColour( int a, int b ) const {
	assert( arity() == 2 );
	return c_edgeColour( encode(a), encode(b) );
}


int RelationalStructure::vertexColour( int x ) const {
	return r[ polynomial_evaluation( std::vector<int>( arity(), decode( x ) ), domain().size() ) ];
}

int& RelationalStructure::c_vertexColour( int x ) {
	return r[ polynomial_evaluation( std::vector<int>( arity(), x ), domain().size() ) ];
}


bool RelationalStructure::WeisfeilerLehman() {
	int n = domain().size(); // Omega -> domain
	int k = arity();
	// int rels = relations().size();
	std::map<std::vector<std::vector<int>>,std::deque<int>> invmap;
	for( auto& x : all_tuples( n, k ) ) {
		std::vector<std::vector<int>> C( n+1 );
		for( int y : range( 0, n ) ) {
			C[y+1].reserve( k );
			for( int i : range( 0, k ) ) {
				int t = x[i];
				x[i] = y;
				C[y+1].push_back( r[ polynomial_evaluation( x, n ) ] );
				x[i] = t;
			}
		}
		int index = polynomial_evaluation( x, n );
		std::sort( C.begin()+1, C.end() );
		C[0] = std::vector<int>( 1, r[index] );
		invmap[ std::move( C ) ].push_back( index );
	}
	//std::cout << invmap;
	size_t new_colour = 0;
	for( const auto& v : invmap ) {
		for( int x : v.second )
			r[ x ] = new_colour;
		new_colour++;
	}
	int old_colours = all_relations_end;
	/*all_relations_end = new_colour;
	colour_relations_end = 0;
	for( int x : range( 0, n ) )
		colour_relations_end = std::max( colour_relations_end, c_vertexColour( x ) );
	colour_relations_end++;*/
	normalise();
	return old_colours != all_relations_end;
}

void RelationalStructure::normalise() {
	std::vector<int> p( arity() );
	std::map<int,int> vertex_colours_found;
	std::map<int,int> all_colours_found;
	int n = Omega.size();
	for( int i : range( 0, n ) ) {
		for( int& x : p )
			x = i;
		vertex_colours_found[ r[ polynomial_evaluation( p, n ) ] ] = 0;
	}
	for( int i : r )
		all_colours_found[ i ] = 0;
	colour_relations_end = vertex_colours_found.size();
	all_relations_end = all_colours_found.size();
	int i = 0;
	for( auto& x : vertex_colours_found )
		x.second = i++;
	for( auto& x : all_colours_found )
		if( vertex_colours_found.find( x.first ) == vertex_colours_found.end() )
			x.second = i++;
		else
			x.second = vertex_colours_found[x.first];
	for( auto& i : r )
		i = all_colours_found[i];
}

std::ostream& operator<<( std::ostream& os, const RelationalStructure& X ) {
	os << "(";
	for( int i : X.relations() )
		os << X.relation( i ) << ",";
	os << ")";
	return os;
}

JohnsonScheme::JohnsonScheme( int m, int t ) : all_tuples( m, t ) {
}

std::map<std::vector<int>,int> JohnsonScheme::completeMapping() const {
	size_t i = 0;
	std::map<std::vector<int>,int> r;
	for( const auto& S : *this )
		r[ S ] = mapping[ i++ ];
	return r;
}



/*ColouredBipartiteGraph inducedBipartiteGraph( std::deque<int>&& V1, std::deque<int>&& V2, int r ) const {
	typedef ColouredBipartiteGraph::side side;
	std::deque<std::deque<int>> ll[2];
	ll[0].resize( V1.size() );
	ll[1].resize( V2.size() );
	int ic = 0;
	for( int i : V1 ) {
		int jc = 0;
		for( int j : V2 ) {
			if( edgeColour( i, j ) == r ) {
				ll[0][i].push_back(  )
			}
			jc++;
		}
		ic++;
	}
}*/