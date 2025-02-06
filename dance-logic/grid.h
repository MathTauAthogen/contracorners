#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>
#include <vector>



template < typename T >
class Grid {

	private:

		std::vector < std::vector < T > > _grid;
		
		int _grid_height;
		int _grid_width;
		
		static constexpr int default_grid_width = 10;
		static constexpr int default_grid_height = 10;
		
		static constexpr T default_null_T = T ( true );
		T _null_T;


	public:

		Grid();

		Grid (
			int width,
			int height,
			T null_T = default_null_T
			);

		int width();

		int height();

		void reset_grid (
			bool same_dims = true, // This makes it hard to accidentally use default values, because they only happen if you already filled in an argument (so you won't accidentally run with no arguments and be surprised)
			int width = default_grid_width,
			int height = default_grid_height
			);
		
		bool initialize_with (
			T val,
			bool change = false,
			int width = default_grid_width,
			int height = default_grid_height
			);

		void print_diag ( std::string id = "GENERAL_DIAGNOSTIC" );

		T & operator[] ( // I made this operator to make sure the grid is kept square. I don't want to ever allow a push_back or erase, I'd only like them to be able to get or set.
			size_t index1,
			size_t index2
			);

};

#endif