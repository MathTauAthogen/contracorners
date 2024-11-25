#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <map>
#include <vector>
#include<unordered_map>
#include<queue>
#include<cmath>
#include<numbers>
#include<functional>
#include "consoleoutput.cpp"

class Shape : public Drawable{
	//private:
	protected:
		std::function<Pixel*(float, float)> getPixel;
	public:
		Shape(std::function<Pixel*(float, float)> get_the_pixel, bool from_center = false, float rotate = 0.0, float scale_factor = 1.0, float current_pos_x = 0, float current_pos_y = 0) : Drawable(Grid<Pixel>(), from_center, rotate, scale_factor, current_pos_x, current_pos_y)
		{
			this->getPixel = get_the_pixel;
		}

		virtual Pixel* get_pixel(int i, int j) override{
			auto [x, y] = transform(i, j);

			if(x == -1 && y == -1){
				return nullpix;
			}
			
			return getPixel(x, y);
		}
};

class Circle : public Shape{
	private:
		vector<Pixel*> possibilities;
	public:
		Circle(bool from_center = false, float scale_factor = 1.0, float current_pos_x = 0, float current_pos_y = 0) :
			Shape(
				[&](float x, float y){
				//cout << "x = " << x << " and y = " << y << endl;
				float approx_const = 0.4;
				if(((1 - approx_const / _scale_factor) <= sqrt(pow(x, 2) + pow(y, 2))) && (sqrt(pow(x, 2) + pow(y, 2)) <= (1 + approx_const / _scale_factor))){
					//cout << "x = " << x << " and y = " << y << endl;
					float side_const = 0.15;
					if(x > 0 && abs(y / x)  < side_const){
						return possibilities[0];
					}
					else if(x < 0 && abs(y / x)  < side_const){
						return possibilities[1];
					}
					else if(y > 0 && abs(x / y)  < side_const){
						return possibilities[2];
					}
					else if(y < 0 && abs(x / y)  < side_const){
						return possibilities[2];
					}
					else if(x < 0 && y/x > 0){
						return possibilities[3];
					}
					else if(x < 0 && y/x < 0){
						return possibilities[4];
					}
					else if(x > 0 && x/y > 0){
						return possibilities[3];
					}
					else if(x > 0 && x/y < 0){
						return possibilities[4];
					}
					else if(x == 0 && y == 0){
						return nullpix;
					}
					else{
						cout << "WHAT?" << x << " and " << y << endl;
					}
					return nullpix;
				}
				else{
					return nullpix;
				}
			}, // This is the first argument

			from_center, 0, scale_factor, current_pos_x, current_pos_y
			){
				possibilities = {new Pixel('_', "blue", "green_neon"), new Pixel('-', "blue", "green_neon"), new Pixel('|', "blue", "green_neon"), new Pixel('/', "blue", "green_neon"), new Pixel('\\', "blue", "green_neon")};
			}
};

#endif