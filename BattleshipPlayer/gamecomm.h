#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
//#include "msclr\marshal_cppstd.h"
#define ROWS 20
#define COLS 20

using namespace std;

int getGameBoard(int iGameBoard[ROWS][COLS])
{
	int iCur;
	int dim;
	char cInput;
	ifstream in("board.txt");
	in >> dim;
	cout << dim << endl;
	for(iCur=0; iCur<ROWS*COLS; iCur++)
	{
		if (iCur%COLS >= dim) {
			iCur = iCur + 20 - dim;
		}
		cInput=in.get();
		switch(cInput)
		{
		case '\n':
		case '\t':
		case ' ':
			iCur--;
			break;
		case '0':
			iGameBoard[iCur/COLS][iCur%COLS] = 0;
			break;
		case '1':
			iGameBoard[iCur / COLS][iCur%COLS] = 1;
			break;
		case '2':
			iGameBoard[iCur / COLS][iCur%COLS] = 2;
			break;
		case '3':
			iGameBoard[iCur / COLS][iCur%COLS] = 3;
			break;
		default:
			in.close();
			//return false;
		}
	}
	in.close();
	return dim;
}

bool putMove(int player, int iMoveRow, int iMoveCol)
{
	string n = to_string(player);
	string filename = "player";
	filename = filename.append(n.c_str());
	filename.append("move.txt");
	cout << filename << endl;
	ofstream out(filename);

	if(iMoveCol>=COLS || iMoveCol<0)
		return false;
	if(iMoveRow>=ROWS || iMoveRow<0)
		return false;
	out << "x" <<iMoveRow << "y" << iMoveCol << endl;
	out.close();
	return true;
}

bool setBoard(int player,int dim, vector<int> xseeds, vector<int> yseeds, vector<int> dseeds) {
	//msclr::interop::marshal_context context;
	//std::string standardString = context.marshal_as<std::string>("player" + player + "move.txt");
	//player1LastMove = standardString;
	string n = to_string(player);
	string filename = "player";
	filename = filename.append(n.c_str());// +"ships.txt";
	filename.append("ships.txt");
	cout << filename << endl;
	ofstream out(filename);

	for (int i = 0; i < dim; i++) {
		out << "x" << xseeds[i] << "y" << yseeds[i] << endl;
		out << dseeds[i] << endl;
	}
	out.close();
	return true;
}