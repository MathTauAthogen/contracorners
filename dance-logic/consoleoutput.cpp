#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <numbers>
#include <functional>

#include "drawable.cpp"

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


// THE BELOW VALUES HAVE BEEN MOVED TO drawable.cpp, EDIT THEM THERE	

/* Values for Avenir Next LT Pro

// EXPERIMENTAL VALUES FOR MY DISPLAY
const int experimental_width = 400;
const int experimental_height = 100;
*/


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
			cout << "\x1B[2J\x1B[H";
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

			cout << string(2 + frame.get_width(), '-') << endl;

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

			cout << string(2 + frame.get_width(), '-') << endl;

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