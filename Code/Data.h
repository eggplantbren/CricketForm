#ifndef _Data_
#define _Data_

#include <vector>

// An object is a data set (a single player's career)
class Data
{
	private:
		std::vector<int> score;
		std::vector<bool> out;

	public:
		Data();
		void load(const char* filename);
};

#endif

