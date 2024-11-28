#ifndef PIX_H
#define PIX_H

#include <string>
#include <map>
#include <vector>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<numbers>
#include<functional>

using namespace std;

/*

Details of output:

I will make a graphical output later, but for now the output is to console.

Output is the class containing the methods to actually put things onto the screen (aka console). It is a singleton because we only want one output, and it has blit methods to take an image, delete the current display, and replace it with the supplied image.

Frame is where the images are made. It represents a slice of Output, which gets passed to Output in a draw request.

Each drawable entity (dancers, arms, etc) inherits from Drawable. Thus, any drawable entity has a .draw() method which can either take a Frame or not. If a frame is supplied, it draws to that frame. Each drawable object also gets its own Frame by default, which can be set. If .draw() isn't given a Frame,
it defaults to drawing on the object's own Frame.

We also have a MovingFrame class. This class receives a vector of MovingDrawable, which is a class containing both a Drawable and a heap called futures of the times and positions it needs to be drawn, sorted by next time first. MovingFrame runs an internal clock, and asks Output to draw frames at a specified interval. In between releases,
it asks each MovingDrawable to pop from its futures until it gets to a time at or after the current time, with time 0 being the time the MovingFrame was made. If that MovingDrawable needs to be drawn now, it does so, and if the heap is empty, it removes this MovingDrawable from consideration.

Output can garbage collect its own Frames if asked, so we needn't worry about that, but we can also pass by reference if we want to (but Output will lock the Frame while processing so we can't make the next frame). Thus, we pass a copy instead.

*/


/* Values for Avenir Next LT Pro

// EXPERIMENTAL VALUES FOR MY DISPLAY
const int experimental_width = 400;
const int experimental_height = 100;
*/

const int experimental_width = 470;
const int experimental_height = 140;

unordered_map<std::string, std::string> color_codes = {
	{"white", "15"},{"black", "0"},{"blue", "21"},{"navy", "12"},{"green", "2"},{"green_neon", "10"}, {"clear_fg", "2m\033[39"}, {"clear_bg", "2m\033[49"}
}; // These are 256 bit codes.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template < typename T >
class Grid {
	private:
		vector < vector < T > > _grid;
		
		int _grid_height;
		int _grid_width;
		
		static constexpr int default_grid_width = 10; // These are arbitrary, shouldn't ever really be used.
		static constexpr int default_grid_height = 10; // These are arbitrary, shouldn't ever really be used.
		
		static constexpr T default_null_T_obj = T ( -1 );
		static constexpr T* default_null_T = const_cast < T* > ( &default_null_T_obj );
		/* TODO: WHY THE HELL DOES THIS WORK? BUT IT DOES SOMEHOW SO LEAVE IT */


		T _null_T;

	public:
		Grid() :
			_grid_height(0),
			_grid_width(0)
		{
			cout << "GRIDINIT1 " << _grid_height << " and " << _grid_width << endl;
		}

		Grid (
			int width,
			int height,
			T* null_T = default_null_T
			) :
			_grid_height ( height ),
			_grid_width ( width ),
			_null_T ( *null_T )
		{
			_grid = vector (
					_grid_height,
					vector < T > (
						_grid_width,
						_null_T
					)
				);
			cout << "GRIDINIT2 " << _grid_height << " and " << _grid_width << endl;
		}


		int width()
		{
			return _grid_width;
		};


		int height()
		{
			return _grid_height;
		}

		void reset_grid (
			bool same_dims = true, // This makes it hard to accidentally use default values, because they only happen if you already filled in an argument (so you won't accidentally run with no arguments and be surprised)
			int width = default_grid_width,
			int height = default_grid_height
			)
		{ 
			cout << "GRIDINIT5 " << _grid_height << " and " << _grid_width << endl;
			if ( !same_dims )
			{
				_grid_height = height;
				_grid_width = width;
			}

			_grid = vector ( _grid_height, vector < T > ( _grid_width, _null_T ) );
		};

		bool initialize_with (
			std::function < T ( int, int ) > func,
			bool change = false,
			int width = default_grid_width,
			int height = default_grid_height
			)
		{
			cout << "GRIDINIT4 " << _grid_height << " and " << _grid_width << endl;
			if (
				change &&
					( 
						_grid_height != height || 
						_grid_width != width
					)
				)
			{
				_grid_height = height;
				_grid_width = width;
				_grid = vector ( height, vector < T > ( width, _null_T ) );
			}

			for ( int i = 0; i < _grid_height; i ++ )
			{
				for ( int j = 0; j < _grid_width; j ++ )
				{
					( _grid ) [i] [j] = func ( i, j );
				}
			}

			return true;
		}

		bool initialize_with (
			T val,
			bool change = false,
			int width = default_grid_width,
			int height = default_grid_height
			)
		{
			cout << "GRIDINIT3 " << _grid_height << " and " << _grid_width << endl;
			if (
				change &&
					( 
						_grid_height != height || 
						_grid_width != width
					)
				)
			{
				_grid_height = height;
				_grid_width = width;
				_grid = vector ( height, vector < T > ( width, _null_T ) );
			}

			for ( int i = 0; i < _grid_height; i ++ )
			{
				for ( int j = 0; j < _grid_width; j ++ )
				{
					( _grid ) [i] [j] = val;
				}
			}

			return true;
		}

		T & operator[] (
			size_t index1,
			size_t index2
			)
		{ // I made this operator to make sure the grid is kept square. I don't want to ever allow a push_back or erase, I'd only like them to be able to get or set 

			if (
				0 <= index1
				&& index1 < _grid_height
				&& 0 <= index2
				&& index2 < _grid_width
				)
			{ 
				if( index1 == 1 || index1 == 2){
					cout << index1 << " and " << index2 << " and " << _grid_height << " and " << _grid_width << " and " << _grid.size() << endl;// " and " << _grid[0].size() << endl;
				}
				return ( _grid ) [index1] [index2];
			}

			else {
				return _null_T;
			}

		};

};

struct Pixel {

	char value;
	std::string bg_color;
	std::string fg_color;

	static const char default_value = ' ';
	static constexpr std::string default_bg_color = "clear_bg";
	static constexpr std::string default_fg_color = "white";

	bool blank = true;
	bool null = false;

	constexpr Pixel ( int bool_blank_null )
	:
		value ( default_value ),
		fg_color ( default_bg_color ),
		bg_color ( default_bg_color )
	{
		if ( bool_blank_null == 0 )
		{
			blank = true;
			null = false;
		}
		else
		{
			blank = true;
			null = true;
		}
	}

	explicit Pixel (
		char new_value = default_value,
		std::string new_bg_color = default_bg_color,
		std::string new_fg_color = default_fg_color
		) :
		value ( new_value ),
		fg_color ( new_fg_color ),
		bg_color ( new_bg_color )
	{
		blank = blank && (value == default_value);
		blank = blank && (fg_color == default_fg_color);
		blank = blank && (bg_color == default_bg_color);
	}

	bool check_valid_pixel()
	{
		if (
			!color_codes.contains(bg_color) ||
			!color_codes.contains(fg_color)
			)
		{
			return false;
		}
		return true;
	}
	std::string visualize(){
		return 
			"\033[38;5;"
			+ color_codes[fg_color]
			+ ";48;5;"
			+ color_codes[bg_color]
			+ "m"
			+ std::string(1, value)
			+ "\033[0m";
	}
	bool is_blank(){
		return blank || null;
	}
	bool is_null(){
		return null;
	}
	bool define_blank(){
		return (!blank) && (blank = true);
	}
	bool assign (
		char new_value = default_value,
		std::string new_bg_color = default_bg_color,
		std::string new_fg_color = default_fg_color
		)
	{
		blank = blank && (value == new_value);
		blank = blank && (fg_color == new_fg_color);
		blank = blank && (bg_color == new_bg_color);
		value = new_value;
		bg_color = new_bg_color;
		fg_color = new_fg_color;
		return blank;
	}

	bool erase(){
		value = default_value;
		bg_color = default_bg_color;
		fg_color = default_fg_color;
		return (!blank) && (blank = true);
	}
};

bool operator!=(const Pixel& pix1, const Pixel&pix2){
	return (pix1.value != pix2.value) || (pix1.bg_color != pix2.bg_color) || (pix1.fg_color != pix2.fg_color) || (pix1.blank != pix2.blank) || (pix1.null != pix2.null);
}

template < typename T > // For compile-time polymorphism
class Drawable;

class Frame {

	private:

		static const int _default_frame_width = experimental_width;
		static const int _default_frame_height = experimental_height;
		
		int _frame_width;
		int _frame_height;
		
		static constexpr Pixel _nullpix_obj = Pixel ( -1 );
		static constexpr Pixel* _nullpix = const_cast < Pixel* > ( &_nullpix_obj );
		/* TODO: WHY THE HELL DOES THIS WORK? BUT IT DOES SOMEHOW SO LEAVE IT */
		
		Grid < Pixel > _rows;

		static Pixel make_default_pixel ( int i = -1, int j = -1 ) // The superfluous ints are to work with the grid initialize_with method.
		{
			return Pixel ( 0 );
		}

		static Pixel make_nullpix ( int i = -1, int j = -1 ) // The superfluous ints are to work with the grid initialize_with method.
		{
			return _nullpix_obj;
		}

	public:

		Frame(
			bool use_blank = true,
			int width = _default_frame_width,
			int height = _default_frame_height
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
				_rows.initialize_with ( make_default_pixel );
			}
		}


		void clear ( bool use_blank = true )
		{
			
			if ( use_blank )
			{
				_rows.initialize_with ( make_default_pixel );
			}
			else {
				_rows.initialize_with ( _nullpix_obj );
			}


		}

		int get_width()
		{
			return _frame_width;
		}

		int get_height()
		{
			return _frame_height;
		}

		Grid < Pixel > get_rows()
		{
			return _rows;
		}

		Pixel get_pixel ( int i, int j )
		{
			return _rows [ i, j ];
		}


		bool change_pixel ( Pixel pixel, int ind_x, int ind_y )  /* This might do better as private but I'll make it so when I know it is easy to draw in other ways. This is mostly an internal method. */
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


		pair < bool, Frame > mergeFrames ( Frame frame1, Frame frame2 ) // We do this in-place on frame1, as we are passing by value.
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

		bool merge_and_assign ( Frame frame )
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

		template < typename T >
		bool draw_on ( Drawable < T > drawable ); // DEFINED AFTER DRAWABLE

		template < typename T >
		bool draw_on ( vector < Drawable < T > > drawable ); // DEFINED AFTER DRAWABLE
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

		Pixel _nullpix_obj { -1 }; // TODO: Apparently this is bad when Pixel has any string members and is static, but only when the class it is in is templated, and somehow only with this name or something? I genuinely don't know what is causing the segfaults.

		//static constexpr Pixel _nullpix_obj { -1 };
		//static constexpr Pixel* _nullpix = const_cast < Pixel* > ( &_nullpix_obj );
		/* TODO: WHY THE HELL DOES THIS WORK? BUT IT DOES SOMEHOW SO LEAVE IT */
		
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


		Frame& get_frame()
		{
			return _internal_frame;
		}


		pair < float, float > transform ( float x, float y )
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

			x = offset_x * cos( -2 * pi * _rotation )
				- offset_y * sin( -2 * pi * _rotation );

			y = offset_x * sin( -2 * pi * _rotation )
				+ offset_y * cos( -2 * pi * _rotation );


			// Recentre

			x += _centre_offset_x;
			y += _centre_offset_y;

			return {x, y};
		}

		Pixel get_pixel ( int x, int y )
		{
			return static_cast < T* > ( this ) -> get_pixel ( x, y );
		}


		bool move ( float x = 0, float y = 0, float scale = 1, float rotation = 0 )
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
};

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
				return _nullpix_obj;
			}
			cout << "SEG1" << endl;
			cout << _image.height() << endl;
			return _image [ round ( adjusted_x ), round ( adjusted_y ) ];
		}
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Output {
	private:

		static const int _default_console_width = experimental_width;
		static const int _default_console_height = experimental_height;

		int _console_width;
		int _console_height;

		Grid < Pixel > _last_image;

		Output (
			int width,
			int height
			) :
			_console_width ( width ),
			_console_height ( height ),
			_last_image ( 
				Grid < Pixel > (
					width,
					height
				)
			)
		{
			cout << "\033[?25l"; // Make the cursor invisible
		};

		~Output(){};

		static Output* _theSingletonObject;

	public:

		Output ( const Output& ) = delete;
		Output* operator= ( const Output& ) = delete;

		static Output* initialize_or_return_singleton ( int width = _default_console_width, int height = _default_console_height )
		{
			if( _theSingletonObject == nullptr )
			{
				_theSingletonObject = new Output ( width, height );
			}

			return _theSingletonObject;
		}


		static void clear()
		{
			std::cout << "\x1B[2J\x1B[H";
		}


		Grid <Pixel> get_last_image()
		{
			return _last_image;
		}

		int get_width()
		{
			return _console_width;
		}

		int get_height()
		{
			return _console_height;
		}
		

		static bool drawFrame ( Frame frame )
		{
			clear();

			cout << std::string(2 + frame.get_width(), '-') << endl;

			Grid < Pixel > last_image = initialize_or_return_singleton()
				-> get_last_image();

			Grid < Pixel > rows = frame.get_rows();

			for( int i = 0; i < rows.height(); i ++ )
			{
				cout << "|";

				for(int j = 0; j < rows.width(); j ++ )
				{
					last_image[i, j] = rows[i, j];
					cout << rows[i, j].visualize();
				}

				cout << "|" << endl;
			}

			cout << std::string(2 + frame.get_width(), '-') << endl;

			return true;
		}


		static bool drawFrameEdit ( Frame frame, bool prevdrawn = false )
		{
			if( !prevdrawn )
			{
				return drawFrame( frame );
			}

			Grid < Pixel > last_image = initialize_or_return_singleton()
				-> get_last_image();

			Grid < Pixel > rows = frame.get_rows();

			int height = rows.height();
			int width = rows.width();

			for( int i = 0; i < height; i ++ )
			{
				for(int j = 0; j < width; j ++ )
				{
					if( last_image[i, j] != rows[i, j] )
					{
						last_image[i, j] = rows[i, j];
						cout << "\033[" << ( i + 2 ) << ";" << ( j + 2 ) << "H";
						cout << rows[i, j].visualize();
					}
				}
			}

			return true;
		}
};

Output* Output::_theSingletonObject = nullptr;

#endif