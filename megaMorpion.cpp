//#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "SDLDisplay.hpp"
#include "Input.hpp"
#include "Colors.hpp"
#include "RNG.hpp"

//#include <windows.h>

using namespace std;
bool RNG::_initialized = false;
Input		input;
SDLDisplay	display("minijeu", 900, 900);

typedef vector<char> vectorC;
typedef vector<vectorC> matrixC;

struct item
{
	int x;
	int y;
};

//ensemble des données de la taille de la fenêtre
int sizeH = display.getWinH();
int sizeW = display.getWinW();
int size_big_tileH = sizeH / 3 - sizeH / 30;
int size_big_tileW = sizeW / 3 - sizeW / 30;
int size_small_tileH = size_big_tileH / 3 - size_big_tileH / 30;
int size_small_tileW = size_big_tileW / 3 - size_big_tileW / 30;

void testPos(item& cursor, int cursor_mode, item old_pos) //updateCursor -> corrige l'emplacement du curseur 
{
	if (cursor_mode == 1) {
		while (cursor.x > 3 * (old_pos.x / 3) + 2) {
			cursor.x--;
		}
		while (cursor.x < 3 * (old_pos.x / 3)) {
			cursor.x++;
		}
		while (cursor.y > 3 * (old_pos.y / 3) + 2) {
			cursor.y--;
		}
		while (cursor.y < 3 * (old_pos.y / 3)) {
			cursor.y++;
		}
	}
}

void getPos(item& cursor) //updateCursor -> récupère la position du curseur
{
	cursor.x = input.getMousePos()[0] / (sizeW / 9);
	cursor.y = input.getMousePos()[1] / (sizeH / 9);
}

void updateCursor(item& cursor, int cursor_mode) { //met a jour l'emplacement du curseur
	item old_pos = cursor;
	getPos(cursor);
	testPos(cursor, cursor_mode, old_pos);
}

bool canPlay(matrixC& board, item& cursor, matrixC& win_board) //vérifie que le curseur est dans une case jouable et que la souris est bien placée
{
	if (input.getMousePos()[0] / (sizeW / 9) != cursor.x || input.getMousePos()[1] / (sizeH / 9) != cursor.y)
		return false;
	if (board[cursor.x][cursor.y] != 'e')
		return false;
	if (win_board[cursor.x / 3][cursor.y / 3] != 'e')
		return false;
	return true;
}

void putCursor(int x, int y) //display_screen -> affiche le curseur
{
	display.putRect(x, y, size_small_tileW, size_small_tileH, Colors::Yellow);
	display.putRect(x + size_small_tileW * 0.1, y + size_small_tileH * 0.1, size_small_tileW * 0.8, size_small_tileH * 0.8, Colors::Black);	
}

void putCross(int x, int y, int size_x = size_small_tileW, int size_y = size_small_tileH) //display_screen -> affiche les croix
{
	display.putRect(x + size_x * 0.35, y + size_y * 0.1, size_x * 0.3, size_y * 0.8, Colors::Dark_Green);
	display.putRect(x + size_x * 0.1, y + size_y * 0.35, size_x * 0.8, size_y * 0.3, Colors::Dark_Green);
}

void putRound(int x, int y, int size_x = size_small_tileW, int size_y = size_small_tileH) //display_screen -> affiche les ronds
{
	display.putRect(x + size_x * 0.1, y + size_y * 0.1, ceil(size_x * 0.8), ceil(size_y * 0.8), Colors::Dark_Red);
	display.putRect(x + size_x * 0.3, y + size_y * 0.3, size_x * 0.4, size_y * 0.4, Colors::Black);
}

bool can_display(matrixC& board, item& cursor, matrixC& win_board) //display_screnn -> teste si le curseur est dans un endroit jouable et affichable
{
	if (board[cursor.x][cursor.y] != 'e')
		return false;
	if (win_board[cursor.x / 3][cursor.y / 3] != 'e')
		return false;
	return true;
}

void display_screen(matrixC board, item cursor, char turn, int cursor_mode, matrixC win_board) //gestion de l'affichage de la fenetre
{
	display.clearScreen(); // "Nettoie" la fenêtre

	if (cursor_mode == 0) { // fond de couleur quand on joue global
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				display.putRect(sizeW / 3 * i + sizeW / 60, sizeH / 3 * k + sizeH / 60, size_big_tileW, size_big_tileH, (turn == 'r' ? 0x780D0D : 0x25630C));
			}
		}
	}
	else { 
		for (int i = 0; i < 3; i++) { // fond bleu et couleur quand on joue local
			for (int k = 0; k < 3; k++) {
				display.putRect(sizeW / 3 * i + sizeW / 60, sizeH / 3 * k + sizeH / 60, size_big_tileW, size_big_tileH, Colors::Blue);
			}
		}
		display.putRect(sizeW / 3 * (cursor.x / 3) + sizeW / 60, sizeH / 3 * (cursor.y / 3) + sizeH / 60, size_big_tileW, size_big_tileH, (turn == 'r' ? 0x780D0D : 0x25630C));
	}

	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			for (int j = 0; j < 3; j++) {
				for (int h = 0; h < 3; h++) {

					display.putRect(sizeW / 3 * i + sizeW / 60 + size_big_tileW / 3 * j + size_big_tileW / 60, // petits carres noirs 
						sizeH / 3 * k + sizeH / 60 + size_big_tileH / 3 * h + size_big_tileH / 60,
						size_small_tileW, size_small_tileH, Colors::Black);

					int x = sizeW / 3 * i + sizeW / 60 + size_big_tileW / 3 * (j % 3) + size_big_tileW / 60;
					int y = sizeH / 3 * k + sizeH / 60 + size_big_tileH / 3 * (h % 3) + size_big_tileH / 60;
					switch (board[i*3+j][k*3+h]) {
					case 'r': //petits ronds
						putRound(x, y); 
						break;
					case 'c': //petites croix
						putCross(x, y); 
						break;
					default:
						break;
					}
				}
			}

			int x = sizeW / 3 * i + sizeW / 60;
			int y = sizeH / 3 * k + sizeH / 60;
			switch (win_board[i][k]) {
			case 'r': //gros ronds
				display.putRect(x + size_big_tileW / 60, y + size_big_tileH / 60, size_big_tileW - size_big_tileW / 30, size_big_tileH - size_big_tileH / 30, Colors::Black); 
				putRound(x, y, size_big_tileW, size_big_tileH);
				break;
			case 'c': //grosses croix
				display.putRect(x + size_big_tileW / 60, y + size_big_tileH / 60, size_big_tileW - size_big_tileW / 30, size_big_tileH - size_big_tileH / 30, Colors::Black);
				putCross(x, y, size_big_tileW, size_big_tileH);
				break;
			default:
				continue;
			}
		}
	}

	if (can_display(board, cursor, win_board)) 
		putCursor(sizeW / 3 * (cursor.x / 3) + sizeW / 60 + size_big_tileW / 3 * (cursor.x % 3) + size_big_tileW / 60, 
			      sizeH / 3 * (cursor.y / 3) + sizeH / 60 + size_big_tileH / 3 * (cursor.y % 3) + size_big_tileH / 60); //curseur
	
	display.refreshScreen(); // Rafraîchit la fenêtre

}

void play_hit(matrixC& board, char& turn, item& cursor) //change la case selectionnee
{
	if (board[cursor.x][cursor.y] == 'e') {
		board[cursor.x][cursor.y] = turn;
	}
}

void update_cursor_mode(matrixC board, item& cursor, int& mode, matrixC win_board) //gestion du mode de selection de coup (0/1)
{
	if (win_board[cursor.x % 3][cursor.y % 3] != 'e') {
		mode = 0;
	}
	else {
		bool full = true;
		for (int i = 3 * (cursor.x % 3); i < 3 * (cursor.x % 3) + 3; i++) {
			for (int k = 3 * (cursor.y % 3); k < 3 * (cursor.y % 3) + 3; k++) {
				if (board[i][k] == 'e')
					full = false;
			}
		}
		if (full) {
			mode = 0;
			win_board[cursor.x % 3][cursor.y % 3] = 'f';
		}
		else {
			mode = 1;
		}
	}
}

bool local_win_test(matrixC board, item cursor, char turn, matrixC win_board) //test les victoires des 9 petits morpions
{
	int x = (cursor.x / 3) * 3;
	int y = (cursor.y / 3) * 3;
	if (win_board[x / 3][y / 3] == 'e') {
		for (int i = 0; i < 3; i++) {
			if ((turn == board[x + i][y]) && (turn == board[x + i][y + 1]) && (turn == board[x + i][y + 2])) {
				return true;
			}
			if ((turn == board[x][y + i]) && (turn == board[x + 1][y + i]) && (turn == board[x + 2][y + i])) {
				return true;
			}
		}
		if ((turn == board[x][y]) && (turn == board[x + 1][y + 1]) && (turn == board[x + 2][y + 2])) {
			return true;
		}
		if ((turn == board[x + 2][y]) && (turn == board[x + 1][y + 1]) && (turn == board[x][y + 2])) {
			return true;
		}
	}
	return false;
}

bool global_win_test(matrixC board, char turn) //test la victoire "globale"
{
	for (int i = 0; i < 3; i++) {
		if ((turn == board[i][0]) && (turn == board[i][1]) && (turn == board[i][2])) {
			return true;
		}
		if ((turn == board[0][i]) && (turn == board[1][i]) && (turn == board[2][i])) {
			return true;
		}
	}
	if ((turn == board[0][0]) && (turn == board[1][1]) && (turn == board[2][2])) {
		return true;
	}
	if ((turn == board[2][0]) && (turn == board[1][1]) && (turn == board[0][2])) {
		return true;
	}
	return false;
}

void update_cursor_pos(matrixC& board, item& cursor, int mode, matrixC& winboard) //gestion de l'emplacement de l'espace jouable 
{
	int x = 3 * (cursor.x % 3);
	int y = 3 * (cursor.y % 3);
	while (board[x][y] != 'e' || winboard[x / 3][y / 3] != 'e') {
		x++;
		if (x > 3 * (cursor.x % 3) + 2) {
			x -= 3;
			y++;
		}
		if (y > 3 * (cursor.y % 3) + 2) {
			x = 0;
			y = 0;
			while (board[x][y] != 'e' || winboard[x / 3][y / 3] != 'e') {
				x++;
				if (x > 9) {
					x = 0;
					y++;
				}
			}
		}
	}
	cursor.x = x;
	cursor.y = y;
}

int main(int argc, char** argv)
{
	matrixC board(9, (vectorC(9, 'e'))); //e : neutre
	matrixC win_board(3, (vectorC(3, 'e')));
	char turn = (RNG::generate(2) == 1) ? 'r' : 'c'; // r : rond ; c : croix
	item cursor{ 4,4 };
	char winner = 'e';
	int cursor_mode = 1; // 0 = global, 1 = local
	bool win = false;

	while (!(input.shouldExit()) && !(input.getKeyState(SDL_SCANCODE_ESCAPE)))
	{
		display_screen(board, cursor, turn, cursor_mode, win_board);
		input.flushEvents();		
		
		if (win) {
			SDL_Delay(2000);
			return 0;
		}

		updateCursor(cursor, cursor_mode);
		if (input.getMouseButtonStateOnce(MOUSEBUTTON_LEFT)) {
			if (canPlay(board,cursor,win_board)) {
				play_hit(board, turn, cursor);
				if (local_win_test(board, cursor, turn, win_board)) {
					win_board[cursor.x / 3][cursor.y / 3] = turn;
					if (global_win_test(win_board, turn)) {
						win = true;
					}
				}
				turn = (turn == 'c') ? 'r' : 'c';
				update_cursor_mode(board, cursor, cursor_mode, win_board); 
				update_cursor_pos(board, cursor, cursor_mode, win_board); 
			}
		}
	}
	return (0);
}



