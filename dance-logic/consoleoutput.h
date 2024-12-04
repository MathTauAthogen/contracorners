#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

#include <string>
#include <cmath>
#include <numbers>

using namespace std;

#include "drawable.h"

class Output {
private:
	static const int _default_console_width = experimental_width;
	static const int _default_console_height = experimental_height;

	int _console_width;
	int _console_height;

	Grid < Pixel > _last_image;

	Output ( int width, int height );

	~Output(){};

	static Output* _theSingletonObject;

public:

	Output ( const Output& ) = delete;
	Output* operator= ( const Output& ) = delete;

	static Output* initialize_or_return_singleton ( int width = _default_console_width, int height = _default_console_height );
	static void clear();
	
	Grid <Pixel> get_last_image();
	
	int get_width();
	int get_height();

	static bool drawFrame ( Frame frame );
	static bool drawFrameEdit ( Frame frame, bool prevdrawn = false );

};

//extern Output* Output::_theSingletonObject; // defined in consoleoutput.cpp

#endif