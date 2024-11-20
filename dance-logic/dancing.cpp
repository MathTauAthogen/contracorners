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

using namespace std;

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
const int experimental_width = 40;
const int experimental_height = 50;
unordered_map<std::string, std::string> color_codes = {
	{"white", ""},{"black", ""},{"blue", ""},{"green", ""}
};

template <typename T>
class Grid{
	private:
		vector<vector<T>>* grid = nullptr;
		int grid_height;
		int grid_width;
		static constexpr int default_grid_width = 10; // These are arbitrary, shouldn't ever really be used.
		static constexpr int default_grid_height = 10; // These are arbitrary, shouldn't ever really be used.
		static constexpr T* default_null_T = new T();
		const T* null_T;
	public:
		Grid(){};
		Grid(int width, int height, const T* null_T = default_null_T){
			grid_height = height;
			grid_width = width;
			this->null_T = null_T;
			grid = new vector(grid_width, vector<T>(grid_height, *null_T));
		};

		int width(){
			return grid_width;
		};

		int height(){
			return grid_height;
		}

		void reset_grid(bool same_dims = true, int width = default_grid_width, int height = default_grid_height){ // This makes it hard to accidentally use default values, because they only happen if you already filled in an argument (so you won't accidentally run with no arguments and be surprised)
			if(!same_dims){
				grid_height = height;
				grid_width = width;
			}
			grid = new vector(grid_width, vector<T>(grid_height, *null_T));
		};

		bool initialize_with(std::function<T(int, int)> func, bool change = false, int width = default_grid_width, int height = default_grid_height){
			if(change && (grid_height != height || grid_width != width)){
				grid_height = height;
				grid_width = width;
				grid = new vector(width, vector<T>(height, null_T));
			}
			for(int i = 0; i < width; i++){
				for(int j = 0; j < height; j++){
					(*grid)[i][j] = func(i, j);
				}
			}
			return true;
		}

		auto begin(){
			return grid->begin();
		}

		auto end(){
			return grid->end();
		}

		T& operator[](size_t index1, size_t index2){
			if(0 <= index1 && index1 < grid_width && 0 <= index2 && index2 < grid_height){ // I made this operator to make sure the grid is kept square. I don't want to ever allow a push_back or erase, I'd only like them to be able to get or set 
				return (*grid)[index1][index2];
			}
			else{
				return &(*(const_cast<T*>(null_T)));
			}
		};

		// I don't really want a getter method that much because I don't want the size of the vector to be change-able. 

		//vector<vector<T>>* get(){
		//	return grid;
		//};
};

struct Pixel{
	char value = ' ';
	std::string bg_color = "white";
	std::string fg_color = "black";
	static const char default_value = ' ';
	static constexpr std::string default_bg_color = "white";
	static constexpr std::string default_fg_color = "black";
	bool blank = true;
	bool null = false;
	constexpr Pixel(int bool_blank_null){
		if (bool_blank_null == 0){
			blank = true;
			null = false;
		}
		else{
			blank = true;
			null = true;
		}
	};

	Pixel(char new_value = default_value, std::string new_bg_color = default_bg_color, std::string new_fg_color = default_fg_color){
		value = new_value;
		bg_color = new_bg_color;
		fg_color = new_fg_color;
	};

	bool check_valid_pixel(){
		if(!color_codes.contains(bg_color) || !color_codes.contains(fg_color)){
			return false;
		}
		return true;
	}
	std::string visualize(){
		return to_string(value);
	}
	bool is_blank(){
		return blank || null;
	}
	bool is_null(){
		return null;
	}
	bool define_blank(){
		return (!blank) && (blank = true);
	}
	bool erase(){
		value = default_value;
		bg_color = default_bg_color;
		fg_color = default_fg_color;
		return (!blank) && (blank = true);
	}
};

class Drawable;

class Frame{
	private:
		static const int default_frame_width = experimental_width;
		static const int default_frame_height = experimental_height;
		int frame_width;
		int frame_height;
		static constexpr Pixel* nullpix = new Pixel(1);
		Grid<Pixel> rows; // This is an array of, for each pixel, a struct with keys "value", "bg_color", "fg_color", and etc, among other properties, which are used in the Outputting.

		/*unordered_map<std::string, char> make_default_pixel(int i = -1, int j = -1){// The superfluous ints are to work with the grid initialize_with method.
			unordered_map<std::string, char> temp = unordered_map<std::string, char>();
			temp.insert(make_pair("value", ' '));
			temp.insert(make_pair("bg_color", 'w'));
			temp.insert(make_pair("fg_color", 'b'));
			return temp;
		}*/

		static Pixel make_default_pixel(int i = -1, int j = -1){// The superfluous ints are to work with the grid initialize_with method.
			//return nullpix;
			return Pixel(0);
		}

	public:
		Frame(bool use_blank = true, int width = default_frame_width, int height = default_frame_height){
			frame_width = width;
			frame_height = height;
			rows = Grid<Pixel>(width, height, nullpix);
			if(use_blank){
				rows.initialize_with(make_default_pixel);
			}
		}

		int get_width(){
			return this->frame_width;
		}

		int get_height(){
			return this->frame_height;
		}

		Grid<Pixel> get_rows(){
			return rows;
		}

		Pixel get_pixel(int i, int j){
			return rows[i, j];
		}

		bool change_pixel(Pixel pixel, int ind_x, int ind_y){ /* This might do better as private but I'll make it so when I know it is easy to draw in other ways. This is mostly an internal method. */
			if(!pixel.check_valid_pixel()){
				return false;
			}
			if(!rows[ind_x, ind_y].is_blank()){
				rows[ind_x, ind_y] = pixel;
			}
			return true;
		}

		pair<bool, Frame> mergeFrames(Frame frame1, Frame frame2){// We do this inplace on frame1, as we are passing by value.
			if(frame1.get_width() != frame2.get_width() || frame1.get_height() != frame2.get_height()){
				return make_pair(false, Frame());
			}
			else{
				Grid<Pixel> frame2_rows = frame2.get_rows();
				for(int i = 0; i < frame1.get_width(); i ++){
					for(int j = 0; j < frame1.get_height(); j++){
						if(!frame2_rows[i, j].is_blank()){
							frame1.change_pixel(frame2_rows[i, j], i, j);
						}
					}
				}
				return make_pair(true, frame1);
			}	
		}

		bool merge_and_assign(Frame frame){
			if(frame.get_width() != frame_width || frame.get_height() != frame_height){
				return false;
			}
			else{
				Grid<Pixel> frame_rows = frame.get_rows();
				for(int i = 0; i < frame_width; i ++){
					for(int j = 0; j < frame_height; j++){
						if(!frame_rows[i, j].is_blank()){
							change_pixel(frame_rows[i, j], i, j);
						}
					}
				}
				return true;
			}
		}

		bool draw_on(Drawable& drawable){
			for(int i = 0; i < frame_width; i++){
				for(int j = 0; j < frame_height; j++){
					Pixel pix = drawable.get_pixel(i, j);
					if(pix.check_valid_pixel() && !pix.is_blank()){
						change_pixel(pix, i, j);
					}
				}
			}
			//if(drawable.get_mode() == "FRAME"){

			//}
		}
};

class Drawable{
	private:
		static constexpr double pi = std::numbers::pi;
		Grid<Pixel> image;
		int rows;
		int cols;
		float scale_factor; // If negative, will be flipped.
		float current_pos_x;
		float current_pos_y;
		float rotation;
		Frame internal_frame = Frame(false);
		static constexpr Pixel* nullpix = new Pixel(1);
		std::string mode = "FRAME"; // Can be "FRAME", "EQUATION", or "DOT_NEAREST". This is how the Drawable is drawn. For a FRAME mode, the internal_frame is just blitted onto 
	public:
		Drawable(Grid<Pixel> image, float rotate = 0.0, float scale_factor = 1.0, float current_pos_x = 0, float current_pos_y = 0){
			rows = image.width();
			cols = image.height();
			this->image = image;
			this->scale_factor = scale_factor;
			this->current_pos_x = current_pos_x;
			this->current_pos_y = current_pos_y;
			this->rotation = rotate;
		}

		std::string get_mode(){
			return mode;
		}

		Pixel get_pixel(int i, int j){
			if(mode == "FRAME"){
				int tempx;
				int tempy;
				if(scale_factor > 0){
					tempx = round(i / scale_factor);
					tempy = round(j / scale_factor);
				}
				else if(scale_factor < 0){
					tempx = round((i - rows) / scale_factor);
					tempy = round((j - cols) / scale_factor);
				}
				else{
					return *nullpix;
				}

				// Now, rotate. We rotate about the centre of the image, (rows/2, cols/2). WE have to rotate backwards to make it so that when we access the rotated image of (i, j), we really access (i. j) as we would like.

				//Here, we define rotation = 1 as a full 360 degree rotation.

				tempx = -sin(-2 * pi * rotation) * tempy + tempx * cos(-2 * pi * rotation);
				tempy = sin(-2 * pi * rotation) * tempx + tempy * cos(-2 * pi * rotation);
				
				// Now, shift into place.
				tempx = tempx - current_pos_x;
				tempy = tempy - current_pos_y;

				return image[tempx, tempy];
			}
			else if (mode == "DOT_NEAREST"){
				return internal_frame.get_pixel(i, j); // Placeholder
			}
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

		// In reality, this method just moves/changes the image that will be drawn via a Frame().draw_on().
		bool draw(Frame& frame, int x, int y, int scale, float rotation){

			
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
};

class Output{ /* Here, we use the Singleton design pattern. */
	private:
		// EXPERIMENTAL VALUES FOR MY DISPLAY
		static const int default_console_width = experimental_width;
		static const int default_console_height = experimental_height;
		int console_width;
		int console_height;
		Output(int width, int height){
			this->console_width = width;
			this->console_height = height;
		}
		static Output* theSingletonObject;
	public:
		static Output* initialize_or_return_singleton(int width = default_console_width, int height = default_console_height){
			if(theSingletonObject == nullptr){
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
					for(auto j : i){
						cout << j.visualize();
					}
					cout << endl;
				}
				return true;
			}
		}
		static bool drawFrame(Frame frame){
			for(auto i : frame.get_rows()){
					for(auto j : i){
						cout << j.visualize();
					}
					cout << endl;
				}
			return true;
		}
};

class MovingDrawable{
	private:

	public:
		MovingDrawable(){

		}
};

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
};

/* This should be a visitor to the Dance class, which  */
//class

class Dancer{ //Not to be confused with Dancers, this models each individual dancer, whereas Dancers models the whole system of Dancers
	private:
		float momentum = 0;
		std::string name;
		//direction = ;
	public:
		Dancer(){
			this->name = "Joey";
		}
		Dancer(std::string name){
			this->name = name;
		}
		std::string get_name(){
			return this->name;
		}
};

class Couple{ // More complicated couples to come, for now I just have a single constructor for a two person couple.
	private:
		bool hasTwoPeople = true;
		Dancer lark;
		Dancer robin;
		vector<Dancer> dancers;
	public:
		Couple(Dancer dancer1, Dancer dancer2){
			lark = dancer1;
			robin = dancer2;
			dancers = {dancer1, dancer2};
		};
		bool swap_positions(){
			if(hasTwoPeople){
				lark, robin = robin, lark;
				return true;
			}
			return false;
		};
};

struct CoupleNames{
	vector<std::string> names;
	CoupleNames(vector<std::string> new_names){
		//for(int i = 0; i < 2; i++){
		//	names[i] = new_names[i];
		//}
		names = new_names;
	};
};

// Here, each inner vector is a pair of Dancer classes, but I have it as a vector as I can't have a vector of arrays.
class Dancers{
	private:
		//std::map<std::string, int> positions = {
		//};

		bool is_running = false;
		std::vector<Couple> dancer_objs;
		std::vector<CoupleNames> names;
		int number_of_couples = 0;
		static const int maximum_couples = -1; // We don't expect for a given set the maximum number of couples to change.
	public:
		Dancers(){
		        //this->names = {"Holly", "Jolly", "Molly", "Polly", "Dolly", "Lolly"};
		};
		/*
		Method Name: add_to_set
		*/
		bool add_to_set(vector<std::string> who){
			if(who.size() != 2){
				return false;
			}
			if(this->maximum_couples != -1){
				if(this->number_of_couples >= this->maximum_couples){
					return false;
				}
			}
			Couple newWho = Couple(Dancer(who[0]), Dancer(who[1]));
			dancer_objs.push_back(newWho);
			CoupleNames newWhoNames = CoupleNames(who);
			names.push_back(newWhoNames);
			stop_if_running();
		};

		bool stop_if_running(){

		}

		vector<CoupleNames> get_names(){
			return names;
		};

		Couple get_dancers(int couple_number){
			return dancer_objs[couple_number];
		};
};

class Dance{
	private:
		Dancers theDancers;
	public:
	Dance(){
		theDancers = Dancers();
	}

};

class HandsFour{
	public:




};

class PlayfordDance : public Dance{
	private:

	public:
		 
};

int main ()
{

	std::string filename = "params.txt";
	std::fstream s = fstream(filename);

	if (!s.is_open()){
		std::cout << "Failed to open!\n";
		return 0;
	};

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
		for(auto j : dancers.get_names()[i].names){
			std::cout << j << " ";
		}
		std::cout  << "\n";
	}
};
