#ifndef DBLE_TYPES_H
#define DBLE_TYPES_H

#include "drawable.cpp"


using namespace std;

class ImageDrawable : public Drawable < ImageDrawable > {

	private:

		Grid < Pixel > _image;
		
		const int _rows, _cols; // image size

	public:

		ImageDrawable (
			Grid < Pixel > image,
			bool from_centre = false,
			float rotate = 0,
			float scale_factor = 1,
			float current_pos_x = 0,
			float current_pos_y = 0
			) :
			_image ( image ),
			_rows ( image.height() ),
			_cols ( image.width() ),

			Drawable < ImageDrawable > (
				from_centre,
				rotate,
				current_pos_x,
				current_pos_y,
				image.height() / 2.0,
				image.width() / 2.0			
			)
		{
			cout << "INHERE2" << endl;
			cout << _image.height() << endl;
			cout << _image [1, 1].visualize() << endl;
			cout << "OUTHERE2" << endl;
		}

		Pixel get_pixel ( int x, int y )
		{
			auto [ adjusted_x, adjusted_y ] = Drawable < ImageDrawable > :: transform ( x, y );

			if( adjusted_x == -1 && adjusted_y == -1 ) // Transform threw an error because the scale_factor was 0
			{
				return _nullpix;
			}
			cout << "SEG1" << endl;
			cout << _image.height() << endl;
			return _image [ round ( adjusted_x ), round ( adjusted_y ) ];
		}
};

#endif