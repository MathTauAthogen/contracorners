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

class Shape : public Drawable {
	
	protected:

		std::function < Pixel ( float, float ) > _getPixel;
	
	public:

		Shape (
			std::function < Pixel ( float, float ) > get_the_pixel,
			bool from_center = false,
			float rotate = 0.0,
			float scale_factor = 1.0,
			float current_pos_x = 0, 
			float current_pos_y = 0
			) :
			Drawable(
				Grid < Pixel > (),
				from_center,
				rotate,
				scale_factor,
				current_pos_x,
				current_pos_y
			),
			_getPixel ( get_the_pixel )
		{}


		virtual Pixel get_pixel ( int i, int j ) override
		{
			auto [x, y] = transform ( i, j );

			if( x == -1 && y == -1 )
			{
				return _nullpix_obj;
			}
			
			return _getPixel ( x, y );
		}
};

class Circle : public Shape {

	private:

		vector < Pixel > possibilities;
	
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

					float approx_const = 0.4;
					float dist = sqrt ( pow( x, 2 ) + pow ( y, 2 ) );
					float error = approx_const / _scale_factor;

					if (
						1 - error <= dist && 
						dist <= 1 + error
						)
					{

						float side_const = 0.15;

						if ( x > 0 && abs(y / x) < side_const )
						{
							return possibilities[0];
						}

						else if ( x < 0 && abs(y / x) < side_const )
						{
							return possibilities[1];
						}

						else if ( y != 0 && abs(x / y)  < side_const )
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