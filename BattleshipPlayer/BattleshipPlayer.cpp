// BattleshipPlayer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include "gamecomm.h"

using namespace std;

bool somethingWentWrong = false;

int player;

const int MAX_BOARD_SIZE = 20;

class Ship {
public: short length; // length of ship, how far it goes from point of origin
		short width; // width of ship, does it bleed into adjacent spaces, only 1 allowed so far
		short xCoord; // point of origin
		short yCoord; // point of origin
		short orientation; // 1 = west, 2 = north, 3 = east, 4 = south. If a ship has width>1, even widths will protrude east/south
		short hp; // how many hits left to sunk. by default initialized to width*length

		Ship(short l, short w, short x, short y, short or ) {
			length = l;
			if (w != 1) { std::cout << "width is only allowed to be 1" << std::endl; }
			width = 1;
			xCoord = x;
			yCoord = y;
			orientation = or ;
			hp = length * width;
		}
};
struct Space {
	bool revealed; // revealed upon the opponent guessing this space
	bool shipPresent; // ship is present in this tile
	Ship* ship;
};
// struct defining the board
class Board {
public:
	int dimension;
	int player; // 1 == player1, 2 == player2
	Space* getSpace(int x, int y) {
		if (x < dimension && y < dimension) {
			return board[x][y];
		}
		return NULL;
	}
	void setSpace(int x, int y, bool reveal, bool present, Ship* s) {
		if (x < dimension && y < dimension) {
			board[x][y]->revealed = reveal;
			board[x][y]->shipPresent = present;
			board[x][y]->ship = s;
		}
	}
private: Space* board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]; // top left = (0,0)

public: Board(int dim, Ship ships[], int player) {
	if (dim > MAX_BOARD_SIZE) exit(-1); // invalid dim
	dimension = dim;
	this->player = player;
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			board[i][j] = new Space;
			board[i][j]->revealed = false;
			board[i][j]->shipPresent = false;
			if (this->player == 1) {

			}
		}
	}

	// note: check no ships are overlapping
	//MessageBox::Show("before for");
	for (int i = 0; i < sizeof(ships) / sizeof(&ships); i++) // for loop working? ffs - how to loop over array in cpp
	{
		//MessageBox::Show("in for");
		board[ships[i].xCoord][ships[i].yCoord]->shipPresent = true;
		board[ships[i].xCoord][ships[i].yCoord]->ship = &ships[i];
		if (ships[i].length > 1) {
			switch (ships[i].orientation) {
			case 1: // west

				for (int k = 1; k < ships[i].length; k++)
				{
					if (board[ships[i].xCoord - k][ships[i].yCoord]->shipPresent == true)somethingWentWrong = true; // if a ship is already here, overlapping ships!
					board[ships[i].xCoord - k][ships[i].yCoord]->shipPresent = true;
					board[ships[i].xCoord - k][ships[i].yCoord]->ship = &ships[i];
				}

				break;
			case 2: // north
				for (int k = 1; k < ships[i].length; k++)
				{
					if (board[ships[i].xCoord][ships[i].yCoord - k]->shipPresent == true)somethingWentWrong = true;
					board[ships[i].xCoord][ships[i].yCoord - k]->shipPresent = true;
					board[ships[i].xCoord][ships[i].yCoord - k]->ship = &ships[i];
				}
				break;
			case 3: // east
				for (int k = 1; k < ships[i].length; k++)
				{
					if (board[ships[i].xCoord + k][ships[i].yCoord]->shipPresent == true)somethingWentWrong = true;
					board[ships[i].xCoord + k][ships[i].yCoord]->shipPresent = true;
					board[ships[i].xCoord + k][ships[i].yCoord]->ship = &ships[i];
				}
				break;
			case 4: // south
				for (int k = 1; k < ships[i].length; k++)
				{
					if (board[ships[i].xCoord][ships[i].yCoord + k]->shipPresent == true)somethingWentWrong = true;
					board[ships[i].xCoord][ships[i].yCoord + k]->shipPresent = true;
					board[ships[i].xCoord][ships[i].yCoord + k]->ship = &ships[i];
					//MessageBox::Show("placing that ship");
				}
				break;
			//default:
				//MessageBox::Show("you shouldn't be here");
			}
		}
	}

}


};

bool getNewBoard() {
	int n[20][20];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			n[i][j] = 0;
		}
	}
	srand(time(NULL));
	int numShips;
	int dim;
	int temp;
	char d;
	vector<int> length;
	vector<int> width;
	vector<int> xseeds;
	vector<int> yseeds;
	vector<int> dseeds;
	ifstream in("settings.txt");
	in >> dim;
	in >> numShips;

	for (int i = 0; i < numShips; i++) {
		in >> d;
		in >> temp;
		length.push_back(temp);
		in >> d;
		in >> temp;
		width.push_back(temp);
	}
	int xseed;
	int yseed;
	int dseed;
	bool valid = true;
	for (int i = 0; i < numShips; i++) {
		xseed = rand() % dim;
		yseed = rand() % dim;
		dseed = 1 + rand() % 4;
		if (n[xseed][yseed] == 0) {
			if (dseed == 1) {
				if (xseed - length[i] >= 0) {
					for (int j = 0; j < length[i]; j++) {
						if (n[xseed - j][yseed] != 0) {
							valid = false;
						}
					}
				}
				else {
					valid = false;
				}
			}
			else if (dseed == 2) {
				if (yseed - length[i] >= 0) {
					for (int j = 0; j < length[i]; j++) {
						if (n[xseed][yseed - j] != 0) {
							valid = false;
						}
					}
				}
				else {
					valid = false;
				}
			}
			else if (dseed == 3) {
				if (xseed + length[i] < dim) {
					for (int j = 0; j < length[i]; j++) {
						if (n[xseed + j][yseed] != 0) {
							valid = false;
						}
					}
				}
				else {
					valid = false;
				}
			}
			else if (dseed == 4) {
				if (yseed + length[i] < dim) {
					for (int j = 0; j < length[i]; j++) {
						if (n[xseed][yseed+j] != 0) {
							valid = false;
						}
					}
				}
				else {
					valid = false;
				}
			}
		}
		else {
			valid = false;
		}
		if (valid == true) {
			dseeds.push_back(dseed);
			yseeds.push_back(yseed);
			xseeds.push_back(xseed);
			if (dseed == 1) {
				for (int j = 0; j < length[i]; j++) {
					n[xseed - j][yseed]++;
				}
			}
			else if (dseed == 2) {
				for (int j = 0; j < length[i]; j++) {
					n[xseed][yseed - j]++;
				}
			}
			else if (dseed == 3) {
				for (int j = 0; j < length[i]; j++) {
					n[xseed + j][yseed]++;
				}
			}
			else if (dseed == 4) {
				for (int j = 0; j < length[i]; j++) {
					n[xseed][yseed + j]++;
				}
			}
		}
		else if (valid == false) {
			i--;
			valid = true;
		}
	}
	//int player = 1;
	setBoard(player, numShips, xseeds, yseeds, dseeds);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			cout << n[i][j];
		}
		cout << endl;
	}
	in.close();
	return true;
}


int getMax(int dim, int w[20][20]) {
	int max = 0;
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (w[i][j] > max) {
				max = w[i][j];
			}
			cout << w[i][j];
		}
		cout << endl;
	}
	return max;
}

void makeMove(int dim, int n[20][20]) {
	srand(time(NULL));
	int w[20][20];
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			w[i][j] = 0;
		}
	}
	int max;

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (n[i][j] != 0) {
				w[i][j] = 0;
				//cout << "you got here " << i << j << endl;
			}
			else {
				//cout << "u be here" << endl;
				if (i > 0) {
					if (n[i - 1][j] == 0) {
						w[i][j]++;
					}
					else if (n[i - 1][j] == 2) {
						w[i][j] += 4;
					}
				}
				if (i < dim - 1) {
					if (n[i + 1][j] == 0) {
						w[i][j]++;
					}
					else if (n[i + 1][j] == 2) {
						w[i][j] += 4;
					}
				}
				if (j > 0) {
					if (n[i][j-1] == 0) {
						w[i][j]++;
					}
					else if (n[i][j - 1] == 2) {
						w[i][j] += 4;
					}
				}
				if (j < dim - 1) {
					if (n[i][j+1] == 0) {
						w[i][j]++;
					}
					else if (n[i][j + 1] == 2) {
						w[i][j] += 4;
					}
				}
			}
		}
	}
	max = getMax(dim, w);
	cout << "I be here "  << max << endl;
	vector<int> v1;
	vector<int> v2;
	int count = 0;
	int randomSeed = 0;
	int row;
	int col;

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			if (w[i][j] == max) {
				v1.push_back(i);
				v2.push_back(j);
				count++;
			}
		}
	}
	cout << "I be here too "  << count << endl;
	randomSeed = rand() % count;
	row = v1[randomSeed];
	col = v2[randomSeed];
	cout << row << col << endl;
	putMove(player, row, col);
}

void main(int argc, char* argv[])
{
	int dim;
	if (strcmp(argv[1], "1") == 0) {
		player = 1;
	}
	else {
		player = 2;
	}
	if (true){//strcmp(argv[2], "1") == 0) {
		getNewBoard();
		cout << "here";
	}
	else {
		int n[20][20];
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				n[i][j] = 9;
			}
		}

		dim = getGameBoard(n);// cout << "didn't work?" << endl;
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				cout << n[i][j];
			}
			cout << endl;
		}
		makeMove(dim, n);
	}
	system("pause");

}
