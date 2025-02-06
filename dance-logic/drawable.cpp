#include "drawable.h"

using namespace std;

Frame::Frame(
	bool use_blank,
	int width,
	int height
	) : 
	_frame_width ( width ),
	_frame_height ( height ),

	_rows (
		Grid < Pixel > (
			width,
			height,
			_nullpix
		)
	)

{
	cout << "MADE_FRAME" << endl;
	if ( use_blank )
	{
		_rows.initialize_with ( _blankpix );
	}
}


void Frame::clear ( bool use_blank ) // Defaults to true in header
{
	
	if ( use_blank )
	{
		_rows.initialize_with ( _blankpix );
	}
	else {
		_rows.initialize_with ( _nullpix );
	}


}



int Frame::get_width()
{
	return _frame_width;
}

int Frame::get_height()
{
	return _frame_height;
}

Grid < Pixel > Frame::get_rows()
{
	return _rows;
}



Pixel Frame::get_pixel ( int i, int j )
{
	return _rows [ i, j ];
}



bool Frame::change_pixel ( Pixel pixel, int ind_x, int ind_y )  /* This might do better as private but I'll make it so when I know it is easy to draw in other ways. This is mostly an internal method. */
{
	if (
		! pixel.check_valid_pixel() || 
		pixel.is_blank()
		)
	{
		return false;
	}

	_rows [ ind_x, ind_y ] = pixel;
	return true;
}




pair < bool, Frame > Frame::mergeFrames ( Frame frame1, Frame frame2 ) // We do this in-place on frame1, as we are passing by value.
{
	if (
		frame1.get_width() != frame2.get_width() || 
		frame1.get_height() != frame2.get_height()
		)
	{
		return { false, Frame() };
	}

	else {
		Grid < Pixel > frame2_rows = frame2.get_rows();

		for ( int i = 0; i < frame1.get_height(); i ++ )
		{
			for ( int j = 0; j < frame1.get_width(); j ++ )
			{
				if( ! frame2_rows [ i, j ].is_blank() )
				{
					frame1.change_pixel ( frame2_rows[i, j], i, j );
				}
			}
		}

		return { true, frame1 };
	}	
}



bool Frame::merge_and_assign ( Frame frame )
{
	if ( 
		frame.get_width() != _frame_width ||
		frame.get_height() != _frame_height
		)
	{
		return false;
	}

	else {
		Grid < Pixel > frame_rows = frame.get_rows();

		for ( int i = 0; i < _frame_height; i ++ )
		{
			for ( int j = 0; j < _frame_width; j ++ )
			{
				if ( ! frame_rows [ i, j ].is_blank() )
				{
					change_pixel ( frame_rows[i, j], i, j );
				}
			}
		}
		return true;
	}
}