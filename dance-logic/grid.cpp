#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>
#include <vector>

// Only used in graveyard
#include<functional>

using namespace std;



template < typename T >
class Grid {

	private:

		vector < vector < T > > _grid;
		
		int _grid_height;
		int _grid_width;
		
		static constexpr int default_grid_width = 10;
		static constexpr int default_grid_height = 10;
		
		static constexpr T default_null_T = T ( true );
		T _null_T;


	public:

		// CONSTRUCTORS

		Grid() :
			_grid_height (0),
			_grid_width (0)
		{
			print_diag ( "GRID_INIT_1" );
		}

		Grid (
			int width,
			int height,
			T null_T = default_null_T
			) :
			_grid_height ( height ),
			_grid_width ( width ),
			_null_T ( null_T )
		{
			_grid = vector (
					_grid_height,
					vector < T > (
						_grid_width,
						_null_T
					)
				);
			print_diag ( "GRID_INIT_2" );
		}

		// END CONSTRUCTORS





		// GETTERS

		int width()
		{
			return _grid_width;
		}


		int height()
		{
			return _grid_height;
		}

		// END GETTERS





		void reset_grid (
			bool same_dims = true, // This makes it hard to accidentally use default values, because they only happen if you already filled in an argument (so you won't accidentally run with no arguments and be surprised)
			int width = default_grid_width,
			int height = default_grid_height
			)
		{ 

			print_diag ( "GRID_INIT_5" );

			if ( !same_dims )
			{
				_grid_height = height;
				_grid_width = width;
			}

			_grid = vector ( _grid_height, vector < T > ( _grid_width, _null_T ) );
		}

		bool initialize_with (
			T val,
			bool change = false,
			int width = default_grid_width,
			int height = default_grid_height
			)
		{
			print_diag ( "GRID_INIT_3"  );
			if (
				change &&
					( 
						_grid_height != height || 
						_grid_width != width
					)
				)
			{
				_grid_height = height;
				_grid_width = width;
				_grid = vector ( height, vector < T > ( width, _null_T ) );
			}

			for ( int i = 0; i < _grid_height; i ++ )
			{
				for ( int j = 0; j < _grid_width; j ++ )
				{
					( _grid ) [i] [j] = val;
				}
			}

			return true;
		}




		// DISPLAY DIAGNOSTICS

		void print_diag ( string id = "GENERAL_DIAGNOSTIC" )
		{
			cout << "DIAGNOSTIC \"" << id << "\" FOR GRID <" << typeid(T).name() << ">: _grid_width = " << _grid_width << " and _grid_height = " << _grid_height << ". " << endl;
			cout << " _grid.size() = " << _grid.size() << " and _grid[0].size() = " << _grid[0].size() << ". " << endl;
			cout << "END DIAGNOSTIC \"" << id << "\"." << endl;
		}


		// OPERATOR OVERRIDES

		T & operator[] ( // I made this operator to make sure the grid is kept square. I don't want to ever allow a push_back or erase, I'd only like them to be able to get or set.
			size_t index1,
			size_t index2
			)
		{ 
			if (
				0 <= index1
				&& index1 < _grid_height
				&& 0 <= index2
				&& index2 < _grid_width
				)
			{ 
				if( index1 == 1 || index1 == 2){
					cout << index1 << " and " << index2 << " and " << _grid_height << " and " << _grid_width << " and " << _grid.size() << endl;// " and " << _grid[0].size() << endl;
				}
				return ( _grid ) [index1] [index2];
			}

			else {
				return _null_T;
			}

		}

};

#endif



/* Graveyard of functions that might need to be used later but currently I have no plans to */



// initialize_with but with a function.

/*
bool initialize_with (
	function < T ( int, int ) > func,
	bool change = false,
	int width = default_grid_width,
	int height = default_grid_height
	)
{

	print_diag ( "GRID_INIT_4" );

	if (
		change &&
			( 
				_grid_height != height || 
				_grid_width != width
			)
		)
	{
		_grid_height = height;
		_grid_width = width;
		_grid = vector ( height, vector < T > ( width, _null_T ) );
	}

	for ( int i = 0; i < _grid_height; i ++ )
	{
		for ( int j = 0; j < _grid_width; j ++ )
		{
			( _grid ) [i] [j] = func ( i, j );
		}
	}

	return true;
}
*/