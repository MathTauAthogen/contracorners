#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <map>
#include <vector>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<numbers>
#include<functional>
#include "consoleoutput.cpp"

template < typename T >
class Shape : public Drawable < Shape < T > > {
	
	protected:

		std::function < Pixel ( float, float ) > _getPixel;

		const float _approx_const = 0.4;
		const float _slope_const = 0.15;

		Drawable < Shape < T > > * newthis = static_cast < Drawable < Shape < T > > * > (this);

		static constexpr Pixel _nullpix_obj = Pixel ( -1 );
		static constexpr Pixel* _nullpix = const_cast < Pixel* > ( &_nullpix_obj );
	
	public:

		Shape (
			std::function < Pixel ( float, float ) > get_the_pixel,
			bool from_center = false,
			float rotate = 0,
			float scale_factor = 1,
			float current_pos_x = 0, 
			float current_pos_y = 0
			) :
			Drawable < Shape > (
				from_center,
				rotate,
				scale_factor,
				current_pos_x,
				current_pos_y
			),
			_getPixel ( get_the_pixel )
		{}


		Pixel get_pixel ( int i, int j )
		{
			auto [x, y] = newthis -> transform ( i, j );

			if( x == -1 && y == -1 )
			{
				return _nullpix_obj;
			}
			
			return _getPixel ( x, y );
		}

};

class Circle : public Shape < Circle > {

	private:

		vector < Pixel > possibilities;
		float dist;
		float error;

		static constexpr Pixel _nullpix_obj = Pixel ( -1 );
		static constexpr Pixel* _nullpix = const_cast < Pixel* > ( &_nullpix_obj );
	
	public:
		
		Circle (
			bool from_center = false,
			float scale_factor = 1.0,
			float current_pos_x = 0,
			float current_pos_y = 0,
			float rotation = 0
			) :
			Shape(

				[&](float x, float y){

					dist = sqrt ( pow( x, 2 ) + pow ( y, 2 ) );
					error = _approx_const / _scale_factor;

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

						if ( x > 0 && abs(y / x) < _slope_const )
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
							return _nullpix_obj;
						}

						else {
							cout << "WHAT?" << x << " and " << y << endl;
						}
						return _nullpix_obj;
					}
					else{
						return _nullpix_obj;
					}

				}, // This is the first argument

				from_center,
				rotation,
				scale_factor,
				current_pos_x,
				current_pos_y
			)
		{
			possibilities = {
				Pixel ( '_', "blue", "green_neon" ),
				Pixel('-', "blue", "green_neon"),
				Pixel('|', "blue", "green_neon"),
				Pixel('/', "blue", "green_neon"),
				Pixel('\\', "blue", "green_neon")
			};
		}
};

#endif