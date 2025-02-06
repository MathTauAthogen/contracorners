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
	Drawable < ImageDrawable > (
		from_centre,
		rotate,
		current_pos_x,
		current_pos_y,
		image.height() / 2.0,
		image.width() / 2.0			
	),

	_image ( image ),
	_rows ( image.height() ),
	_cols ( image.width() ),
	isntshit ( 5 )
{
	cout << "INHERE2" << endl;
	_image.print_diag("INSIDE_IMAGE_DRAWABLE1");
	cout << _image.height() << endl;
	cout << _image [1, 1].visualize() << endl;
	cout << static_cast < ImageDrawable* >(this)->isntshit << " ISN'TSHIT INSIDE" << endl;
	isntshit = 7;
	cout << "OUTHERE2" << endl;
}



Pixel ImageDrawable::get_pixel ( int x, int y )
{
	//_image.print_diag("INSIDE_IMAGE_DRAWABLE");
	cout << "ONE " << isntshit << endl;
	(static_cast < ImageDrawable* >(this)->_image).print_diag("INSIDE_IMAGE_DRAWABLE");
	cout << "TWO" << endl;
	auto [ adjusted_x, adjusted_y ] = Drawable < ImageDrawable > :: transform ( x, y );

	if( adjusted_x == -1 && adjusted_y == -1 ) // Transform threw an error because the scale_factor was 0
	{
		return _nullpix;
	}
	cout << "SEG1" << endl;
	cout << _image.height() << endl;
	return _image [ round ( adjusted_x ), round ( adjusted_y ) ];
}