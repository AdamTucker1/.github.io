#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <list>
#include <vector>
#include <sstream>
using namespace std;
#define MAX 1000;

struct arrow
{
	bool color; // false for red, blue for true
	int direction; // 0 being up, 1 being NE, 2 being E, 3 being SE, ETC
};



//holds everything about the apollo dianna puzzle
class Apollo
{
private:

public:
	Apollo() //default constructor
	{

	}
	vector<int> adj[1000]; // adjacenty list

	int sx = 0; //size x
	int sy = 0; //size y
	int dist[1000], pred[1000];

	string inputArrow;
	string line;
	arrow  outputArrow;
	arrow puzzle[1000]; //There can be no more than 1000 inputs in the input file because I dont want to use 2d vectors
	int start = 0; //start point, zero by default
	int finish = 0; // Goal ,gets changed when O is found

	//------Breaks each input into color and direction----------
	arrow breaker(string line)
	{
		char color;
		char dir[3];
		arrow value;
		int len = line.length();
		color = line.at(0);
		dir[0] = line.at(2);
		if (len > 3)
			dir[1] = line.at(3);
		if (color == 'R')
			value.color = false;
		else
			value.color = true;

		switch (dir[0]) {
		case 'E': value.direction = 2; //if it E or W, there can be no other letter
			break;
		case 'W': value.direction = 6;
			break;
		case 'N': //if it is N, then it can be N, NE or NW
			if (len == 3)
				value.direction = 0;
			else if (dir[1] == 'E')
				value.direction = 1;
			else
				value.direction = 7;
			break;
		case 'S':
			if (len == 3)
				value.direction = 4;
			else if (dir[1] == 'E')
				value.direction = 3;
			else
				value.direction = 5;
			break;
		}
		return value;
	}
	//----------creates an array of arrow structs which hold color and direction----------
	void arrMaker(string input)
	{
		int current = 0;
		ifstream fp(input);

		if (!fp.is_open()) { //makes sure we can open the input file
			fprintf(stderr, "Can't open file: %s\n", "input.txt");
			exit(1);
		}



		fp >> sy >> sx; //this gets the number of operations

		for (int y = 0; y < sy; y++)  //goes through operations 1 by 1
		{
			getline(fp, line); //gets next line
			for (int x = 0; x < sx; x++)
			{
				current = x + sx*y;
				fp >> inputArrow;
				if (inputArrow == "O")
				{
					
					outputArrow.color = false; //arbitrary
					outputArrow.direction = 8; //8 is goal state
					puzzle[current] = outputArrow;
					finish = current;
				}
				else
					puzzle[current] = breaker(inputArrow);
			}
		}
	}

	//---------------adds edge to adjacency list, probably not needed given that I put everything into a class anyway----------
	void addEdge(int src, int dest)
	{
		adj[src].push_back(dest);
	}

	//-----------does breadth first search to find shortest path----------
	bool BFS(int src, int dest)
	{
		list<int> queue;
		bool visited[100];
		
		int size = sx*sy;
		for (int i = 0; i < size; i++) {
			visited[i] = false;
			dist[i] = 9999;
			pred[i] = -1;
		}
		visited[src] = true; //sets first place to visited
		dist[src] = 0; // distance to self is 0
		queue.push_back(src); // adds start in queue

		while (!queue.empty()) //while there are elements in queue
		{
			int u = queue.front();
			queue.pop_front();
			for (int i = 0; i < adj[u].size(); i++) //check all the near by elements
			{
				if (visited[adj[u][i]] == false) // if not visited add to queue
				{
					visited[adj[u][i]] = true; //now visited
					dist[adj[u][i]] = dist[u] + 1;  // adds dist
					pred[adj[u][i]] = u;  //adds to previous
					queue.push_back(adj[u][i]); //adds visited node to queue


					if (adj[u][i] == dest) //end if dest found
						return true;
				}
			}
		}
		return false;
	}

	//----------prints shortest path ----------------------
	void printShort(string output)
	{
		//int pred[100], dist[100];
		ofstream out(output);
		if (BFS(start, finish) == false)
		{
			cout << "Source and destination are not connected";
			return;
		}

		// vector path stores the shortest path
		vector<int> path;
		vector<int> pathDist;
		int crawl = finish; //starts at end
		path.push_back(crawl); // pushes into path
		pathDist.push_back(0); // pushes into path
		while (pred[crawl] != -1) { //while there is a predecessor
			path.push_back(pred[crawl]); //add preddecessor to path
			pathDist.push_back(distTo(pred[crawl], crawl));
			crawl = pred[crawl]; //check pred
		}


		// printing path from source to destination
		for (int i = path.size() - 1; i >= 1; i--)
		{
			int pos = path[i];
			int dist = pathDist[i];

			cout  << dist << dirToString(puzzle[pos].direction) << " ";
			out << dist << dirToString(puzzle[pos].direction) << " ";
		}
	}

	//----------finds edges that are connected, adds them to adjacenty list---------
	void connector(int x)
	{
		bool inColor = puzzle[x].color; //input color
		int direct = puzzle[x].direction; //input direction

		int move;

		if (direct == 8)
			return;

		switch (direct) { //figures out which point in the array to check to get next value
		case 0: //N
			move = -sx;
			break;
		case 1: //NE
			move = -sx + 1;
			break;
		case 2: //E
			move = 1;
			break;
		case 3: //SE
			move = sx + 1;
			break;
		case 4: //S
			move = sx;
			break;
		case 5: //SW
			move = sx - 1;
			break;
		case 6: //W
			move = -1;
			break;
		case 7: //NW
			move = -sx - 1;
			break;
		default:
			move = 0;
		}

	
		//checks if colors are different in line, if so then add to adj list
		int bound = x%sx;
		int nextBound = 0;
		int delta = 0;

		for (int xVal = x + move; xVal >= 0 && xVal < sx*sy; xVal += move)
		{
			//cout << endl << xVal;
			nextBound = xVal%sx; //checks to make sure it doesnt loop around
			delta = abs(bound - nextBound);
			//cout << "\nDelta" << delta;
			bound = nextBound;
			if (delta > 1) // if it looped around, there nothing left to check
				break;
			else
			if (inColor != puzzle[xVal].color || puzzle[xVal].direction == 8) //if it a different color or the goal
			{
				addEdge(x, xVal);
			}
		}
	}
	// ----------changes my int directions back into normal directions----------
	string dirToString(int x)
	{
		switch (x) {
		case 0: //N
			return "N";
		case 1: //NE
			return "NE";
		case 2: //E
			return "E";
		case 3: //SE
			return "SE";
		case 4: //S
			return "S";
		case 5: //SW
			return "SW";
		case 6: //W
			return "W";
		case 7: //NW
			return "NW";
		case 8:
			return "O";

		default:
			return "Invalid";
		}
	}
	//---------- finds distance, was hacked together when I realised that I forgot to calculate this
	int distTo(int s, int f)
	{
		int d = puzzle[s].direction;
		int diff = abs(s - f); 
		if (d == 2 || d == 6) // if east or west, it just the difference between s and f
			return diff;
		else if (d == 3 || d == 7) //if NW or SE the distance between 2 points increases by size in the x+1 each time
			return diff / (sx + 1);
		else if (d == 1 || d == 5) //if SW or NE, the distance betwen 2 points increases by size in the x -1 each time
			return (diff / (sx - 1)); 
		else
			return (diff / sx); //if N or S, change is just by sx
	}

};



//arrow breaker(string line);

int main()
{

	Apollo game;
	string inputDef = "apollodiana.txt";
	string input = inputDef;
	string output = "output.txt";

	cout << "Enter the name of the file you wish to open (apollodiana.txt by default): ";
	getline(cin, input);
	if (!input.empty()) {
		istringstream stream(input);
		stream >> input;
	}
	else
		input = inputDef;
	

	game.arrMaker(input);
	int size = (game.sx) * (game.sy); //size of the puzzle board
	for (int x = 0; x < size; x++)
	{
		game.connector(x); //creates adj list
	}

	game.printShort(output);

	do
	{
		cout << "\n\nPress a key to continue...";
	} while (cin.get() != '\n');
	return 1;
}






