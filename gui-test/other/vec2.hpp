#pragma once
#include <cmath>
#include <algorithm>

struct vec2_t {
	/* default builders */
	vec2_t( ) : x( 0 ) , y( 0 ) { }
	vec2_t( float x , float y ) : x( x ) , y( y ) { }
	~vec2_t( ) { }

	/* operators */
	bool operator==( const vec2_t& src ) const {
		return ( src.x == this->x ) && ( src.y == y );
	}

	bool operator!=( const vec2_t& src ) const {
		return ( src.x != this->x ) || ( src.y != y );
	}

	float& operator[]( int i ) {
		return ( ( float* ) this ) [ i ];
	}

	float operator[]( int i ) const {
		return ( ( float* ) this ) [ i ];
	}

	vec2_t& operator+=( const vec2_t& v ) {
		this->x += v.x; this->y += v.y;
		return *this;
	}

	vec2_t& operator-=( const vec2_t& v ) {
		this->x -= v.x; this->y -= v.y;
		return *this;
	}

	vec2_t& operator*=( float fl ) {
		this->x *= fl;
		this->y *= fl;

		return *this;
	}

	vec2_t& operator*=( const vec2_t& v ) {
		this->x *= v.x;
		this->y *= v.y;

		return *this;
	}

	vec2_t& operator/=( const vec2_t& v ) {
		this->x /= v.x;
		this->y /= v.y;

		return *this;
	}

	vec2_t& operator+=( float fl ) {
		this->x += fl;
		this->y += fl;

		return *this;
	}

	vec2_t& operator/=( float fl ) {
		this->x /= fl;
		this->y /= fl;

		return *this;
	}

	vec2_t& operator-=( float fl ) {
		this->x -= fl;
		this->y -= fl;

		return *this;
	}

	vec2_t& operator+( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x + v.x;
		temp.y = this->y + v.y;

		return temp;
	}

	vec2_t& operator*( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x * v.x;
		temp.y = this->y * v.y;

		return temp;
	}

	vec2_t& operator*( float v ) {
		vec2_t temp;
		temp.x = this->x * v;
		temp.y = this->y * v;

		return temp;
	}

	vec2_t& operator-( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x - v.x;
		temp.y = this->y - v.y;

		return temp;
	}

	vec2_t& operator/( vec2_t v ) {
		vec2_t temp;
		temp.x = this->x / v.x;
		temp.y = this->y / v.y;

		return temp;
	}

	vec2_t& operator/( float v ) {
		vec2_t temp;
		temp.x = this->x / v;
		temp.y = this->y / v;

		return temp;
	}

	/* functions */
	float length( ) const {
		return std::sqrtf( this->x * this->x + this->y * this->y );
	}

	float dist_to( const vec2_t& vec ) const {
		vec2_t delta;
		delta.x = this->x - vec.x;
		delta.y = this->y - vec.y;

		return delta.length( );
	}

	vec2_t normalized( ) const {
		auto res = *this;
		auto l = res.length( );

		if ( l != 0.0f )
			res /= l;
		else
			res.x = res.y = 0.0f;

		return res;
	}

	/* parameters */
	float x;
	float y;
};
