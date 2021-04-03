/*
** main.cpp for  in /home/thepatriot/thepatriotsrepo/perso/codingclub
**
** Made by Alexis Bertholom
** Login   bertho_d
** Email   <alexis.bertholom@epitech.eu>
**
** Started on  Tue Jan 27 14:13:06 2015 Alexis Bertholom
// Last update Fri Jan 19 23:44:18 2018 Lucas
*/

//#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "SDLDisplay.hpp"
#include "Input.hpp"
#include "Colors.hpp"
#include "RNG.hpp"
bool RNG::_initialized = false;
using namespace std;
/* Gestion de la fenêtre */
SDLDisplay	display("minijeu", 900, 900);
/* Gestion des touches */
Input		input;

struct obj {
	int x;
	int y;
};

void affiche_screen(vector<vector<char>>& plateau, char tour, char win, obj curseur, bool pushed, int& animation);
void descendre_curseur(vector<vector<char>> const& plateau, obj& curseur, int cote);
void deplacement_curseur(vector<vector<char>> const& plateau, obj& curseur);
char init_tour();
void valider_coup(vector<vector<char>>& plateau, obj& curseur, char& tour, char& win, bool pushed);
void changer_tour(char& tour);
void detection_full(vector<vector<char>>& plateau, obj& curseur, char& tour, char& win);
void detection_win(vector<vector<char>> const& p, char& tour, char& win);
void reset(vector<vector<char>>& plateau, obj& curseur, char& tour, char& win);

int main(int argc, char** argv)
//int		main()
{


	// Les déclarations de variables se font ici
	vector<vector<char>> plateau(7, vector<char>(6, 'e'));
	char tour = 'e';
	char win = 'e';
	obj curseur{ 3,5 };
	bool started = false;
	bool pushed = false;


	while (!(input.shouldExit()) && !(input.getKeyState(SDL_SCANCODE_ESCAPE)))
	{
		input.flushEvents();

		if (!(started)) {
			started = true;
			tour = init_tour();
		}
		int x = -1;
		affiche_screen(plateau, tour, win, curseur, pushed, x);
		deplacement_curseur(plateau, curseur);
		valider_coup(plateau, curseur, tour, win, pushed);
		if (input.getMousePos()[0/*x*/] >= 12 && input.getMousePos()[0] <= 136 && input.getMousePos()[1/*y*/] >= 12 && input.getMousePos()[1] <= 72) {
			pushed = true;
			if (input.getMouseButtonStateOnce(MOUSEBUTTON_LEFT)) {
				reset(plateau, curseur, tour, win);
				pushed = false;
			}
		}
		else pushed = false;
	}
	return (0);
}




void affiche_screen(vector<vector<char>>& plateau, char tour, char win, obj curseur, bool pushed, int& animation)
{
	input.flushEvents();
	if ((input.shouldExit()) || (input.getKeyState(SDL_SCANCODE_ESCAPE)) || (input.getKeyState(SDL_SCANCODE_Q))) {
		animation = -1;
		return;
	}
	display.clearScreen(); // "Nettoie" la fenêtre

	display.putImage("./Puissance4/Plateau.png", 0, 0);

	for (unsigned int i = 0; i < plateau.size(); i++) {//pion
		for (unsigned int k = 0; k < plateau[i].size(); k++) {
			switch (plateau[i][k]) {
			case 'r':
				display.putImage("./Puissance4/jetons/rouge.png", 131 + i * 96, 178 + k * 96);
				break;
			case 'y':
				display.putImage("./Puissance4/jetons/jaune.png", 131 + i * 96, 178 + k * 96);
				break;
			}
		}
	}



	switch (tour) {//tour
	case 'y':
		display.putImage("./Puissance4/Tour/jaune.png", 0, 0);
		display.putImage("./Puissance4/curseur/jaune.png", 128 + curseur.x * 96, 175 + curseur.y * 96);
		break;
	case 'r':
		display.putImage("./Puissance4/Tour/rouge.png", 0, 0);

		display.putImage("./Puissance4/curseur/rouge.png", 128 + curseur.x * 96, 175 + curseur.y * 96);
		break;
	default:
		display.putImage("./Puissance4/Tour/neutre.png", 0, 0);
	}



	switch (win) {
	case 'y':
		display.putImage("./Puissance4/win/jaune.png", 0, 0);
		break;
	case 'r':
		display.putImage("./Puissance4/win/rouge.png", 0, 0);
		break;
	default:
		display.putImage("./Puissance4/win/neutre.png", 0, 0);
	}



	if (pushed)
		display.putImage("./Puissance4/button/pushed.png", 10, 10);
	else
		display.putImage("./Puissance4/button/not_pushed.png", 10, 10);



	if (0 <= animation) {//animation
		switch (tour) {
		case 'r':
			display.putImage("./Puissance4/jetons/rouge.png", 131 + curseur.x * 96, 178 + (curseur.y * 96 - animation));
			break;
		case 'y':
			display.putImage("./Puissance4/jetons/jaune.png", 131 + curseur.x * 96, 178 + (curseur.y * 96 - animation));
			break;
		}
		display.putImage("./Puissance4/cache.png", 0, 0);
		display.refreshScreen();
		animation -= 16;
		affiche_screen(plateau, tour, win, curseur, pushed, animation);
		if (animation <= 0) {
			plateau[curseur.x][curseur.y] = tour;
		}//fin anim
	}

	display.refreshScreen(); // Rafraîchit la fenêtre
}

void descendre_curseur(vector<vector<char>> const& plateau, obj& curseur, int cote)
{
	int y = 5;
	while (plateau[curseur.x][y] != 'e') {
		if (y == 0) {
			if (cote == 1) {
				curseur.x++;
				if (curseur.x == 7) {
					curseur.x = 0;
				}
			}
			else {
				curseur.x--;
				if (curseur.x == -1) {
					curseur.x = 6;
				}
			}
			descendre_curseur(plateau, curseur, cote);
			y = curseur.y;

		}
		else {
			y--;
		}
	}
	curseur.y = y;
}

void deplacement_curseur(vector<vector<char>> const& plateau, obj& curseur)
{
	if (input.getKeyStateOnce(SDL_SCANCODE_RIGHT)) {
		curseur.x++;
		if (curseur.x > 6) {
			curseur.x = 0;
		}
		descendre_curseur(plateau, curseur, 1);
	}
	if (input.getKeyStateOnce(SDL_SCANCODE_LEFT)) {
		curseur.x--;
		if (curseur.x < 0) {
			curseur.x = 6;
		}
		descendre_curseur(plateau, curseur, 2);
	}
}

char init_tour()
{
	int random = RNG::generate(2);
	switch (random) {
	case 0:
		return 'y';
		break;
	case 1:
		return 'r';
		break;
	}
	return 'e';
}

void valider_coup(vector<vector<char>>& plateau, obj& curseur, char& tour, char& win, bool pushed)
{
	if ((input.getKeyStateOnce(SDL_SCANCODE_RETURN) || input.getKeyStateOnce(SDL_SCANCODE_SPACE)) && tour != 'e') {
		int animation = curseur.y * 96;
		affiche_screen(plateau, tour, win, curseur, pushed, animation);
		detection_win(plateau, tour, win);
		changer_tour(tour);
		detection_full(plateau, curseur, tour, win);
		descendre_curseur(plateau, curseur, 1);
	}
}

void changer_tour(char& tour)
{
	if (tour == 'y') {
		tour = 'r';
	}
	else if (tour == 'r') {
		tour = 'y';
	}
}

void detection_full(vector<vector<char>>& plateau, obj& curseur, char& tour, char& win)
{
	bool full = true;
	for (unsigned int i = 0; i < plateau.size(); i++) {
		for (unsigned int k = 0; k < plateau[i].size(); k++) {
			if (plateau[i][k] == 'e') {
				full = false;
			}
		}
	}
	if (full) {
		reset(plateau, curseur, tour, win);
	}
}


void detection_win(vector<vector<char>> const& p, char& tour, char& win)
{
	bool winner = false;
	for (unsigned int i = 0; i < p.size(); i++) {
		for (unsigned int k = 0; k < p[i].size() - 3; k++) {
			if ((p[i][k] != 'e') && (p[i][k] == p[i][k + 1]) && (p[i][k] == p[i][k + 2]) && (p[i][k] == p[i][k + 3])) {
				winner = true;
			}
		}
	}
	for (unsigned int i = 0; i < p.size() - 3; i++) {
		for (unsigned int k = 0; k < p[i].size(); k++) {
			if ((p[i][k] != 'e') && (p[i][k] == p[i + 1][k]) && (p[i][k] == p[i + 2][k]) && (p[i][k] == p[i + 3][k])) {
				winner = true;
			}
		}
	}
	for (unsigned int i = 0; i < p.size() - 3; i++) {
		for (unsigned int k = 0; k < p[i].size() - 3; k++) {
			if ((p[i][k] != 'e') && (p[i][k] == p[i + 1][k + 1]) && (p[i][k] == p[i + 2][k + 2]) && (p[i][k] == p[i + 3][k + 3])) {
				winner = true;
			}
		}
	}

	for (unsigned int i = p.size() - 1; i > 2; i--) {
		for (unsigned int k = 0; k < p[i].size() - 3; k++) {
			if ((p[i][k] != 'e') && (p[i][k] == p[i - 1][k + 1]) && (p[i][k] == p[i - 2][k + 2]) && (p[i][k] == p[i - 3][k + 3])) {
				winner = true;
			}
		}
	}
	if (winner) {
		win = tour;
		tour = 'e';
	}
}

void reset(vector<vector<char>>& plateau, obj& curseur, char& tour, char& win)
{
	win = 'e';
	curseur.x = 3;
	curseur.y = 5;
	tour = init_tour();
	for (unsigned int i = 0; i < plateau.size(); i++) {
		for (unsigned int k = 0; k < plateau[i].size(); k++) {
			plateau[i][k] = 'e';
		}
	}
}
