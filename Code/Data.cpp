#include "Data.h"
#include <fstream>
#include <iostream>

using namespace std;

Data Data::instance;

Data::Data()
{

}

void Data::load(const char* filename)
{
	// Open the file, print error message if it fails
	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"# Error opening file "<<filename<<"."<<endl;
		return;
	}

	cout<<"# Reading data from file "<<filename<<". ";

	// Make the data vectors empty
	score.clear();
	out.clear();

	// Read in the data!
	int temp1; bool temp2;
	while(fin>>temp1)
	{
		temp2 = true;
		if(fin.peek() == '*')
		{
			temp2 = false;
			fin.ignore(1000000, '\n');
		}
		score.push_back(temp1);
		out.push_back(temp2);
	}
	fin.close();

	cout<<"Found "<<score.size()<<" scores."<<endl;
}

