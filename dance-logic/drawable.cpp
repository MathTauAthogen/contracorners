#include "drawable.h"

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

///////////////////////////////////// DRAW_ON DEFINITION /////////////////////////////////////////////////////////////////////////////

template < typename T >
inline bool Frame::draw_on ( Drawable < T > drawable )
{
	bool changed_anything = false;

	for( int i = 0; i < _frame_height; i ++ )
	{
		for( int j = 0; j < _frame_width; j ++ )
		{
			Pixel pix = drawable.get_pixel ( i, j );

			changed_anything = change_pixel ( pix, i, j ) || changed_anything;
		}
	}

	return changed_anything;
}


template < typename T >
inline bool Frame::draw_on ( vector < Drawable < T > > drawables )
{
	bool changed_anything = false, is_changed = false;

	typename vector < Drawable < T > >::iterator count = drawables.begin(), end = drawables.end();

	for( int i = 0; i < _frame_height; i ++ )
	{
		for( int j = 0; j < _frame_width; j ++ )
		{
			is_changed = false;
			count = drawables.begin();

			while( !is_changed && count != end )
			{
				Pixel pix = ( *count ).get_pixel ( i, j );
				
				is_changed = change_pixel( pix, i, j );

				changed_anything = is_changed || changed_anything;
				count ++;
			}
		}
	}

	return changed_anything;
}





// DRAWABLE //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T >		
Drawable<T>::Drawable (
	bool from_centre,
	float rotate,
	float scale_factor,
	float current_pos_x,
	float current_pos_y,
	float centre_offset_x,
	float centre_offset_y
	) :
	_centre_offset_x ( centre_offset_x ),
	_centre_offset_y ( centre_offset_y ),
	_current_pos_x ( current_pos_x ),
	_current_pos_y ( current_pos_y ),
	_scale_factor ( scale_factor ),
	_rotation ( rotate ),
	_from_centre ( from_centre )
{
	cout << "INHERE" << endl;
}


template < typename T >			
Frame& Drawable<T>::get_frame()
{
	return _internal_frame;
}


template < typename T >
pair < float, float > Drawable<T>::transform ( float x, float y )
{

	return {x, y}; // TEMPORARY

	if( _scale_factor == 0 ){ return {-1, -1}; } // The default error value, shouldn't happen


	// First, shift into place.

	x = x - _current_pos_x;
	y = y - _current_pos_y;


	if( _scale_factor < 0 ){ 
		
		// Flip
		x -= 2.0 * _centre_offset_x; // Janky replacement for _rows and _cols
		y -= 2.0 * _centre_offset_y; // Janky replacement for _rows and _cols
	
	}

	// Rescale.

	x /= _scale_factor;
	y /= _scale_factor;


	// If measuring from the centre, shift so the centre is at 0,0.

	if( _from_centre ){ x += _centre_offset_x; y += _centre_offset_y; }

	// Now, rotate. We rotate about the centre of the image.
	// We have to rotate backwards. This is so that when we access the rotated image of (x, y), we instead access (x. y) as we would like.

	//Here, we define rotation = 1 as a full 360 degree rotation.

	offset_x = x - _centre_offset_x;
	offset_y = y - _centre_offset_y;

	x = offset_x * std::cos( -2 * pi * _rotation )
		- offset_y * std::sin( -2 * pi * _rotation );

	y = offset_x * std::sin( -2 * pi * _rotation )
		+ offset_y * std::cos( -2 * pi * _rotation );


	// Recentre

	x += _centre_offset_x;
	y += _centre_offset_y;

	return {x, y};
}



template < typename T >	
Pixel Drawable<T>::get_pixel ( int x, int y )
{
	return static_cast < T* > ( this ) -> get_pixel ( x, y );
}


template < typename T >	
bool Drawable<T>::move ( float x, float y, float scale, float rotation )
{
	if( scale == 0 )
	{
		return false;
	}

	_current_pos_x = x;
	_current_pos_y = y;
	_scale_factor = scale;
	_rotation = rotation;

	return true;
}