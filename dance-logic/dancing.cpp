#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

// For now, I'll only be modelling dances in a hands-four, which doesn't leave that hands-four. That is, I'm discounting triple minors, magpie dances, squares, etc.
// Beckett will be added as an option now but inoperative, and then I'll implement Beckett.


class Dancer{ //Not to be confused with Dancers, this models each individual dancer, whereas Dancers models the whole system of Dancers
	public:
		Dancer(std::string name){
			this->name = name;
		}
		std::string name;

};


class HandsFour{
	public:




}

class Dancers{
	public:
		Dancers(){
		        this->names = {"Holly", "Jolly", "Molly", "Polly", "Dolly", "Lolly"};
		};
		std::vector<std::string> names;
	private:
		std::map<std::string, int> positions = {
		};
};

int main ()
{

	std::string filename{"params.txt"};
	std::fstream s{filename};

	if (!s.is_open()){
		std::cout << "Failed to open!\n";
		return 0;
	};

	std::map<std::string, std::string> dance_moves = {
		{"",""},
		{"",""},
		{"",""}
	};
	Dancers dancers = Dancers();
	for (int i = 0; i < dancers.names.size(); i ++){
		std::cout << dancers.names[i] << "\n";
	}
};

