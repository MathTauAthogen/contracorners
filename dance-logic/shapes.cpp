#include "shapes.h"



template < typename T >
Shape<T>::Shape (
	bool from_center,
	float rotate,
	float scale_factor,
	float current_pos_x, 
	float current_pos_y
	) :
	Drawable < Shape < T > > (
		from_center,
		rotate,
		scale_factor,
		current_pos_x,
		current_pos_y
	)
	{}

template < typename T >
Pixel Shape<T>::get_pixel ( int i, int j )
{
	auto [x, y] = Drawable <Shape < T > >::transform ( i, j );

	if( x == -1 && y == -1 )
	{
		return Drawable <Shape < T > >::_nullpix;
	}

	return static_cast < T* > (this) -> _getPixel ( x, y );
}








Pixel Circle::_getPixel ( float x, float y )
{

	dist = sqrt ( pow( x, 2 ) + pow ( y, 2 ) );
	error =  _approx_const / _scale_factor;

	if (
		1 - error <= dist && 
		dist <= 1 + error
		)
	{

		// De-rotate for correct symbol for real-life slope

		if( _from_centre ){ x += _centre_offset_x; y += _centre_offset_y; }

		offset_x = x - _centre_offset_x;
		offset_y = y - _centre_offset_y;


		x = offset_x * cos( 2 * pi * _rotation )
			- offset_y * sin( 2 * pi * _rotation );

		y = offset_x * sin( 2 * pi * _rotation )
			+ offset_y * cos( 2 * pi * _rotation );

		x += _centre_offset_x;
		y += _centre_offset_y;
		
		// End de-rotate

		if ( x > 0 && abs(y / x) < 	_slope_const )
		{
			return possibilities[0];
		}

		else if ( x < 0 && abs(y / x) < _slope_const )
		{
			return possibilities[1];
		}

		else if ( y != 0 && abs(x / y)  < _slope_const )
		{
			return possibilities[2];
		}

		else if ( x != 0 && y / x > 0 )
		{
			return possibilities[3];
		}
		
		else if ( x != 0 && y / x < 0 )
		{
			return possibilities[4];
		}
		
		else if ( x == 0 && y == 0 )
		{
			return _nullpix;
		}

		else {
			cout << "WHAT?" << x << " and " << y << endl;
		}
		return _nullpix;
	}
	else{
		return _nullpix;
	}

}

Circle::Circle (
	bool from_center,
	float scale_factor,
	float current_pos_x,
	float current_pos_y,
	float rotation
	) :
	Shape <Circle> (
		from_center,
		rotation,
		scale_factor,
		current_pos_x,
		current_pos_y
	)
{
	possibilities = {
		Pixel ( '_', "blue", "green_neon" ),
		Pixel ( '-', "blue", "green_neon" ),
		Pixel ( '|', "blue", "green_neon" ),
		Pixel ( '/', "blue", "green_neon" ),
		Pixel ( '\\', "blue", "green_neon" )
	};
	cout << "VIS" << (_getPixel (0, 1)).visualize() << endl;
}