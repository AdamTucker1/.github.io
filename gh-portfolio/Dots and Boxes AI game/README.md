# Dots and Boxes AI
Coded By Adam Tucker and Calvin Chan
This project has the user play against an AI in a game of Dots and Boxes. The game has players taking turns placing lines on a grid of dots. 
If a player creates a box, they get a point and get to place another line. The game ends when there are no more places to place lines and the player with the most points wins. The AI attempts to get as many points as possible while minimizing the number of points given to the oppenent.

## Getting Started

Compile Source.cpp with Header.h in the same directory, using "g++ -std=c++11 Source.cpp".
./a.out to run the Source.exe file.

## How to play

When run, the program will ask if the user wants to be player 1 or player 2. By default, the board is a 4 by 4 grid of dots. To place a line, the user first picks a dot by entering a value (by default 0-15) with the first dot of each row being labeled. The user then chooses a direction from that dot, up, down, left or right by entering u, d, l, r. If it is a legal move, the line will be placed, else go again. If a box is made, a number fills the middle indicating the player that got the point and that player must place another line. The game ends when no more lines are possible moves.

## Authors

* **Adam Tucker** - *Coding* - [AdamTucker1](https://github.com/AdamTucker1)
* **Calvin Chan** - *Testing, report writing* 

