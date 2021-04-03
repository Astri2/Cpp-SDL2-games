#include "Formes.h"

Bias::Bias() {
	pointPos = { {-1,0}, {0,0}, {0,1}, {1,1} };
	color = Colors::TCyan;
}

ReverseBias::ReverseBias() {
	pointPos = { {-1,1}, {0,1}, {0,0}, {1,0} };
	color = Colors::TGreen;
}

L::L() {
	pointPos = { {-1,-1}, {-1,0}, {0,0}, {1,0} };
	color = Colors::TBlue;
}

ReverseL::ReverseL() {
	pointPos = { {-1,0}, {0,0}, {1,0}, {1,-1} };
	color = Colors::TOrange;
}

T::T() {
	pointPos = { {0,-1}, {-1,0}, {0,0},{1,0} };
	color = Colors::TPurple;
}

Line::Line() {
	//pointPos = { {-1,0}, {0,0}, {1,0},{2,0} };
	pointPos = { {-1.5,0}, {-0.5,0}, {0.5,0},{1.5,0} };
	color = Colors::TYellow;
}

Square::Square() {
	pointPos = { {-0.5,-0.5}, {0.5,0.5}, {-0.5,0.5}, {0.5,-0.5} };
	color = Colors::TPink;
}