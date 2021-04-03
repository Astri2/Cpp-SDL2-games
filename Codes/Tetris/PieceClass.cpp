#include "PieceClass.h"

PieceClass::PieceClass()
{
	color = Colors::Color();
	setMiddle(4);
}

void PieceClass::setMiddle(int horTileNumber) {
	middle = horTileNumber / 2;
	piecePos = { (double)middle, 1 };
}

const Colors::Color PieceClass::getColor()
{
	return color;
}

PieceClass::Point& PieceClass::getPos()
{
	return piecePos;
}

const std::vector<PieceClass::Point> PieceClass::getRelPoints() { 
	return pointPos;
}

const std::vector<PieceClass::Point> PieceClass::getAbsPoints() {
	std::vector<Point> absPoints = pointPos;
	for (int i = 0; i < 4; i++) {
		absPoints[i].x = floor(absPoints[i].x) + piecePos.x;
		absPoints[i].y = floor(absPoints[i].y) + piecePos.y;
	}
	return absPoints;
}

void PieceClass::moveLeft(std::vector<std::vector<Colors::Color>> colorBoard)
{
	std::vector<PieceClass::Point> absPiecePoints = getAbsPoints();
	bool canMove = true;
	for (int i = 0; i < 4; i++) {
		if (absPiecePoints[i].x - 1 < 0 || colorBoard[absPiecePoints[i].x - 1][absPiecePoints[i].y] != Colors::Black) {
			return;
		}
	}
	piecePos.x--;
}

void PieceClass::moveRight(std::vector<std::vector<Colors::Color>> colorBoard, int max)
{
	std::vector<PieceClass::Point> absPiecePoints = getAbsPoints();
	bool canMove = true;
	for (int i = 0; i < 4; i++) {
		if (absPiecePoints[i].x + 1 > (double)max - 1 || colorBoard[absPiecePoints[i].x + 1][absPiecePoints[i].y] != Colors::Black) {
			return;
		}
	}
	piecePos.x++;
}

void PieceClass::rotate(double angle, std::vector<std::vector<Colors::Color>> colorBoard, int xMax, int yMax) {
	std::vector<Point> points = pointPos;
	for (int i = 0; i < 4; i++) {
		
		double relX = points[i].x;
		double relY = points[i].y;
		
		points[i].x = ((angle == 90) ? -relY : relY);
		points[i].y = ((angle == 90) ? relX : -relX);
		
		double absX = points[i].x + piecePos.x; 
		double absY = points[i].y + piecePos.y;

		if (absX < 0 || absX >= xMax || absY < 0 || absY >= yMax ||
			colorBoard[absX][absY] != Colors::Black) return;
	}
	pointPos = points;
}
