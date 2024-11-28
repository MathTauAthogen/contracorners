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
#include "drawabletypes.cpp"

template < typename T >
class Shape : public Drawable < Shape < T > > {
	
	protected:

		std::function < Pixel ( float, float ) > _getPixel;

		static constexpr float _approx_const = 0.4;
		static constexpr float _slope_const = 0.15;

		//Drawable < Shape < T > > * newthis = static_cast < Drawable < Shape < T > > * > (this);
	
	public:

		Shape (
			//std::function < Pixel ( float, float ) > get_the_pixel,
			bool from_center = false,
			float rotate = 0,
			float scale_factor = 1,
			float current_pos_x = 0, 
			float current_pos_y = 0
			) :
			Drawable < Shape < T > > (
				from_center,
				rotate,
				scale_factor,
				current_pos_x,
				current_pos_y
			)//,
			//_getPixel ( get_the_pixel )
		{			cout << "VISSY" << (_getPixel (1, 1)).visualize() << endl;  }


		Pixel get_pixel ( int i, int j )
		{
			auto [x, y] = Drawable <Shape < T > >::transform ( i, j );

			if( x == -1 && y == -1 )
			{
				return Drawable <Shape < T > >::_nullpix_obj;
			}
			
			cout << "HEREERERER" << endl;
			return static_cast < Drawable < Shape < T > > * > (this) -> _getPixel ( x, y );
		}

};

class Circle : public Shape < Circle > {

	private:

		vector < Pixel > possibilities;
		float dist;
		float error;

		Pixel _getPixel (float x, float y)
		{

			dist = sqrt ( pow( x, 2 ) + pow ( y, 2 ) );
			error = Shape < Circle > ::  _approx_const / Shape < Circle > :: _scale_factor;

			if (
				1 - error <= dist && 
				dist <= 1 + error
				)
			{

				// De-rotate for correct symbol for real-life slope

				if( Shape < Circle > :: _from_centre ){ x += Shape < Circle > :: _centre_offset_x; y += Shape < Circle > ::  _centre_offset_y; }

				Shape < Circle > :: offset_x = x - Shape < Circle > :: _centre_offset_x;
				Shape < Circle > :: offset_y = y - Shape < Circle > :: _centre_offset_y;


				x = Shape < Circle > :: offset_x * cos( 2 * pi * Shape < Circle > :: _rotation )
					- Shape < Circle > :: offset_y * sin( 2 * pi * Shape < Circle > :: _rotation );

				y = Shape < Circle > :: offset_x * sin( 2 * pi * Shape < Circle > :: _rotation )
					+ Shape < Circle > :: offset_y * cos( 2 * pi * Shape < Circle > :: _rotation );

				x += Shape < Circle > :: _centre_offset_x;
				y += Shape < Circle > :: _centre_offset_y;
				
				// End de-rotate

				if ( x > 0 && abs(y / x) < 	Shape < Circle > :: _slope_const )
				{
					return possibilities[0];
				}

				else if ( x < 0 && abs(y / x) < Shape < Circle > :: _slope_const )
				{
					return possibilities[1];
				}

				else if ( y != 0 && abs(x / y)  < Shape < Circle > :: _slope_const )
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
					return Shape < Circle > :: _nullpix;
				}

				else {
					cout << "WHAT?" << x << " and " << y << endl;
				}
				return Shape < Circle > :: _nullpix;
			}
			else{
				return Shape < Circle > :: _nullpix;
			}

		}
	
	public:
		
		Circle (
			bool from_center = false,
			float scale_factor = 1.0,
			float current_pos_x = 0,
			float current_pos_y = 0,
			float rotation = 0
			) :
			Shape <Circle> (

				//s_getPixel,

				from_center,
				rotation,
				scale_factor,
				current_pos_x,
				current_pos_y
			)
		{
			cout << "VIS" << (Shape < Circle > :: _getPixel (1, 1)).visualize() << endl;
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