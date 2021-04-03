//#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Input.hpp"
#include "Colors.hpp"
#include "RNG.hpp"
#include "TetrisClass.h"



bool RNG::_initialized = false;
int main(int argc, char** argv)
{

	unsigned int tileSize   = 32;
	unsigned int horTileNbr = 10;
	unsigned int verTileNbr = 25;

	if (tileSize % 16 != 0) 
		tileSize -= tileSize % 16;
	if (tileSize == 0) 
		tileSize = 16;
	if (horTileNbr < 4) horTileNbr = 4;
	if (verTileNbr < 4) verTileNbr = 4;

	/*if (argc == 3 && atoi(argv[2]) % 16 == 0 && atoi(argv[3]) % 16 == 0) {
		std::cout << argv[1] << " " << argv[2] << std::endl;
		TetrisClass myTetris(argv[0], atoi(argv[1]), atoi(argv[2]));
		myTetris.play(); 
	}
	else {*/
	TetrisClass myTetris("jeu", 700, 700,tileSize,horTileNbr,verTileNbr);
	myTetris.play();
	//}

	return (0);
}



