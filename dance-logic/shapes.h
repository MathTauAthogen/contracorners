#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <cmath>

#include "drawable.h"

class Shape : public Drawable {
	
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
		virtual Pixel _getPixel (int x, int y) = 0; 
};


class Circle : public Shape {

	private:

		std::vector < Pixel > possibilities;

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