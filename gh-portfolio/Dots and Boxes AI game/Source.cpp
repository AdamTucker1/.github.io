#include "Header.h"


int main()
{
	//bool x;
	int point;
	char dir;
	bool validEdge;
	pointDir value;
	int score = 0;

	int playerPos = 1;
	bool inputer = true;
	while (inputer)
	{
		cout << "\nDo you want to go first or second (1 or 2)?: ";
		cin >> playerPos;
		if (playerPos == 1 || playerPos == 2)
			inputer = false;
		else
			cout << "\nInvalid input";
	}
	Dots game(playerPos);
	
	while (score < BOXES)
	{
		score = game.scoreP1 + game.scoreP2; //gets total score
		cout << endl;
		for (int x = 0; x< SIZE; x++) //debug thing, displays dot value
		{
			if (x%SX == 0)
				cout << endl;
			cout << game.dot[x];
			
		}
		//
		cout << endl;
		for (int x = 0; x< SIZE; x++) //debug thing, displays counter value
		{
			if (x%SX == 0)
				cout << endl;
			cout << game.counter[x]; 

		}//*/
		cout << endl << "Player " << game.turn % 2 + 1 << "'s turn\n";
		if (game.turn % 2 + 1 == playerPos) { //checks if player's turn
			game.printer(); //prints board
			cout << "\nEnter point (0 to " << SIZE-1 << "):";
			cin >> point;
			cout << "\nEnter direction ((up, down, left, right) u, d, l, r): ";
			cin >> dir;
			validEdge = game.AddEdge(point, dir); //makes sure the edge is valid
			if (validEdge) 
				if (!game.checker(point, dir))
				{
					score = game.scoreP1 + game.scoreP2; //gets total score
					game.turn++;
				}
		}
		else { //else it the AI turn
			game.printer();
			score = game.scoreP1 + game.scoreP2;
			if (score >= BOXES)
				break;
			value = game.AI(); //gets the best possible point+direction
			if (value.point == -1) //if every
			{
				Dots gameTest(game);
				 value = gameTest.badChoice();
				 game.AddEdge(value.point, value.dir);
			}
			if (!game.checker(value.point, value.dir))
			{
				//score = game.scoreP1 + game.scoreP2; //gets total score
				game.turn++;
			}


			
		}
	}
	if (game.scoreP1 > game.scoreP2)
		cout << "PLAYER 1 WINS WITH A SCORE OF " << game.scoreP1 << " to " << game.scoreP2;
	else
		cout << "PLAYER 2 WINS WITH A SCORE OF " << game.scoreP2 << " to " << game.scoreP1;
	cin.get();
	do
	{
		cout << '\n' << "Press a key to continue...";
	} while (cin.get() != '\n');

	return 0;
}



