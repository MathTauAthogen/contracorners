#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <numbers>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>

//#include "dancebase.cpp"

//#include "shapes.h"
#include "drawabletypes.h"
#include "consoleoutput.h"

using namespace std;

atomic < int > quit { 0 };

void thread_1 ()
{
	//std::string filename = "params.txt";
	//std::fstream s = fstream(filename);

	//if (!s.is_open()){
	//	std::cout << "Failed to open!\n";
	//	return 0;
	//};

	//std::map<std::string, std::string> dance_moves = {
	//	{"",""},
	//	{"",""},
	//	{"",""}
	//};

	vector<vector<std::string>> dancer_names = {
		{"Holly", "Jolly"},
		{"Molly", "Polly"},
		{"Dolly", "Lolly"},
	};

	/*
	Dancers dancers = Dancers();
	for(auto i : dancer_names){
		dancers.add_to_set(i);
	}
	for (int i = 0; i < dancers.get_names().size(); i ++){
		//for(auto j : dancers.get_names()[i].names){ ///////////////////////////////////////////////////////////////// TODO: FIGURE OUT WHY THIS BLOWS ITSELF UP
		for(int j = 0; j < dancers.get_names()[i].names.size(); j++){
			std::cout << dancers.get_names()[i].names[j] << " ";
		}
		std::cout  << "\n";

	};
	*/

	Grid<Pixel> image(7, 6);
	
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

	Frame output_frame = Frame(false);
	ImageDrawable drawing2 = ImageDrawable(image, true);
	Output::drawFrameEdit(output_frame, false);

	Output::initialize_or_return_singleton();
	auto start = std::chrono::system_clock::now();
	int count = 0;
	const double pi = std::numbers::pi;
	while(true){
		
		if ( quit.load() == 1 )
		{
			count = 0;
			quit.store ( 0 );
			//return;
		}
		else if ( quit.load() == -1 ) 
		{
			return;
		}

		output_frame.clear();
		float scaled_count = count / 100.0;
		drawing2.move(50 + 50 * cos(scaled_count/10 * 2 * pi),150 + 50 * sin(scaled_count/10 * 2 * pi), 8, scaled_count);
		output_frame.draw_on(drawing2);
		output_frame.change_pixel(Pixel((char)'%'), 50, 150);
		while((std::chrono::system_clock::now() - start) < count * 80ms){
			std::this_thread::sleep_for(1ms);
			cout << "OUT" << endl;
		}
		count += 1;
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
