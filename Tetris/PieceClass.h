#include <vector>
#include <cmath>
#include <iostream>

#include "Colors.hpp"


#ifndef DEF_PIECE
#define DEF_PIECE

class PieceClass {
public:
	struct Point {
		double x;
		double y;
	};

	enum class PieceType {
		line, square, T, L, reverse_L, bias, reverse_bias
	};


	PieceClass();

	void setMiddle(int horTileNumber);

	const Colors::Color getColor();
	const std::vector<Point> getRelPoints();
	const std::vector<Point> getAbsPoints();
	Point& getPos();
	

	void moveLeft(std::vector<std::vector<Colors::Color>> colorBoard);
	void moveRight(std::vector<std::vector<Colors::Color>> colorBoard, int max);
	void rotate(double angle, std::vector<std::vector<Colors::Color>> colorBoard, int xMax, int yMax);

private:
	Point piecePos;
	int middle;

protected :

	Colors::Color color;
	std::vector<Point> pointPos;
};

#endif

