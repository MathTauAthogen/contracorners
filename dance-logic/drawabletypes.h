#ifndef DBLE_TYPES_H
#define DBLE_TYPES_H

#include <cmath>
#include "drawable.h"

class ImageDrawable : public Drawable {

	private:
		
		const int _rows, _cols; // image size

	public:

		Grid < Pixel > _image {15, 16};

		ImageDrawable (
			Grid < Pixel > & image,
			bool from_centre = false,
			float rotate = 0,
			float scale_factor = 1,
			float current_pos_x = 0,
			float current_pos_y = 0
			);


		Pixel get_pixel ( int x, int y );
};

#endif