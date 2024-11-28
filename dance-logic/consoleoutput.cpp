#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

#include <string>
#include <cmath>
#include <numbers>

#include "drawable.cpp"

using namespace std;

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