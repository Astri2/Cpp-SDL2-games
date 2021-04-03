#include "TetrisClass.h"
#include "Formes.h"


TetrisClass::TetrisClass(std::string name, int h, int w, unsigned int tileSize, unsigned int horTileNbr, unsigned int verTileNbr)
	: display(name, w, h), tileSize{ tileSize }, horTileNbr{ horTileNbr }, verTileNbr{ verTileNbr } 
{
	colorBoard = { horTileNbr, (colorVector(verTileNbr, Colors::Black)) };
	generateNewPiece();
	updateFantom();
	score = 0;
	baseCooldown = 1000;
	cooldown = baseCooldown;
	time = getTime();
	easterEgg = false;
	loose = false;
	fantom = false;
}

TetrisClass::~TetrisClass() {
	std::cout << "deleting pointers...";
	if (piece != NULL)
		delete(piece);
}

int TetrisClass::play() {
	while (!(input.shouldExit()) && !(input.getKeyState(SDL_SCANCODE_ESCAPE)) || loose)
	{
		/*   SDL   */
		input.flushEvents();
		
		/*   AUTOMATIC EVENTS   */
		if (getTime() - time >= cooldown) {
			time = getTime();
			if (!moveDown(piece->getPos(), piece->getRelPoints())) {
				printPiece();
				generateNewPiece();
			}
		}

		cooldown = baseCooldown;

		//fantomPiece->setPoints(piece->getRelPoints());
		//fantomPiece->setPos(piece->getPos());
		//while (moveDown(fantomPiece->getPos(),fantomPiece->getRelPoints()));
		

		/*   KEYBOARD CONTROLS   */

		//*****************************************// DEBUG
		if (input.getKeyStateOnce(SDL_SCANCODE_RETURN)) {
			if (!moveDown(piece->getPos(),piece->getRelPoints())) {
				printPiece();
				generateNewPiece();
			}
			updateFantom();
		}

		if (input.getKeyStateOnce(SDL_SCANCODE_SPACE)) {
			generateNewPiece();
			updateFantom();
		}

		//*****************************************//
		
		if (input.getKeyStateOnce(SDL_SCANCODE_F)) {
			fantom ? fantom = false : fantom = true;
		}

		if (input.getKeyStateOnce(SDL_SCANCODE_A)) {
			piece->rotate(270, colorBoard, horTileNbr, verTileNbr);
			updateFantom();
		}

		if (input.getKeyStateOnce(SDL_SCANCODE_D)) {
			piece->rotate(90, colorBoard, horTileNbr, verTileNbr);
			updateFantom();
		}
		
		if (input.getKeyStateOnce(SDL_SCANCODE_UP)) {
			while (moveDown(piece->getPos(), piece->getRelPoints())) {}
			printPiece();
			generateNewPiece();
			updateFantom();
		}

		if (input.getKeyState(SDL_SCANCODE_DOWN))
			cooldown = baseCooldown/10;

		if(input.getKeyStateOnce(SDL_SCANCODE_LEFT)) {
			piece->moveLeft(colorBoard);
			updateFantom();
		}
		if (input.getKeyStateOnce(SDL_SCANCODE_RIGHT)) {
			piece->moveRight(colorBoard, horTileNbr);
			updateFantom();
		}

		if (input.getKeyState(SDL_SCANCODE_E))
			easterEgg = true;
		else
			easterEgg = false;

		/*   DISPLAY   */
		TetrisClass::displayGame();
	}

	return 0;
}

void TetrisClass::displayGame() {
	display.clearScreen();

	displayBoard();

	display.refreshScreen();
}

void TetrisClass::displayBoard() {
	//presets
	int x = (display.getWinW() - horTileNbr * tileSize) / 2; //horizontal centralization
	int y = (display.getWinH() - verTileNbr * tileSize) / 2; //vertical centralization
	int scale = tileSize / 16;

	//board grid
	for (unsigned int i = 0; i <= horTileNbr; i++)
		display.putRect((x - tileSize / 8) + i * tileSize, y - tileSize / 8, 4 * scale, tileSize * verTileNbr + (tileSize / 4), Colors::Blue);

	for (unsigned int i = 0; i <= verTileNbr; i++)
		display.putRect(x - tileSize / 8, (y - tileSize / 8) + i * tileSize, tileSize * horTileNbr + (tileSize / 4), 4 * scale, Colors::Blue);

	//dead line
	display.putRect(x, (y - scale) + 2 * tileSize, tileSize * horTileNbr, 2 * scale, Colors::Red);

	//dead pieces
	for (size_t i = 0; i < colorBoard.size(); i++) {
		for (size_t k = 0; k < colorBoard[i].size(); k++) {
			if (!colorBoard[i][k] == Colors::Black)
				displayTile(i * tileSize, k * tileSize, colorBoard[i][k]);
		}
	}

	//current piece
	std::vector<PieceClass::Point> absPiecePoints = piece->getAbsPoints();
	for (int i = 0; i < 4; i++) {
		displayTile(absPiecePoints[i].x * tileSize, absPiecePoints[i].y * tileSize, piece->getColor());
	}

	//fantom piece
	if (fantom) {
		for (int i = 0; i < 4; i++) {
			int absX = fantomPoints[i].x + fantomPos.x;
			int absY = fantomPoints[i].y + fantomPos.y;
			display.putRect(x + absX * tileSize + scale * 6, y + absY * tileSize + scale * 6,
				scale * 4, scale * 4, piece->getColor());
		}
	}
}

void TetrisClass::displayTile(int x, int y, Colors::Color color) {
	if (easterEgg) color = Colors::TRed;
	x += (display.getWinW() - horTileNbr * tileSize) / 2; //horizontal centralization
	y += (display.getWinH() - verTileNbr * tileSize) / 2; //vertical centralization

	int scale = tileSize / 16;
	Colors::Color lightGrey = Colors::TGreyLight;
	Colors::Color grey = Colors::TGrey;
	Colors::Color white = Colors::TWhite;
	Colors::Color colorDark = Colors::White;
	switch (color) {
	case Colors::TRed: colorDark = Colors::TRedDark; break;
	case Colors::TPink: colorDark = Colors::TPinkDark; break;
	case Colors::TPurple: colorDark = Colors::TPurpleDark; break;
	case Colors::TBlue: colorDark = Colors::TBlueDark; break;
	case Colors::TCyan: colorDark = Colors::TCyanDark; break;
	case Colors::TGreen: colorDark = Colors::TGreenDark; break;
	case Colors::TYellow: colorDark = Colors::TYellowDark; break;
	case Colors::TOrange: colorDark = Colors::TOrangeDark; break;
	}

	display.putRect(x, y, tileSize, tileSize, lightGrey);

	display.putRect(x, y, tileSize - 1 * scale, tileSize - 1 * scale, white);
	display.putRect(x + 1 * scale, y + 1 * scale, tileSize - 1 * scale, tileSize - 1 * scale, grey);

	display.putRect(x + 1 * scale, y + 1 * scale, tileSize - 2 * scale, tileSize - 2 * scale, lightGrey);

	display.putRect(x + 3 * scale, y + 3 * scale, tileSize - 6 * scale, tileSize - 6 * scale, colorDark);
	display.putRect(x + 4 * scale, y + 4 * scale, tileSize - 6 * scale, tileSize - 6 * scale, white);

	display.putRect(x + 4 * scale, y + 4 * scale, tileSize - 7 * scale, tileSize - 7 * scale, color);
}

bool TetrisClass::moveDown(PieceClass::Point& pos, std::vector<PieceClass::Point> points) {
	for (int i = 0; i < 4; i++) {
		int x = pos.x + points[i].x;
		int y = pos.y + points[i].y;
		if (y + 1 > (int)verTileNbr - 1 || colorBoard[x][y + 1] != Colors::Black) {
			return false;
		}
	}
	pos.y++;
	return true;
}

void TetrisClass::generateNewPiece() {

	if (piece != NULL) {
		checkFullLines();
		delete(piece);

		std::cout << "score = " << score << std::endl <<
			"cooldown = " << baseCooldown << std::endl <<
			"==============================" << std::endl;
	}

	baseCooldown *= 0.999;
	int randomInt = RNG::generate(7);
	switch (randomInt) {
	case 0:
		piece = new Line();
		break;
	case 1:
		piece = new Square();
		break;
	case 2:
		piece = new T();
		break;
	case 3:
		piece = new L();
		break;
	case 4:
		piece = new ReverseL();
		break;
	case 5:
		piece = new Bias();
		break;
	case 6:
		piece = new ReverseBias();
		break;
	default:
		//should not happen
		break;
	}
	testLoose();
	piece->setMiddle(horTileNbr);
	updateFantom();
}

void TetrisClass::printPiece() {
	Colors::Color color = piece->getColor();
	std::vector<PieceClass::Point> absPiecePoints = piece->getAbsPoints();
	for (int i = 0; i < 4; i++) {
		colorBoard[absPiecePoints[i].x][absPiecePoints[i].y] = color;
	}
}

void TetrisClass::checkFullLines() {
	std::vector<int> editedLines;
	std::vector<int> fullLines;
	for (PieceClass::Point i : piece->getAbsPoints()) {
		if (std::find(editedLines.begin(), editedLines.end(), i.y) != editedLines.end()) continue;
		
		editedLines.push_back(i.y);
		//std::cout << "edited : " << i.y << std::endl;
		
		bool full = true;
		for (auto column : colorBoard) {
			if (column[i.y] == Colors::Black) {
				full = false;
				break;
			}
		}
		if (full) {
			fullLines.push_back(i.y);
		}
	}
	std::sort(fullLines.begin(), fullLines.end());
	if (fullLines.size() != 0) {
		updateScore(fullLines.size());
		removeFullLines(fullLines);
	}
	
	//std::cout << "==================" << std::endl;
}

void TetrisClass::removeFullLines(std::vector<int> fullLines) {
	colorMatrix oldBoard = colorBoard;
	colorMatrix newBoard = colorBoard;
	for (int i : fullLines) {
		//std::cout << "full : " << i << std::endl;
		for (unsigned int k = 0; k < horTileNbr; k++) {
			newBoard[k].erase(newBoard[k].begin() + i);
			newBoard[k].insert(newBoard[k].begin(), Colors::Black);
		}
	}
	long now = getTime();
	colorBoard = newBoard;
	displayGame();
	while (!(input.shouldExit()) && !(input.getKeyState(SDL_SCANCODE_ESCAPE))) {
		if (getTime() - now >= 200) {
			colorBoard = newBoard;
			return;
		}
		if (getTime() - now >= 100) {
			colorBoard = oldBoard;
			displayGame();
			continue;
		}
	}
}

void TetrisClass::updateScore(int linesNbr) {
	switch (linesNbr) {
	case 4:
		score += 900;
		baseCooldown *= 0.99;
	case 3:
		score += 200;
		baseCooldown *= 0.99;
	case 2:
		score += 60;
		baseCooldown *= 0.99;
	case 1:
		score += 40;
		baseCooldown *= 0.99;
		break;
	}
}

void TetrisClass::testLoose() {
	for (colorVector column : colorBoard) {
		if (column[1] != Colors::Black) {
			gameLoose();
		}
	}
}

void TetrisClass::gameLoose() {
	loose = true;
}

void TetrisClass::updateFantom() {
	if (!fantom) return;
	fantomPos = piece->getPos();
	fantomPoints = piece->getRelPoints();
	while (moveDown(fantomPos, fantomPoints));
}

long TetrisClass::getTime() {
	using namespace std::chrono;

	long ms = (long)duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()).count();
	return ms;
}