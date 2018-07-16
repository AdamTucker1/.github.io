#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <iomanip>
using namespace std;

class counter
{
private:
	double solutions[100];

public:
	counter() //default contstuctor
	{
	}

	double Count(int n, int s, int l) // called to iteratively get solution
	{
				solutions[0] = 1;
				solutions[1] = s; //if there is only one spot left, the number of combinations is the number of small candies
				solutions[2] = s*s + l; //if there only 2 spots left, it can be any combinations of 2 shorts, which is s^2 or any of the longs

				for (int i = 3; i <= n; i++)
			  {
						solutions[i] = (s * solutions[i - 1] + l * solutions [i - 2]); //lookup the previously calculated values
			  }

				cout << fixed << setprecision(0) << solutions[n] << endl;
				return solutions[n];
	}

	
};

int main()
{
	int numOP = 0;
	int n;
	int s;
	int l;
	string line;
	counter tests[100]; //There can be no more than 100 inputs in the input file

	ifstream fp("input.txt");
	ofstream out("output.txt");
	if (!fp.is_open()) { //makes sure we can open the input file
		fprintf(stderr, "Can't open file: %s\n", "input.txt");
		exit(1);
	}

	if (!out.is_open()) { // makes sure no issue opening the output file
		fprintf(stderr, "Can't open file: %s\n", "output.txt");
		exit(1);
	}

	fp >> numOP; //this gets the number of operations

	for (int OP = 0; OP < numOP; OP++)  //goes through operations 1 by 1
	{
		tests[OP] = *new counter(); //Adds a counter class to array of counters. Can probably remove as the array isnt really used
		getline(fp, line); //gets next line
		fp >> n >> s >> l; //gets the size of the choc box, number of smalls and number of longs from line
		out << fixed << setprecision(0) <<tests[OP].Count(n, s, l) << endl; //outputs the result of the operation. Fixed prevents sci notation and set precision prevents decimals
  }

	return 1;
}
