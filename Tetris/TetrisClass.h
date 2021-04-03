#ifndef DEF_TETRIS
#define DEF_TETRIS


#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>

#include "SDLDisplay.hpp"
#include "Input.hpp"
#include "Colors.hpp"
#include "RNG.hpp"
#include "PieceClass.h"


class TetrisClass {
public:
	typedef std::vector<Colors::Color> colorVector;
	typedef std::vector<colorVector> colorMatrix;
	typedef std::vector<int> intVector;
	typedef std::vector<intVector> intMatrix;

	TetrisClass(std::string name, int h, int w, unsigned int tileSize, unsigned int horTileNbr, unsigned int verTileNbr);
	~TetrisClass();
	int play();

private:

	Input		input;

	void displayGame();
	void displayBoard();
	void displayTile(int x, int y, Colors::Color color);
	bool moveDown(PieceClass::Point& pos, std::vector<PieceClass::Point> points);
	void generateNewPiece();
	void printPiece();
	void checkFullLines();
	void removeFullLines(std::vector<int> fullLines);
	void updateScore(int linesNbr);
	void testLoose();
	void gameLoose();
	void updateFantom();
	long getTime();


	SDLDisplay display;
	const unsigned int tileSize;
	const unsigned int horTileNbr;
	const unsigned int verTileNbr;
	long score;
	long time;
	int baseCooldown;
	int cooldown;
	bool easterEgg;
	colorMatrix colorBoard;
	PieceClass *piece{ NULL };
	bool loose;
	
	bool fantom;
	std::vector<PieceClass::Point> fantomPoints;
	PieceClass::Point fantomPos;
};

#endif