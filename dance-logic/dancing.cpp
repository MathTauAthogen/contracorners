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

// EXPERIMENTAL VALUES FOR MY DISPLAY
int experimental_width = 40;
int experimental_height = 50;
unordered_map<char, std::string> color_codes = {
	{'w', ""},{'b', ""},{'u', ""},{'g', ""}
}

class Output{ /* Here, we use the Singleton design pattern. */
	private:
		// EXPERIMENTAL VALUES FOR MY DISPLAY
		int console_width = experimental_width;
		int console_height = experimental_height;
		Output(int width, int height){
			this->console_width = width;
			this->console_height = height;
		}
		static Output theSingletonObject;
	public:
		static Output initialize_or_return_singleton(int width = this->console_width, int height = this->console_height){
			if(!theSingletonObject){
				theSingletonObject = new Output(width, height);
			}
			return theSingletonObject;

		}
		static bool drawFrame(Frame* frame){
			if(frame == nullptr){
				return false;
			}
			else{
				for(auto i : frame->get_rows()){
					cout << i << endl;
				}
				return true;
			}
		}
		static bool drawFrame(Frame frame){
			for(auto i : frame.get_rows()){
				cout << i << endl;
			}
			return true;
		}
}

class Frame{
	private:
		int width = experimental_width;
		int height = experimental_height;
		unordered_map<std::string, char> rows[][]; // This is an array of, for each pixel, a hashmap with keys "value", "bg_color", "fg_color", and etc, among other properties, which are used in the Outputting.

		bool check_valid_pixel(unordered_map<std::string, char> pixel){
			if(!pixel.contains("value") || !pixel.contains("bg_color") || !pixel.contains("fg_color")){
				return false;
			}
			if(!color_codes.contains(pixel["bg_color"]) || !color_codes.contains(pixel["fg_color"])){
				return false;
			}
			return true;
		}

		unordered_map<std::string, char> make_default_pixel(){
			unordered_map<std::string, char> temp = new unordered_map<std::string, char>();
			temp.insert(make_pair("value", ' '));
			temp.insert(make_pair("bg_color", 'w'));
			temp.insert(make_pair("fg_color", 'b'));
			return temp;
		}

	public:
		Frame(int width = this->width, int height = this->height){
			rows = new unordered_map<std::string, char>[width][height];
			for(int i = 0; i < width; i++){
				for(int j = 0; j < height; j++){
					rows[i][j] = make_default_pixel();
				}
			}
		}

		int get_width(){
			return this->width;
		}

		int get_height(){
			return this->height;
		}

		unordered_map<std::string, char>[][] get_rows(){
			return rows;
		}

		bool change_pixel(unordered_map<std::string, char> pixel, int ind_x, int ind_y){
			if(ind_x < 0 || ind_y < 0 || ind_x >= this->width || ind_y >= this->height || !check_valid_pixel(pixel)){
				return false;
			}
			rows[ind_x][ind_y] = pixel;
			return true;
		}

		pair<bool, Frame> mergeFrames(Frame frame1, Frame frame2){// We do this inplace on frame1, as we are passing by value.
			if(frame1.get_width() != frame2.get_width() || frame1.get_height() != frame2.get_height()){
				return <false, Frame()>;
			}
			else{
				unordered_map<std::string, char>[][] frame2_rows = frame2.get_rows();
				for(int i = 0; i < frame1.get_width(); i ++){
					for(int j = 0; j < frame1.get_height(); j++){
						if(frame2_rows[i][j]["value"] != ' '){
							frame1.change_pixel(frame2_rows[i][j], i, j);
						}
					}
				}
				return frame1;
			}	
		}
		bool merge_and_assign(Frame frame){
			if(frame.get_width() != width || frame.get_height() != height){
				return false;
			}
			else{
				unordered_map<std::string, char>[][] frame_rows = frame.get_rows();
				for(int i = 0; i < width; i ++){
					for(int j = 0; j < height; j++){
						if(frame_rows[i][j]["value"] != ' '){
							change_pixel(frame_rows[i][j], i, j);
						}
					}
				}
				return true;
			}
		}
}

class MovingFrame{
	private:
		queue<MovingDrawable*> MovingDrawableQueue;
		
	public:
		MovingFrame(vector<MovingDrawable*> drawables){

		}
		MovingFrame(queue<MovingDrawable*> drawables){

		}
		MovingFrame(){
			// In this case, we use an empty queue. We wait for the start signal.
		}
		void start(){

		}
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
		x = x-coordinate to place the image, which is the top left corner.
		y = y-coordinate to place the image, which is the top left corner.
		scale = amount to compress the image. If there is a majority symbol in the ASCII art, it is the value of the pixel - otherwise it is a fixed arbitrary symbol among those present.
		rotation = a percent to rotate by. The method snaps this to a multiple of 0.25 (so a 90 degree turn) for ASCII reasons.
		flip = whether the image is flipped.
		*/
		bool draw(Frame& frame, int x, int y, int scale, float rotation, bool flip){

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
		//std::map<std::string, int> positions = {
		//};

		bool is_running = false;
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
			names.push_back(who);
			stop_if_running();
		};

		bool stop_if_running(){

		}

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