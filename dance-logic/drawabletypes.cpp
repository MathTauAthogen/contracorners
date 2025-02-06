#include "drawabletypes.h"

using namespace std;

ImageDrawable::ImageDrawable (
	Grid < Pixel > & image,
	bool from_centre,
	float rotate,
	float scale_factor,
	float current_pos_x,
	float current_pos_y
	) :
	Drawable (
		from_centre,
		rotate,
		current_pos_x,
		current_pos_y,
		image.height() / 2.0,
		image.width() / 2.0			
	),

	_image ( image ),
	_rows ( image.height() ),
	_cols ( image.width() )
{
}

Pixel ImageDrawable::get_pixel ( int x, int y )
{
	auto [ adjusted_x, adjusted_y ] = transform ( x, y );

	if( adjusted_x == -1 && adjusted_y == -1 ) // Transform threw an error because the scale_factor was 0
	{
		return _nullpix;
	}
	return _image [ round ( adjusted_x ), round ( adjusted_y ) ];
}