#include "grid.h"

template < typename T >
Grid<T>::Grid() :
	_grid_height (0),
	_grid_width (0)
{
	print_diag ( "GRID_INIT_1" );
}

template < typename T >
Grid<T>::Grid (
	int width,
	int height,
	T null_T
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
template < typename T >
int Grid<T>::width() 
{
	return _grid_width;
}


template < typename T >
int Grid<T>::height()
{
	return _grid_height;
}

// END GETTERS





template < typename T >
void Grid<T>::reset_grid (
	bool same_dims,
	int width,
	int height
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


template < typename T >
bool Grid<T>::initialize_with (
	T val,
	bool change,
	int width,
	int height
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

template < typename T >
void Grid<T>::print_diag ( string id )
{
	cout << "DIAGNOSTIC \"" << id << "\" FOR GRID <" << typeid(T).name() << ">: _grid_width = " << _grid_width << " and _grid_height = " << _grid_height << ". " << endl;
	cout << " _grid.size() = " << _grid.size() << " and _grid[0].size() = " << _grid[0].size() << ". " << endl;
	cout << "END DIAGNOSTIC \"" << id << "\"." << endl;
}




// OPERATOR OVERRIDES
template < typename T >
T & Grid<T>::operator[] ( // I made this operator to make sure the grid is kept square. I don't want to ever allow a push_back or erase, I'd only like them to be able to get or set.
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

#include "pixel.h"

template class Grid<Pixel>;