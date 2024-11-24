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
		std::function<Pixel(float, float)> getPixel;
	public:
		Shape(std::function<Pixel(float, float)> get_the_pixel, bool from_center = false, float rotate = 0.0, float scale_factor = 1.0, float current_pos_x = 0, float current_pos_y = 0) : Drawable(Grid<Pixel>(), from_center, rotate, scale_factor, current_pos_x, current_pos_y)
		{
			this->getPixel = get_the_pixel;
		}

		virtual Pixel get_pixel(int i, int j) override{
			auto [tempx, tempy] = transform(i, j);

			if(tempx == -1 && tempy == -1){
				return *nullpix;
			}
			
			return getPixel(tempx, tempy);
		}
};

class Circle : public Shape{
	private:

	public:
		Circle(bool from_center = false, float scale_factor = 1.0, float current_pos_x = 0, float current_pos_y = 0) :
			Shape(

				[&](float tempx, float tempy){
				//cout << "tempx = " << tempx << " and tempy = " << tempy << endl;
				float approx_const = 1;
				if(((1 - approx_const / this->scale_factor) <= sqrt(pow(tempx, 2) + pow(tempy, 2))) && (sqrt(pow(tempx, 2) + pow(tempy, 2)) <= (1 + approx_const / this->scale_factor))){
					float side_const = 0.15;
					if(tempx > 0 && abs(tempy / tempx)  < side_const){
						return Pixel('|', "blue", "green_neon");
					}
					else if(tempx < 0 && abs(tempy / tempx)  < side_const){
						return Pixel('|', "blue", "green_neon");
					}
					else if(tempy > 0 && abs(tempx / tempy)  < side_const){
						return Pixel('-', "blue", "green_neon");
					}
					else if(tempy < 0 && abs(tempx / tempy)  < side_const){
						return Pixel('_', "blue", "green_neon");
					}
					else if(tempx < 0 && tempy/tempx > 0){
						return Pixel('\\', "blue", "green_neon");
					}
					else if(tempx < 0 && tempy/tempx < 0){
						return Pixel('/', "blue", "green_neon");
					}
					else if(tempx > 0 && tempx/tempy > 0){
						return Pixel('\\', "blue", "green_neon");
					}
					else if(tempx > 0 && tempx/tempy < 0){
						return Pixel('/', "blue", "green_neon");
					}
					else if(tempx == 0 && tempy == 0){
						return *nullpix;
					}
					else{
						cout << "WHAT?" << tempx << " and " << tempy << endl;
					}
					return *nullpix;
				}
				else{
					return *nullpix;
				}
			}, // This is the first argument

			from_center, 1, scale_factor, current_pos_x, current_pos_y
			){}
};

#endif