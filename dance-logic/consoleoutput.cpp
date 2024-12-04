#include "consoleoutput.h"

// THE BELOW VALUES HAVE BEEN MOVED TO drawable.cpp, EDIT THEM THERE	

/* Values for Avenir Next LT Pro

// EXPERIMENTAL VALUES FOR MY DISPLAY
const int experimental_width = 400;
const int experimental_height = 100;
*/

Output* Output::_theSingletonObject = nullptr;

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


bool Output::drawFrameEdit ( Frame frame, bool prevdrawn ) // prevdrawn defaults to false in header
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