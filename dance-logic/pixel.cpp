#ifndef PIX_H
#define PIX_H

#include <string>
#include <unordered_map>

using namespace std;


unordered_map<string, string> color_codes = {
	{"white", "15"},{"black", "0"},{"blue", "21"},{"navy", "12"},{"green", "2"},{"green_neon", "10"}, {"clear_fg", "2m\033[39"}, {"clear_bg", "2m\033[49"}
}; // These are 256 bit codes.



struct Pixel {

	// VARIABLES

	char value;
	string bg_color;
	string fg_color;

	static const char default_value = ' ';
	static constexpr string default_bg_color = "clear_bg";
	static constexpr string default_fg_color = "white";

	bool blank = true;
	bool null = false;

	// END VARIABLES




	// CONSTRUCTORS

	constexpr Pixel ( bool is_null )
	:
		value ( default_value ),
		fg_color ( default_bg_color ),
		bg_color ( default_bg_color )
	{
		if ( is_null )
		{
			null = true;
		}
	}

	explicit Pixel (
		char new_value = default_value,
		string new_bg_color = default_bg_color,
		string new_fg_color = default_fg_color
		) :
		value ( new_value ),
		fg_color ( new_fg_color ),
		bg_color ( new_bg_color )
	{
		blank = blank && (value == default_value);
		blank = blank && (fg_color == default_fg_color);
		blank = blank && (bg_color == default_bg_color);
	}

	// END CONSTRUCTORS



	// VALIDATION

	bool check_valid_pixel()
	{
		if (
			!color_codes.contains(bg_color) ||
			!color_codes.contains(fg_color)
			)
		{
			return false;
		}
		return true;
	}

	bool is_blank(){
		return blank || null;
	}

	bool is_null(){
		return null;
	}

	// END VALIDATION





	// DISPLAY

	string visualize(){
		return 
			"\033[38;5;"
			+ color_codes[fg_color]
			+ ";48;5;"
			+ color_codes[bg_color]
			+ "m"
			+ string(1, value)
			+ "\033[0m";
	}


	// END DISPLAY




	// SETTERS

	bool assign (
		char new_value = default_value,
		string new_bg_color = default_bg_color,
		string new_fg_color = default_fg_color
		)
	{
		null = false;
		blank = blank && (value == new_value);
		blank = blank && (fg_color == new_fg_color);
		blank = blank && (bg_color == new_bg_color);
		value = new_value;
		bg_color = new_bg_color;
		fg_color = new_fg_color;
		return blank;
	}

	bool erase( bool is_null ){
		value = default_value;
		bg_color = default_bg_color;
		fg_color = default_fg_color;
		
		null = is_null;
		return ( !blank ) && ( blank = true );
	}

	// END SETTERS
};


bool operator== (
	const Pixel& pix1,
	const Pixel& pix2
	)
{
	return
		pix1.value == pix2.value &&
		pix1.bg_color == pix2.bg_color &&
		pix1.fg_color == pix2.fg_color &&
		pix1.blank == pix2.blank &&
		pix1.null == pix2.null;
}

bool operator!= (
	const Pixel& pix1,
	const Pixel& pix2
	)
{
	return ! (pix1 == pix2);
}


#endif