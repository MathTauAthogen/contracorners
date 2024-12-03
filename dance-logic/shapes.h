#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <cmath>

#include "drawable.h"

template < typename T >
class Shape : public Drawable < Shape < T > > {
	
	protected:

		static constexpr float _approx_const = 0.4;
		static constexpr float _slope_const = 0.15;
	
	public:

		Shape (
			bool from_center = false,
			float rotate = 0,
			float scale_factor = 1,
			float current_pos_x = 0, 
			float current_pos_y = 0
			);


		Pixel get_pixel ( int i, int j );

};






class Circle : public Shape < Circle > {

	private:

		vector < Pixel > possibilities;

		float dist;
		float error;

	public:

		Pixel _getPixel ( float x, float y );
		
		Circle (
			bool from_center = false,
			float scale_factor = 1.0,
			float current_pos_x = 0,
			float current_pos_y = 0,
			float rotation = 0
			);
};

#endif