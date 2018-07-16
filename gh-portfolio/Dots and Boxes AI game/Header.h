#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;
#define SX 4 //size of the arary, x and y
#define SY 4
#define SIZE SX*SY
#define BOXES (SX-1)*(SY-1)
struct pointDir //holds a point and direction for use in the AI
{
	int point;
	char dir;
	int cost;
};

class Dots //used to hold the problem
{

private:


public:
	int scoreP1 = 0;
	int scoreP2 = 0;
	int turn = 0;
	int dot[SIZE] = {}; //holds the values of the dots, 0 means no edge, 1 means edge to right, 2 means edge down, 3 means edges right and down, 4 means player one has a box, 5 means player two has a box
	int counter[SIZE] = {}; //for each dot being the top left of the box, holds the number of lines in the box
	int playerPos;
	bool tester = false;
	Dots()
	{
		int dot[SIZE] = {};
		int playerPos = 1;
	}
	Dots(int player)
	{
		int dot[SIZE] = {};
		int playerPos = player;
	}
	/*Dots(int arr[SIZE])
	{
		for(int x = 0; x< SIZE; x++)
			dot[x] = arr[x];
		tester = true;
	//	copy(begin(arr), end(dot))
	} */

	Dots(const Dots & d2) //copy constructor, used to test values
	{
		scoreP1 = d2.scoreP1;
		scoreP2 = d2.scoreP2;
		turn = d2.turn;
		playerPos = d2.playerPos;
		for (int x = 0; x < SIZE; x++) {
			dot[x] = d2.dot[x];
			counter[x] = d2.counter[x];
		}
		tester = true;
	}
	//-------------used to add an edge, checks to make sure it is legal-------------
	bool AddEdge(int x, char dir) 
	{
		bool found = false;
		char d = dir;
		if (dir == 'l') //edge to left is an edge to the right of the previous point. Was easier to make everything in terms of right and down
		{
			x = x - 1;
			d = 'r';
		}
		if (dir == 'u') //up is down of the point above
		{
			x = x - SX;
			d = 'd';
		}

		switch (d) 
		{
		case 'r':  // if right
			if (x%SX == SX - 1 || x > SIZE || x < 0) //if is on the edge or out of bounds
			{
				cout << "Invalid input\n";
				found =false;
			}
			else if (dot[x] == 0) //if no edges from point
			{
				dot[x] = 1;
				if (x - SX >= 0)
					counter[x - SX]++;
				//if (x + SX <= SIZE)
					//counter[x + SX]++;
				counter[x]++;
				found =true;
			}
			else if (dot[x] == 1 || dot[x] >= 3) //if right edge already exists
			{
				cout << "Edge already exists\n";
				found =false;
			}
			else if (dot[x] == 2) //if down edge exists
			{
				dot[x] = 3;
				if (x - SX >= 0)
					counter[x - SX]++;
				//if (x + SX <= SIZE)
					//counter[x + SX]++;
				counter[x]++;
				found =true;
			}
			else
			{
				cout << "Invalid input\n";
				found =false;
			}
			break;
		case 'd':
			if (x > SIZE - (SX+1)) //if bottom row or higher
			{
				cout << "Invalid input\n";
				found =false;
			}
			else if (dot[x] == 0) //if no edges from point
			{
				dot[x] = 2;
				if (x > 0)
					counter[x - 1]++;
				counter[x]++;
				found =true;
			}
			else if (dot[x] == 2 || dot[x] >= 3) //if down edge already exists
			{
				cout << "Edge already exists\n";
				found =false;
			}
			else if (dot[x] == 1) //if right edge exists
			{
				dot[x] = 3;
				if (x > 0)
					counter[x - 1]++;
				counter[x]++;
				found =true;
			}
			else
			{
				cout << "Invalid input\n";
				found =false;
			}
			break;
		default:
		{
			cout << "Invalid input\n";
			found =false;
		}
		}
		return found;
	}
	//------------- prints out the current board-------------
	void printer()
	{
		for (int x = 0; x < SIZE; x++) // go through all points
		{
			if (x%SX == 0) //puts row labels
				cout << setw(5) << x << ' ';
			cout << '+'; //sets up points
			if (dot[x] == 1 || dot[x] >= 3) //if right edge exists
				cout << '-' <<'-' << '-';
			else
				cout << ' ' << ' ' << ' ';
			if (x%SX == SX-1 && x!=0) //at end of the row
			{
				cout << endl << setw(7);
				for (int y = SX-1; y >= 0; y--)  //checks if lines exist from above points
				{
					if (dot[x - y] == 2 || dot[x - y] >= 3)  //if down edge exists
					{
						cout << '|' << ' ';
						if (dot[x - y] == 3)
							cout << ' ';
						else if (dot[x - y] == 4)
							cout << '1';
						else if (dot[x - y] == 5)
							cout << '2';
						else
							cout << ' ';
					}
					else
						cout << ' ' << ' ' << ' ';
					cout << ' ';
				}
				cout << endl;

			}
		}
	}
	//-------------checks to see if added edge made a box-------------
	bool checker(int x, char dir)
	{
		int i = x;
		bool found = false;
		char d = dir;
		if (dir == 'l') //edge to left is an edge to the right of the previous point. Was easier to make everything in terms of right and down
		{
			i = x - 1;
			d = 'r';
		}
		if (dir == 'u') //up is down of the point above
		{
			i = x - SX;
			d = 'd';
		}
		if (d == 'r') //only checks places that a box could be formed relative to new line
		{
			//checks if box exists below given line
			if (dot[i] >= 3)  //checks if down and right has a line
				if (dot[i + 1] == 2 || dot[i + 1] >= 3) //checks if line exists to the right
					if (dot[i + SX] == 1 || dot[i + SX] >= 3) //checks if line exists below
					{
						if(!tester)
							cout << "\n\n PLAYER " << turn % 2 + 1 << " FOUND A BOX \n\n";
						dot[i] = 3 + (turn % 2 + 1);
						if (turn % 2 + 1 == 1)
							scoreP1++;
						else
							scoreP2++;
						found = true;
					}
			//checks if box exists above the line
			if (i-SX >= 0) //if there even a point above start
				if(dot[i-SX] >= 3) //if 
					if (dot[i - SX + 1] == 2 || dot[i - SX + 1] >= 3)
					{
						if (!tester)
							cout << "\n\n PLAYER " << turn % 2 + 1 << " FOUND A BOX \n\n";
						dot[i-SX] = 3 + (turn % 2 + 1);
						if (turn % 2 + 1 == 1)
							scoreP1++;
						else
							scoreP2++;
						found = true;
					}
		}
		else if (d == 'd') //checks left and right of line to see if box formed
		{
			//checks if box to the left
			if(dot[i-1] >= 3 && i >0) 
				if (dot[i + SX - 1] == 1 || dot[i + SX - 1] >= 3)
				{
					if (!tester)
						cout << "\n\n PLAYER " << turn % 2 + 1 << " FOUND A BOX \n\n";
					dot[i-1] = 3 + (turn % 2 + 1);
					if (turn % 2 + 1 == 1)
						scoreP1++;
					else
						scoreP2++;
					found = true;
				}
			//checks if box to the right
			if(dot[i] >= 3)
				if(dot[i+1] == 2 || dot[i+1] >= 3)
					if (dot[i + SX] == 1 || dot[i + SX] >= 3)
					{
						if (!tester)
							cout << "\n\n PLAYER " << turn % 2 + 1 << " FOUND A BOX \n\n";
						dot[i] = 3 + (turn % 2 + 1);
						if (turn % 2 + 1 == 1)
							scoreP1++;
						else
							scoreP2++;
						found = true;
					}

		}
		/*if (found == false)
			turn++; */
		return found;
	}

	//-------------checks the OK values to make sure they actually ok to use. returns direction-----------------
	char okCheck(int x)
	{
		char input;
		if (dot[x] <= 1 && !(x > SIZE - (SX + 1))) //if the given point does not have a down edge
		{
			if (x > 0) {
				if (counter[x - 1] != 2) { //makes sure that will not create bad situation to left
					return 'd';
				}
			}
		}
		if((dot[x] == 0 || dot[x] == 2) && !(x%SX == SX - 1)) //if no right edge
			if (x - SX > 0) //checks if box above 
			{
				if (counter[x - SX] != 2) 
					return 'r';
			}
			else 
				return 'r';
			
		return 'x';		
	}




	//-------------Does AI stuff-------------
	pointDir AI()
	{
		pointDir value;
		vector<int> good;
		vector<int> bad;
		vector<int> ok;
		//vector<int>::iterator iter = good.begin();
		int i;
		char dir = 'x';
		bool choice = false;
		int randomIndex;

		for (int x = 0; x < SIZE; x++) //finds good(free box), bad(gives box to other player) and ok(no net gain)
		{
			if (counter[x] == 3)
				good.push_back(x);
			if (counter[x] == 2)
				bad.push_back(x);
			if (counter[x] <= 1)
				ok.push_back(x);
		}
		if (!good.empty()) //if there is a box to make, do this
		{
			i = good.back();
			pointDir goodPointDir = checkPointDir(i); //finds out what edge is legal to make
			dir = goodPointDir.dir;
			i = goodPointDir.point;
			good.pop_back(); //removes from potential good things
			choice = true;
		}
		else 
		{
			if (!ok.empty()) //checks all the ok positions, makes sure they arent bad
			{
				while (!ok.empty() && !choice) //while there is still ok choices and choice has not been made
				{
					randomIndex = rand() % ok.size();
					i = ok.at(randomIndex);
					//i = ok.back();
					dir = okCheck(i);
					if (dir == 'x')
					{
						bad.push_back(ok.at(randomIndex));
						ok.erase(ok.begin() + randomIndex);
					}
					else
					{
					//	cout << endl << i << dir;
						choice = true;
					}
				}
			}
			if(!choice) //if bad solutions are all that is left
			{
				value.point = -1;
				value.dir = 'r';
				return value;
			}
		}

		value.point = i;
		value.dir = dir;
		AddEdge(i, dir);
		return value;
		
	}
	//given point i, finds a legal edge to add. Used for good and bad elements
	pointDir checkPointDir(int i)
	{
		char dir = 'd';
		pointDir value;
		if (dot[i] <= 1) //if there no down edge, down is legal
			dir = 'd';
		else 
			if (dot[i] == 2) //if no right edge, right is legal
			{ 
				dir = 'r';
			}
		else 
			if (dot[i + 1] < 2) //if no down edge on point to the right, i+1 down is legal
			{
				i++;
				dir = 'd';
			}
			else //by process of elemination, the only legal edge is below the given edge and right
		{
			i += SX;
			dir = 'r';
		}
		value.dir = dir;
		value.point = i;
		return value;
	}
	//--------------If it can only make a bad choice, picks the best bad choice---------------------
	pointDir badChoice() //gets all the good and bad options. Picks random bad option, counts how many good options it produces, pick choice that produces fewest good choices
	{
		vector<int> good;
		vector<int> bad;
		vector<pointDir> potentialBadPointDir; //potential choices to pick from
		vector<int> costs; // cost associated with choice
		
		pointDir possible;
		pointDir input;
		int cost = 0;
		do // as long as there still options
		{
			cost = 0;
			good.clear();
			bad.clear();
			for (int x = 0; x < SIZE-(SX+1); x++) //gets all good and bad choices
			{
				if (x%SX != SX - 1) {
					if (counter[x] == 3)
						good.push_back(x);
					if (counter[x] <= 2)
						bad.push_back(x);
				}
			}

			while (!good.empty()) // as long as there are good options, add those edges, increase cost of action
			{
				
				input = checkPointDir(good.back());
				AddEdge(input.point, input.dir);
				checker(input.point, input.dir);
				printer();
				if (!good.empty()) {
					good.pop_back();
					cost++; //increase cost
				}
				good.clear();
				bad.clear();
					for (int x = 0; x < SIZE - (SX + 1); x++) //I should probably find a way to remove this loop
					{
						if (x%SX != SX - 1 && !(x > SIZE - (SX + 1))) {
							if (counter[x] == 3)
								good.push_back(x);
							if (counter[x] <= 2)
								bad.push_back(x);
					}
				}
			}
			
			if (!bad.empty() || (scoreP1+scoreP2 < (SX - 1)*(SY - 1))) {
				possible = checkPointDir(bad.back()); //gets a possible pointDir for bad choice
				potentialBadPointDir.push_back(possible); //adds to list of potential

				AddEdge(possible.point, possible.dir); //adds bad edge
				checker(possible.point, possible.dir);
				printer();
				//bad.clear();
			}
			if(cost != 0)
				costs.push_back(cost);
		} while (!bad.empty());
		vector<int>::iterator result = min_element(begin(costs), end(costs)); //finds the min cost
		
		return (potentialBadPointDir.at(distance(begin(costs), result))); //returns min cost bad point direction
	} 
};