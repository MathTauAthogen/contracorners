#include <string>
#include <vector>
#include<queue>

using namespace std;

// For now, I'll only be modelling dances in a hands-four, which doesn't leave that hands-four. That is, I'm discounting triple minors, magpie dances, squares, etc.
// Beckett will be added as an option now but inoperative, and then I'll implement Beckett.



class MovingDrawable{
	private:
		
	public:
		MovingDrawable(){

		}
};

class MovingFrame{
	private:
		queue<MovingDrawable*> MovingDrawableQueue;
		
	public:
		MovingFrame(vector<MovingDrawable*> drawables){

		}
		MovingFrame(queue<MovingDrawable*> drawables){

		}
		MovingFrame(){
			// In this case, we use an empty queue. We wait for the start signal.
		}
		void start(){

		}
};

/* This should be a visitor to the Dance class, which  */
//class

class Dancer{ //Not to be confused with Dancers, this models each individual dancer, whereas Dancers models the whole system of Dancers
	private:
		float momentum = 0;
		std::string name;
		//direction = ;
	public:
		Dancer(){
			this->name = "Joey";
		}
		Dancer(std::string name){
			this->name = name;
		}
		std::string get_name(){
			return this->name;
		}
};

class Couple{ // More complicated couples to come, for now I just have a single constructor for a two person couple.
	private:
		bool hasTwoPeople = true;
		Dancer lark;
		Dancer robin;
		vector<Dancer> dancers;
	public:
		Couple(Dancer dancer1, Dancer dancer2){
			lark = dancer1;
			robin = dancer2;
			dancers = {dancer1, dancer2};
		};
		bool swap_positions(){
			if(hasTwoPeople){
				lark, robin = robin, lark;
				return true;
			}
			return false;
		};
};

struct CoupleNames{
	vector<std::string> names;
	CoupleNames(vector<std::string> new_names){
		//for(int i = 0; i < 2; i++){
		//	names[i] = new_names[i];
		//}
		names = new_names;
	};
};

// Here, each inner vector is a pair of Dancer classes, but I have it as a vector as I can't have a vector of arrays.
class Dancers{
	private:
		//std::map<std::string, int> positions = {
		//};

		bool is_running = false;
		std::vector<Couple> dancer_objs;
		std::vector<CoupleNames> names;
		int number_of_couples = 0;
		static const int maximum_couples = -1; // We don't expect for a given set the maximum number of couples to change.
	public:
		Dancers(){
		        //this->names = {"Holly", "Jolly", "Molly", "Polly", "Dolly", "Lolly"};
		};
		/*
		Method Name: add_to_set
		*/
		bool add_to_set(vector<std::string> who){
			if(who.size() != 2){
				return false;
			}
			if(this->maximum_couples != -1){
				if(this->number_of_couples >= this->maximum_couples){
					return false;
				}
			}
			Couple newWho = Couple(Dancer(who[0]), Dancer(who[1]));
			dancer_objs.push_back(newWho);
			CoupleNames newWhoNames = CoupleNames(who);
			names.push_back(newWhoNames);
			stop_if_running();
			/////////////////////////////////////////////////////
			return false; // TODO: FINISH
			/////////////////////////////////////////////////////
		};

		bool stop_if_running(){
			return false; // TODO: FINISH
		}

		vector<CoupleNames> get_names(){
			return names;
		};

		Couple get_dancers(int couple_number){
			return dancer_objs[couple_number];
		};
};

class Dance{
	private:
		Dancers theDancers;
	public:
	Dance(){
		theDancers = Dancers();
	}

};

class HandsFour{
	public:




};

class PlayfordDance : public Dance{
	private:

	public:
		 
};