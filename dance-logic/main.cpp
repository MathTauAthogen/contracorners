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
#include "dancebase.cpp"
#include "shapes.cpp"

using namespace std;

int main ()
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
				image[i, j] = new Pixel((char)(crude_drawing[i][j]), "blue", "green");
			}
		}
	}

	Frame output_frame = Frame(false);
	Drawable* drawing = new Drawable(image, true);
	//Drawable* drawing = new Circle(true, 3, 50, 50); // Drawable
	//Drawable* drawing2 = new Circle(true, 2, 50, 100); // Drawable
	//Drawable* drawing3 = new Circle(true, 1, 50, 150); // Drawable
	Drawable* drawing2 = new Drawable(image, true);
	cout << output_frame.draw_on(drawing);
	Output::drawFrameEdit(output_frame, false);

	Output::initialize_or_return_singleton();
	auto start = std::chrono::system_clock::now();
	int count = 0;
	const double pi = std::numbers::pi;
	while(true){
		output_frame.clear();
		float scaled_count = count/10;
		//drawing.move(50 + 50 * cos(count / 1000.0 * 2 * pi),150 + 50 * sin(count / 1000.0 * 2 * pi), 8, 0.01 * count);//, 4.5, 0.1 * count);
		drawing2->move(50 + 50 * cos(count / 1000.0 * 2 * pi),150 + 50 * sin(count / 1000.0 * 2 * pi), 8, 0.01 * count);//, 4.5, 0.1 * count);
		//drawing2.move(50, 150, 1, 0.01 * count);//, 4.5, 0.1 * count);
		drawing->move(50 + scaled_count, 50 + scaled_count * scaled_count, 3);
		//output_frame.draw_on(drawing3);
		//output_frame.draw_on(drawing2);
		//output_frame.draw_on(drawing);
		output_frame.draw_on({drawing, drawing2});
		//output_frame.draw_on({drawing, drawing2, drawing3});
		output_frame.change_pixel(new Pixel((char)'%'), 50, 150);
		while((std::chrono::system_clock::now() - start) < count * 10ms){
			std::this_thread::sleep_for(1ms);
		}
		count += 1;
		Output::drawFrameEdit(output_frame, true);
		//Output::drawFrame(drawing.get_frame());
	}
};
