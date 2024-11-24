#include <string>
#include <map>
#include <vector>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<numbers>
#include<functional>

using namespace std;

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
const int experimental_width = 400;
const int experimental_height = 100;
unordered_map<std::string, std::string> color_codes = {
	{"white", "15"},{"black", "0"},{"blue", "21"},{"navy", "12"},{"green", "2"},{"green_neon", "10"}, {"clear_fg", "2m\033[39"}, {"clear_bg", "2m\033[49"}
}; // These are 256 bit codes.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
class Grid{
	private:
		vector<vector<T>>* grid = nullptr;
		int grid_height;
		int grid_width;
		static constexpr int default_grid_width = 10; // These are arbitrary, shouldn't ever really be used.
		static constexpr int default_grid_height = 10; // These are arbitrary, shouldn't ever really be used.
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//static constexpr T* default_null_T = new T(-1); // There's an assumption that T will have -1 as a parameter map to a null.
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		static constexpr T default_null_T_obj = T(-1);
		static constexpr T* default_null_T = const_cast<T*>(&(default_null_T_obj)); /* TODO: WHY THE HELL DOES THIS WORK? BUT IT DOES SOMEHOW SO LEAVE IT */
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		const T* null_T;
	public:
		Grid(){};
		Grid(int width, int height, const T* null_T = default_null_T){
			grid_height = height;
			grid_width = width;
			this->null_T = null_T;
			grid = new vector(grid_height, vector<T>(grid_width, *null_T));
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
			grid = new vector(grid_height, vector<T>(grid_width, *null_T));
		};

		bool initialize_with(std::function<T(int, int)> func, bool change = false, int width = default_grid_width, int height = default_grid_height){
			if(change && (grid_height != height || grid_width != width)){
				grid_height = height;
				grid_width = width;
				grid = new vector(height, vector<T>(width, *null_T));
			}
			for(int i = 0; i < height; i++){
				for(int j = 0; j < width; j++){
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
			if(0 <= index1 && index1 < grid_height && 0 <= index2 && index2 < grid_width){ // I made this operator to make sure the grid is kept square. I don't want to ever allow a push_back or erase, I'd only like them to be able to get or set 
				return (*grid)[index1][index2];
			}
			else{
				//cout << "OUT OF RANGE: i = " << index1 << " and j = " << index2 << " AND GRID HEIGHT = " << grid_height << " AND GRID WIDTH = " << grid_width << endl;
				return *(const_cast<T*>(null_T));
			}
		};

		// I don't really want a getter method that much because I don't want the size of the vector to be change-able. 

		//vector<vector<T>>* get(){
		//	return grid;
		//};
};

struct Pixel{	
	char value = ' ';
	std::string bg_color = "clear_bg";
	std::string fg_color = "black";
	static const char default_value = ' ';
	static constexpr std::string default_bg_color = "clear_bg";
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
		blank = blank && (value == new_value);
		blank = blank && (fg_color == new_fg_color);
		blank = blank && (bg_color == new_bg_color);
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
		return "\033[38;5;" + color_codes[fg_color] + ";48;5;" + color_codes[bg_color] + "m" + std::string(1, value) + "\033[0m";
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
	bool assign(char new_value = default_value, std::string new_bg_color = default_bg_color, std::string new_fg_color = default_fg_color){
		blank = blank && (value == new_value);
		blank = blank && (fg_color == new_fg_color);
		blank = blank && (bg_color == new_bg_color);
		value = new_value;
		bg_color = new_bg_color;
		fg_color = new_fg_color;
		return blank;
	}

	bool erase(){
		value = default_value;
		bg_color = default_bg_color;
		fg_color = default_fg_color;
		return (!blank) && (blank = true);
	}
};

bool operator!=(const Pixel& pix1, const Pixel&pix2){
	return (pix1.value != pix2.value) || (pix1.bg_color != pix2.bg_color) || (pix1.fg_color != pix2.fg_color) || (pix1.blank != pix2.blank) || (pix1.null != pix2.null);
}

class Drawable;

class Frame{
	private:
		static const int default_frame_width = experimental_width;
		static const int default_frame_height = experimental_height;
		int frame_width;
		int frame_height;
		//static constexpr Pixel* nullpix = new Pixel(-1);
		static constexpr Pixel nullpix_obj = Pixel(-1);
		static constexpr Pixel* nullpix = const_cast<Pixel*>(&(nullpix_obj)); /* TODO: WHY THE HELL DOES THIS WORK? BUT IT DOES SOMEHOW SO LEAVE IT */
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

		void clear(bool use_blank = true){
			rows = Grid<Pixel>(frame_width, frame_height, nullpix);
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
			if(!pixel.check_valid_pixel() || pixel.is_blank()){
				return false;
			}
			//if(!pixel.is_blank()){
			rows[ind_x, ind_y] = pixel;
			//}
			return true;
		}

		pair<bool, Frame> mergeFrames(Frame frame1, Frame frame2){// We do this inplace on frame1, as we are passing by value.
			if(frame1.get_width() != frame2.get_width() || frame1.get_height() != frame2.get_height()){
				return make_pair(false, Frame());
			}
			else{
				Grid<Pixel> frame2_rows = frame2.get_rows();
				for(int i = 0; i < frame1.get_height(); i ++){
					for(int j = 0; j < frame1.get_width(); j++){
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
				for(int i = 0; i < frame_height; i ++){
					for(int j = 0; j < frame_width; j++){
						if(!frame_rows[i, j].is_blank()){
							change_pixel(frame_rows[i, j], i, j);
						}
					}
				}
				return true;
			}
		}
		bool draw_on(Drawable& drawable); // DEFINED AFTER DRAWABLE
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
		static constexpr Pixel nullpix_obj = Pixel(-1);
		static constexpr Pixel* nullpix = const_cast<Pixel*>(&(nullpix_obj)); /* TODO: WHY THE HELL DOES THIS WORK? BUT IT DOES SOMEHOW SO LEAVE IT */
		std::string mode = "FRAME"; // Can be "FRAME", "EQUATION", or "DOT_NEAREST". This is how the Drawable is drawn. For a FRAME mode, the internal_frame is just blitted onto 
	public:
		Drawable(Grid<Pixel> image, float rotate = 0.0, float scale_factor = 1.0, float current_pos_x = 0, float current_pos_y = 0){
			rows = image.height();
			cols = image.width();
			this->image = image;
			this->scale_factor = scale_factor;
			this->current_pos_x = current_pos_x;
			this->current_pos_y = current_pos_y;
			this->rotation = rotate;
		}

		std::string get_mode(){
			return mode;
		}

		Frame& get_frame(){
			return internal_frame;
		}

		Pixel get_pixel(int i, int j){
			if(mode == "FRAME"){
				float tempx = i;
				float tempy = j;

				if(scale_factor == 0){
					return *nullpix;
				}

				// Now, shift into place.
				tempx = tempx - current_pos_x;
				tempy = tempy - current_pos_y;

				// Rescale.

				if(scale_factor > 0){
					tempx = tempx / scale_factor;
					tempy = tempy / scale_factor;
				}
				else if(scale_factor < 0){
					tempx = (tempx - rows) / scale_factor;
					tempy = (tempy - cols) / scale_factor;
				}

				// Now, rotate. We rotate about the centre of the image, (rows/2, cols/2). WE have to rotate backwards to make it so that when we access the rotated image of (i, j), we really access (i. j) as we would like.

				//Here, we define rotation = 1 as a full 360 degree rotation.

				float offset_x = rows/2.0;
				float offset_y = cols/2.0;

				float oldtempx = tempx;
				float oldtempy = tempy;

				float olddist = sqrt(pow(tempx - offset_x, 2) + pow(tempy - offset_y, 2));

				//cout << "oldtempx = " << oldtempx << endl;
				tempx = (-sin(-2 * pi * rotation) * (oldtempy - offset_y)) + ((oldtempx - offset_x) * cos(-2 * pi * rotation));
				tempy = (sin(-2 * pi * rotation) * (oldtempx - offset_x)) + ((oldtempy - offset_y) * cos(-2 * pi * rotation));
				//cout << "tempx = " << tempx << endl;

				tempx += offset_x;
				tempy += offset_y;

				float newdist = sqrt(pow(tempx - offset_x, 2) + pow(tempy - offset_y, 2));

				/*if(0 <= tempx && tempx < image.height() && 0 <= tempy && tempy < image.width()){
					cout << "ORIGINAL = " << i << " AND " << j << endl;
					cout << "tempx = " << round(tempx) << "tempy = " << round(tempy) << endl;
					cout << "output += " << image[round(tempx), round(tempy)].visualize() << endl;
				}*/
				return image[round(tempx), round(tempy)];
			}
			else if (mode == "DOT_NEAREST"){
				return internal_frame.get_pixel(i, j); // Placeholder
			}
			else{
				return Pixel(-1); // Shouldn't happen for now, TODO: REPLACE
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
		//bool draw(Frame& frame, int x, int y, int scale, float rotation){
		bool draw(float x = 0, float y = 0, float scale = 1, float rotation = 0){
			this->current_pos_x = x;
			this->current_pos_y = y;
			this->scale_factor = scale;
			this->rotation = rotation;
			return true;
			//return false; // TODO: FINISH
		}

		/*
		Method Name: load_points
		Parameters:
		points = a vector of [x, y] points.

		scale_factor = amount to compress the image. If there is a majority symbol in the ASCII art, it is the value of the pixel - otherwise it is a fixed arbitrary symbol among those present.
		flip = whether the image is flipped.
		*/
		bool load_points(vector<int[2]> points, float delay, int scale_factor, bool flip){
			return false; // TODO: FINISH
		}

		bool compute_splines(vector<int[2]> points, vector<int[2]> momenta, vector<int[2]> accels){
			return false; // TODO: FINISH
		}
};


///////////////////////////////////// DRAW_ON DEFINITION /////////////////////////////////////////////////////////////////////////////


inline bool Frame::draw_on(Drawable& drawable){
	bool changed_anything = false;
	for(int i = 0; i < frame_height; i++){
		for(int j = 0; j < frame_width; j++){
			Pixel pix = drawable.get_pixel(i, j);
			changed_anything = change_pixel(pix, i, j) || changed_anything;
		}
	}
	//if(drawable.get_mode() == "FRAME"){

	//}
	return changed_anything;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Output{ /* Here, we use the Singleton design pattern. */
	private:
		// EXPERIMENTAL VALUES FOR MY DISPLAY
		static const int default_console_width = experimental_width;
		static const int default_console_height = experimental_height;
		int console_width;
		int console_height;
		Grid<Pixel> last_image;
		Output(int width, int height){
			this->console_width = width;
			this->console_height = height;
			this->last_image = Grid<Pixel>(width, height);
			cout << "\033[?25l";
		};
		~Output(){};
		static Output* theSingletonObject;
	public:
		Output(const Output&) = delete;
		Output* operator=(const Output&) = delete;

		static Output* initialize_or_return_singleton(int width = default_console_width, int height = default_console_height){
			if(theSingletonObject == nullptr){
				theSingletonObject = new Output(width, height);
			}
			return theSingletonObject;

		}
		static void clear(){
			/*for(int i = 0; i < theSingletonObject->console_height; i++){
				cout << endl;
			}*/
			std::cout << "\x1B[2J\x1B[H";
		}

		Grid<Pixel> get_last_image(){
			return last_image;
		}

		int get_width(){
			return console_width;
		}

		int get_height(){
			return console_height;
		}
		/*static bool drawFrame(Frame* frame){
			if(frame == nullptr){
				return false;
			}
			else{
				clear();
				cout << std::string(2 + frame.get_width(), '-') << endl;
				int rownum, colnum = 0;
				for(auto i : frame->get_rows()){
					cout << "|";
					colnum = 0;
					for(auto j : i){
						last_image[rownum, colnum] = j;
						colnum += 1;
						cout << j.visualize();
					}
					cout << "|";
					cout << endl;
					rownum += 1;
				}
				cout << std::string(2 + frame.get_width(), '-') << endl;
				return true;
			}
		}*/
		static bool drawFrame(Frame frame){
			clear();
			cout << std::string(2 + frame.get_width(), '-') << endl;
			int rownum = 0, colnum = 0;
			Grid<Pixel> last_image = initialize_or_return_singleton()->get_last_image();
			for(auto i : frame.get_rows()){
				cout << "|";
				colnum = 0;
				for(auto j : i){
					last_image[rownum, colnum] = j;
					colnum += 1;
					cout << j.visualize();
				}
				cout << "|";
				cout << endl;
				rownum += 1;
			}
			cout << std::string(2 + frame.get_width(), '-') << endl;
			return true;
		}
		static bool drawFrameEdit(Frame frame, bool prevdrawn = false){
			if(!prevdrawn){
				return drawFrame(frame);
			}
			int rownum = 0, colnum = 0;
			Grid<Pixel> last_image = initialize_or_return_singleton()->get_last_image();
			for(auto i : frame.get_rows()){
				colnum = 0;
				for(auto j : i){
					if(last_image[rownum, colnum] != j){
						cout << "\033[" << (rownum + 2) << ";" << (colnum + 2) << "H";
						cout << j.visualize();
					}
					last_image[rownum, colnum] = j;
					colnum += 1;
				}
				rownum += 1;
			}
			return true;
		}
};

Output* Output::theSingletonObject = nullptr; // I guess I have to do this outside of the class