#include "consoleoutput.h"

// THE BELOW VALUES HAVE BEEN MOVED TO drawable.cpp, EDIT THEM THERE	

/* Values for Avenir Next LT Pro

// EXPERIMENTAL VALUES FOR MY DISPLAY
const int experimental_width = 400;
const int experimental_height = 100;
*/

Output* Output::_theSingletonObject = nullptr;

using namespace std;

recursive_mutex mut{};

Output::Output (
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


Output* Output::initialize_or_return_singleton ( int width, int height ) // Default values in header file
{
	if( _theSingletonObject == nullptr )
	{
		_theSingletonObject = new Output ( width, height );
	}

	return _theSingletonObject;
}


void Output::clear()
{
	cout << "\x1B[2J\x1B[H";
}


Grid <Pixel> Output::get_last_image()
{
	return _last_image;
}

int Output::get_width()
{
	return _console_width;
}

int Output::get_height()
{
	return _console_height;
}


bool Output::drawFrame ( Frame frame )
{
	unique_lock locked {mut};
	ostringstream full{};
	clear();

	full << string(2 + frame.get_width(), '-') << endl;

	Grid < Pixel > last_image = initialize_or_return_singleton()
		-> get_last_image();

	Grid < Pixel > rows = frame.get_rows();

	for( int i = 0; i < rows.height(); i ++ )
	{
		full << "|";

		for(int j = 0; j < rows.width(); j ++ )
		{
			last_image[i, j] = rows[i, j];
			full << rows[i, j].visualize();
		}

		full << "|" << endl;
	}

	full << string(2 + frame.get_width(), '-') << endl;

	cout << full.str();

	return true;
}


bool Output::drawFrameEdit ( Frame frame, bool prevdrawn ) // prevdrawn defaults to false in header
{
	unique_lock locked {mut};
	if( !prevdrawn )
	{
		return drawFrame( frame );
	}

	Grid < Pixel > last_image = initialize_or_return_singleton()
		-> get_last_image();

	Grid < Pixel > rows = frame.get_rows();

	int height = rows.height();
	int width = rows.width();
	bool going;
	string running;
	ostringstream full{};
	int ystart;

	for( int i = 0; i < height; i ++ )
	{
		going = false;
		running = "";

		for(int j = 0; j < width; j ++ )
		{
			if( last_image[i, j] != rows[i, j] )
			{
				if (!going) {ystart = j;}
				going = true;
				last_image[i, j] = rows[i, j];
				running += rows[i, j].visualize();
				continue;
				
			}
			if (going) {
				full << "\033[" << ( i + 2 ) << ";" << ( ystart + 2 ) << "H";
				full << running;
				running = "";
				going = false;
			}
		}
		if (going) {
			full << "\033[" << ( i + 2 ) << ";" << ( ystart + 2 ) << "H";
			full << running;
			running = "";
			going = false;
		}
	}

	cout << full.str();

	return true;
}