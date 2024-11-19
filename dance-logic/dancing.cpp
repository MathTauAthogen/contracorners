#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

// For now, I'll only be modelling dances in a hands-four, which doesn't leave that hands-four. That is, I'm discounting triple minors, magpie dances, squares, etc.
// Beckett will be added as an option now but inoperative, and then I'll implement Beckett.

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

class Output{ /* Here, we use the Singleton design pattern. */
	private:
		// EXPERIMENTAL VALUES FOR MY DISPLAY
		int console_width = 40;
		int console_height = 50;
		Output(int width, int height){
			
		}
		static Output theSingletonObject;
	public:
		static Output initialize_or_return_singleton(){
			initialize_or_return_singleton(this->console_width, this->console_height);
		}
		static Output initialize_or_return_singleton(int width, int height){
			if(!theSingletonObject){

			}
		}
		static Output drawFrame(){

		}
}

class Frame{
	private:

	public:
		Frame(){

		}
}

class MovingFrame{
	private:
		vector<int> DrawableQueue;
	public:
		
}

class MovingDrawable{
	private:

	public:
		MovingDrawable(){

		}
}

class Drawable{
	private:
		vector<vector<char>> image;
		int rows;
		int cols;
		int scale_default = 1;
		int current_pos_x;
		int current_pos_y;
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
		Description: 
		Parameters:
		x = x-coordinate to place the image.
		y = y-coordinate to place the image.
		scale = amount to compress the image. If there is a majority symbol in the ASCII art, it is the value of the pixel - otherwise it is a fixed arbitrary symbol among those present.
		rotation = a percent to rotate by. The method snaps this to a multiple of 0.25 (so a 90 degree turn) for ASCII reasons.
		flip = whether the image is flipped.
		*/
		bool draw(int x, int y, int scale, float rotation, bool flip){
			int new_rotate = 
		}

		/*
		Method Name: load_points
		Parameters:
		points = a vector of [x, y] points.

		scale_factor = amount to compress the image. If there is a majority symbol in the ASCII art, it is the value of the pixel - otherwise it is a fixed arbitrary symbol among those present.
		flip = whether the image is flipped.
		*/
		bool load_points(vector<int[2]> points, float delay, int scale_factor, bool flip){

		}

		bool compute_splines(vector<int[2]> points, vector<int[2]> momenta, vector<int[2]> accels){

		}

}

/* This should be a visitor to the Dance class, which  */
class


class PlayfordDance : public Dance{
	private:

	public:
		 
}

class Dance{
	private:
		Dancers theDancers;
	public:
	Dance(){
		theDancers = new Dancers();
	}

}

class Dancer{ //Not to be confused with Dancers, this models each individual dancer, whereas Dancers models the whole system of Dancers
	private:
		float momentum = 0;
		direction = ;
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
	private:
		std::map<std::string, int> positions = {
		};
	protected:
		std::vector<std::string[2]> names;
		int number_of_couples = 0;
		const int maximum_couples = -1; // We don't expect for a given set the maximum number of couples to change.
	public:
		Dancers(){
		        //this->names = {"Holly", "Jolly", "Molly", "Polly", "Dolly", "Lolly"};
		};
		/*
		Method Name: add_to_set
		*/
		bool add_to_set(std::string[2] who){
			if(this->maximum_couples != -1){
				if(this->number_of_couples >= this->maximum_couples){
					return false;
				}
			}
		};
		vector<std::string[2]> get_names(){

		};
		std::string[2] get_names(int couple_number){

		};
};

int main ()
{

	std::string filename = "params.txt";
	std::fstream s = filename;

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

