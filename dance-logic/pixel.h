#ifndef PIX_H
#define PIX_H

#include <string>
#include <unordered_map>


extern std::unordered_map<std::string, std::string> color_codes; // defined in pixel.cpp


struct Pixel {

	char value;
	std::string bg_color;
	std::string fg_color;

	static const char default_value = ' ';
	static constexpr std::string default_bg_color = "clear_bg";
	static constexpr std::string default_fg_color = "white";

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
		std::string new_bg_color = default_bg_color,
		std::string new_fg_color = default_fg_color
		);


	bool check_valid_pixel();

	bool is_blank();

	bool is_null();



	std::string visualize();

	bool assign (
		char new_value = default_value,
		std::string new_bg_color = default_bg_color,
		std::string new_fg_color = default_fg_color
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