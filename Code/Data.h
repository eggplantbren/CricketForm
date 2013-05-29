#ifndef _Data_
#define _Data_

#include <vector>

// An object is a data set (a single player's career)
class Data
{
	private:
		int N;
		std::vector<int> score;
		std::vector<bool> out;

	public:
		Data();
		void load(const char* filename);

		// Getters
		int  get_score(int i) const { return score[i]; }
		bool get_out(int i) const { return out[i]; }
		int get_N() const { return N; }

	// Static instance
	private:
		static Data instance;
	public:
		static const Data& get_instance() { return instance; }
};

#endif

