#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

// For now, I'll only be modelling dances in a hands-four, which doesn't leave that hands-four. That is, I'm discounting triple minors, magpie dances, squares, etc.
// Beckett will be added as an option now but inoperative, and then I'll implement Beckett.

class Output{
	private:
		// EXPERIMENTAL VALUES FOR MY DISPLAY
		int console_width = 40;
		int console_height = 50;

		char[][]

	public:
		Output(){

		}
		Output(int width){

		}

}


class Drawable{
	private:
		vector<vector<char>> image;
		int rows;
		int cols;
		int scale_default = 1;
	public:
		Drawable(vector<vector<char>> image){

		}
		Drawable(vector<vector<char>> image, int scale_default){

		}
		Drawable(vector<vector<char>> image, int rows, int cols){

		}
		Drawable(vector<vector<char>> image, int rows, int cols, int scale_default){

		}
		/*
		Method Name: draw
		Parameters:
		x = x-coordinate to place the image.
		y = y-coordinate to place the image.
		scale = amount to compress the image. If there is a majority symbol in the ASCII art, 
		rotation = a percent to rotate by. The method snaps this to a multiple of 0.25 (so a 90 degree turn) for ASCII reasons.
		flip = whether the image is flipped.
		*/
		bool draw(int x, int y, int scale, float rotation, bool flip){
			int new_rotate = 
		}


}

class Dancer{ //Not to be confused with Dancers, this models each individual dancer, whereas Dancers models the whole system of Dancers
	public:
		Dancer(std::string name){
			this->name = name;
		}
		std::string name;

};


class HandsFour{
	public:




};

class Dancers{
	public:
		Dancers(){
		        this->names = {"Holly", "Jolly", "Molly", "Polly", "Dolly", "Lolly"};
		};
		std::vector<std::string> names;
	private:
		std::map<std::string, int> positions = {
		};
};

int main ()
{

	std::string filename{"params.txt"};
	std::fstream s{filename};

	if (!s.is_open()){
		std::cout << "Failed to open!\n";
		return 0;
	};

	std::map<std::string, std::string> dance_moves = {
		{"",""},
		{"",""},
		{"",""}
	};
	Dancers dancers = Dancers();
	for (int i = 0; i < dancers.names.size(); i ++){
		std::cout << dancers.names[i] << "\n";
	}
};

