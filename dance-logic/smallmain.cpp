#include <iostream>
#include <string>
#include <vector>
#include<chrono>
#include<thread>
#include <atomic>


#include "consoleoutput.h"
#include "shapes.h"

using namespace std;

atomic < int > quit { 0 };

void thread_1 ()
{

	Grid<Pixel> image (7, 6);
	
	char crude_drawing[6][7] = {
		{' ','/','-','-','-','\\', ' '},
		{'/','|','O',' ','O','|', '\\'},
		{'|','|',' ',' ',' ','|', '|'},
		{'|','|',' ',' ',' ','|', '|'},
		{'\\','|','\\','_','/','|', '/'},
		{' ','\\','_','_','_','/', ' '},
	};

	for(int i = 0; i < 6; i ++){
		for(int j = 0; j < 7; j++){
			if(crude_drawing[i][j] != ' '){
				image[i, j] = Pixel(crude_drawing[i][j], "blue", "green");
			}
		}
	}

	cout << "IMAGING:" << image [0, 1].visualize() << endl;

	Frame output_frame = Frame (false);
	

	Drawable < Shape < Circle > > drawing = Circle(true, 5);

	cout << "IMAGING:" << drawing.get_pixel( 2, 2 ).visualize() << endl;

	Output::initialize_or_return_singleton();
	Output::drawFrameEdit(output_frame, false);	

	auto start = std::chrono::system_clock::now();

	int count;

	while( count += 1 ){
		
		if ( quit.load() == 1 )
		{
			quit.store ( 0 );
		}
		else if ( quit.load() == -1 ) 
		{
			return;
		}

		output_frame.clear();

		drawing.move (50, 50);
	
		output_frame.draw_on(drawing);

		while((std::chrono::system_clock::now() - start) < count * 80ms){
			std::this_thread::sleep_for(1ms);
		}

		Output::drawFrameEdit(output_frame, true);
	}
}

void thread_2 ()
{
	char the_char = getchar();
	
	while ( quit.load() != 0 )
	{
		this_thread::sleep_for (10ms);
	}

	if ( the_char == 'q' )
	{
		quit.store( -1 );
	}
	else
	{	
		quit.store( 1 );
	}
	
}

int main ()
{

	thread t1 = thread (thread_1);
	while ( quit.load() != -1 )
	{
			thread t2 = thread (thread_2);
			t2.join();
	}
	t1.join();
	Output::clear();
}
