#ifndef DBLE_H
#define DBLE_H

#include <vector>
#include <numbers>
#include <cmath>

#include "pixel.h"
#include "grid.h"

/* Values for Avenir Next LT Pro

// EXPERIMENTAL VALUES FOR MY DISPLAY
const int experimental_width = 400;
const int experimental_height = 100;
*/

const int experimental_width = 470;
const int experimental_height = 140;

template < typename T > // For compile-time polymorphism
class Drawable;




class Frame {

	private:

		static const int _default_frame_width = experimental_width;
		static const int _default_frame_height = experimental_height;
		
		int _frame_width;
		int _frame_height;
		
		static constexpr Pixel _nullpix { true };
		static constexpr Pixel _blankpix { false };
		
		Grid < Pixel > _rows;

	public:

		Frame(
			bool use_blank = true,
			int width = _default_frame_width,
			int height = _default_frame_height
			);

		void clear ( bool use_blank = true );


		int get_width();
		int get_height();


		Grid < Pixel > get_rows();


		Pixel get_pixel ( int i, int j );


		bool change_pixel ( Pixel pixel, int ind_x, int ind_y );


		std::pair < bool, Frame > mergeFrames ( Frame frame1, Frame frame2 );

		bool merge_and_assign ( Frame frame );

		template < typename T >
		bool draw_on ( Drawable < T > drawable );

		template < typename T >
		bool draw_on ( std::vector < Drawable < T > > drawable );
};





template < typename T > // For compile-time polymorphism
class Drawable {

	protected:

		static constexpr double pi = std::numbers::pi;

		float _current_pos_x, _current_pos_y;
		
		float _scale_factor; // If negative, will be flipped.

		float _rotation;
		bool _from_centre;
		
		Frame _internal_frame = Frame ( false );

		const float _centre_offset_x;
		const float _centre_offset_y;

		float offset_x;
		float offset_y;

		Pixel _nullpix { true }; // TODO: Apparently this is bad when Pixel has any string members and is static, but only when the class it is in is templated, and somehow only with this name or something? I genuinely don't know what is causing the segfaults.
		
	public:

		// CONSTRUCTOR
		Drawable (
			bool from_centre = false,
			float rotate = 0,
			float scale_factor = 1,
			float current_pos_x = 0,
			float current_pos_y = 0,
			float centre_offset_x = 0,
			float centre_offset_y = 0
			);

		Frame& get_frame();

		std::pair < float, float > transform ( float x, float y );

		Pixel get_pixel ( int x, int y );

		bool move ( float x = 0, float y = 0, float scale = 1, float rotation = 0 );
};









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
inline bool Frame::draw_on ( std::vector < Drawable < T > > drawables )
{
	bool changed_anything = false, is_changed = false;

	typename std::vector < Drawable < T > >::iterator count = drawables.begin(), end = drawables.end();

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
	std::cout << "INHERE" << std::endl;
}


template < typename T >			
Frame& Drawable<T>::get_frame()
{
	return _internal_frame;
}


template < typename T >
std::pair < float, float > Drawable<T>::transform ( float x, float y )
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

//#include "drawabletypes.h"

//template class Drawable<ImageDrawable>;

//#include "shapes.h"

//template class Drawable<Shape<Circle>>;

#endif