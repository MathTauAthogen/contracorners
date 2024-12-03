#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<numbers>
#include<functional>
#include<chrono>
#include<thread>
#include <atomic>

#include "dancebase.cpp"

#include "consoleoutput.h"
#include "shapes.cpp"

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

	cout << "IMAGING                       :" << image [0, 1].visualize() << endl;

	Frame output_frame = Frame(false);
	//Drawable < ImageDrawable > drawing = ImageDrawable(image, true);
	//Drawable < Shape < Circle > > drawing = new Circle(true, 3, 50, 50); // Drawable
	//Drawable < Shape < Circle > > drawing2 = new Circle(true, 2, 50, 100); // Drawable
	//Drawable* drawing3 = new Circle(true, 1, 50, 150); // Drawable
	cout << "HEREING" << endl;
	Drawable < ImageDrawable > drawing2 = ImageDrawable(image, true);
	cout << static_cast < ImageDrawable* >(&drawing2)->isntshit << " ISN'TSHIT" << endl;
	//static_cast < ImageDrawable* >(&drawing2)->_image.print_diag("OUTSIDE");
	cout << "HEREING2" << endl;
	cout << "IMAGING                       :" << drawing2.get_pixel( 2, 2 ).visualize() << endl;
	cout << "HERE" << endl;
	cout << output_frame.draw_on(drawing2);
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
		//drawing.move(50 + 50 * cos(count / 1000.0 * 2 * pi),150 + 50 * sin(count / 1000.0 * 2 * pi), 8, 0.01 * count);//, 4.5, 0.1 * count);
		drawing2.move(50 + 50 * cos(scaled_count/10 * 2 * pi),150 + 50 * sin(scaled_count/10 * 2 * pi), 8, scaled_count);//, 4.5, 0.1 * count);
		//drawing2.move(50, 150, 1, 0.01 * count);//, 4.5, 0.1 * count);
		//drawing->move(50 + scaled_count, 50 + scaled_count * scaled_count, 3);
		//drawing->move(50 + scaled_count * 10, 50 + 100 * scaled_count * scaled_count, 20, scaled_count);
		//drawing.move(100 + scaled_count - 30 * pow(cos(scaled_count/10 * 2 * pi), 3), 100 + 25 * cos(scaled_count * 2 * pi), 5, scaled_count);
		//output_frame.draw_on(drawing3);
		output_frame.draw_on(drawing2);
		//output_frame.draw_on(drawing);
		//output_frame.draw_on({drawing, drawing2});
		//output_frame.draw_on({drawing, drawing2, drawing3});
		output_frame.change_pixel(Pixel((char)'%'), 50, 150);
		while((std::chrono::system_clock::now() - start) < count * 80ms){
			std::this_thread::sleep_for(1ms);
			//cout << "OUT";
		}
		count += 1;
		Output::drawFrameEdit(output_frame, true);
		//Output::drawFrame(drawing.get_frame());
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
