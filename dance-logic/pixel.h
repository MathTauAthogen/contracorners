#ifndef PIX_H
#define PIX_H

#include <string>
#include <unordered_map>

using namespace std;


unordered_map<string, string> color_codes = {
	{"white", "15"},{"black", "0"},{"blue", "21"},{"navy", "12"},{"green", "2"},{"green_neon", "10"}, {"clear_fg", "2m\033[39"}, {"clear_bg", "2m\033[49"}
}; // These are 256 bit codes.



struct Pixel {

	char value;
	string bg_color;
	string fg_color;

	static const char default_value = ' ';
	static constexpr string default_bg_color = "clear_bg";
	static constexpr string default_fg_color = "white";

	bool blank = true;
	bool null = false;

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
		);


	bool check_valid_pixel();

	bool is_blank();

	bool is_null();



	string visualize();

	bool assign (
		char new_value = default_value,
		string new_bg_color = default_bg_color,
		string new_fg_color = default_fg_color
		);

	bool erase( bool is_null );
};


bool operator== (
	const Pixel& pix1,
	const Pixel& pix2
	);

bool operator!= (
	const Pixel& pix1,
	const Pixel& pix2
	);


#endif