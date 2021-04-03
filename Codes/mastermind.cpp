//#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "SDLDisplay.hpp"
#include "Input.hpp"
#include "Colors.hpp"
#include "RNG.hpp"


#include <windows.h>

bool RNG::_initialized = false;
SDLDisplay	display("minijeu", 770, 700);
void afficher_carre(int x, int y, int tx, int ty, int color);
void dog();


int main(int argc, char** argv)
//int		main()
{
	/* Gestion de la fenêtre */

	/* Gestion des touches */
	Input		input;

	// Les déclarations de variables se font ici


  //setup
	int solution[4] = { 0 };


	for (int i = 0; i < 4; i++) {
		solution[i] = RNG::generate(6);
		//printf("\n\n\n\nsoluce%d : %d\n",i,solution[i]);
	}

	int plateau[4][10] = { 0 };
	int tour = 1;
	int emplacement_x = 0;
	int Bien_Place = 0;
	int Mal_Place = 0;
	int Bien[10] = { 0 };
	int Mal[10] = { 0 };
	printf("\n\n\n\n\n\n\n\n\n");

	while (!(input.shouldExit()) && !(input.getKeyState(SDL_SCANCODE_ESCAPE)))
	{
		display.clearScreen(); // "Nettoie" la fenêtre
  //debut

  //Affichage curseur 

		afficher_carre(emplacement_x * 175, (tour - 1) * 70, 175, 70, 6);

		//affichage plateau 

		for (int y = 0; y < tour; y++) {

			for (int x = 0; x < 4; x++) {

				afficher_carre(x * 175 + 5, y * 70 + 5, 165, 60, plateau[x][y]);


			}

		}

		//gestion du placement du curseur 

		if (input.getKeyStateOnce(SDL_SCANCODE_RIGHT)) {

			emplacement_x++;
			if (emplacement_x > 3) {
				emplacement_x = 0;

			}

		}
		if (input.getKeyStateOnce(SDL_SCANCODE_LEFT)) {
			emplacement_x--;
			if (emplacement_x < 0) {
				emplacement_x = 3;
			}
		}
		//changement de couleur 
		if (input.getKeyStateOnce(SDL_SCANCODE_SPACE)) {
			plateau[emplacement_x][tour - 1]++;
			if (plateau[emplacement_x][tour - 1] == 6) {
				plateau[emplacement_x][tour - 1] = 0;
			}
		}
		//Validation du coup 

		if (input.getKeyStateOnce(SDL_SCANCODE_RETURN)) {
			emplacement_x = 0;
			Bien_Place = 0;
			Mal_Place = 0;

			int reponse[4] = { 0 };
			int coup[4] = { 0 };
			for (int i = 0; i < 4; i++) {
				reponse[i] = solution[i];
				coup[i] = plateau[i][tour - 1];

			}



			//test des bien places
			for (int i = 0; i < 4; i++) {
				if (reponse[i] == coup[i]) {
					Bien_Place++;
					reponse[i] = -50;
					coup[i] = -20;
				}
			}
			//test des mal places
			for (int i = 0; i < 4; i++) {

				for (int k = 0; k < 4; k++) {
					if (coup[i] == reponse[k]) {
						Mal_Place++;
						coup[i] = -20;
						reponse[k] = -50;
					}
				}


			}


			//Affichage console des indices
			printf("Bien place : %d\n", Bien_Place);
			printf("Mal place : %d\n", Mal_Place);
			printf("Couleur non presente : %d\n\n\n", 4 - Bien_Place - Mal_Place);
			//victoire
			if (Bien_Place == 4) {
				printf("Bravo tu as gagne(e) !!!\n");
				printf("Pour ce faire, il t'a fallu %d tours.\n\n\n\n", tour);

				dog();
				return 0;
			}
			else {
				if (tour == 10) { //defaite
					printf("Dommage, après 10 tours tu n'as toujours pas trouvé la solution. Tu as donc perdu.\n\n\n\n");
					display.clearScreen(); // "Nettoie" la fenêtre
					for (int i = 0; i < 4; i++) {
						afficher_carre(i * 175 + 72, 300, 100, 100, solution[i]);
					}
					display.refreshScreen(); // Rafraîchit la fenêtre
					Sleep(5);
					return 0;
				}
			}




			tour++;

		}
		//affichage graphique des indices
		for (int i = 0; i < tour - 1; i++) {
			Bien[tour - 1] = Bien_Place;
			Mal[tour - 1] = Mal_Place;
			afficher_carre(713, 70 * i + 13, 9, 9, 6);
			afficher_carre(748, 70 * i + 13, 9, 9, 6);
			afficher_carre(713, 70 * i + 48, 9, 9, 6);
			afficher_carre(748, 70 * i + 48, 9, 9, 6);
			switch (Mal[i + 1] + Bien[i + 1]) {
			case 4:
				afficher_carre(739, 70 * i + 48, 27, 9, 6);
			case 3:
				afficher_carre(704, 70 * i + 48, 27, 9, 6);
			case 2:
				afficher_carre(739, 70 * i + 13, 27, 9, 6);
			case 1:
				afficher_carre(704, 70 * i + 13, 27, 9, 6);
				break;
			}
			switch (Bien[i + 1]) {
			case 4:
				afficher_carre(748, 70 * i + 39, 9, 27, 6);
			case 3:
				afficher_carre(713, 70 * i + 39, 9, 27, 6);
			case 2:
				afficher_carre(748, 70 * i + 4, 9, 27, 6);
			case 1:
				afficher_carre(713, 70 * i + 4, 9, 27, 6);
				break;
			}

		}



		//fin
		display.refreshScreen(); // Rafraîchit la fenêtre
		input.flushEvents(); // Met à jour les touches pressées / clics de souris
	}
	return (0);
}




void afficher_carre(int x, int y, int tx, int ty, int color)
{

	switch (color) {

	case 0:
		display.putRect(x, y, tx, ty, Colors::Blue);
		break;

	case 1:
		display.putRect(x, y, tx, ty, Colors::Red);
		break;

	case 2:
		display.putRect(x, y, tx, ty, Colors::Green);
		break;

	case 3:
		display.putRect(x, y, tx, ty, Colors::Violet);
		break;

	case 4:
		display.putRect(x, y, tx, ty, Colors::Yellow);
		break;

	case 5:
		display.putRect(x, y, tx, ty, Colors::Orange);
		break;
	case 6:
		display.putRect(x, y, tx, ty, Colors::White);
		break;

	default:
		display.putRect(x, y, tx, ty, Colors::Black);
		break;
	}

}

void dog()
{
	display.clearScreen(); // "Nettoie" la fenêtre  

	int chien[19][21] = { //LE CHIEN
		6,6,7,6,7,7,7,7,6,7,6,6,6,6,6,6,6,6,7,6,6,
		6,7,6,7,6,6,6,6,7,6,7,6,6,6,6,6,6,7,1,7,6,
		6,7,6,6,6,6,6,6,6,6,7,6,6,6,6,6,7,1,1,1,7,
		6,7,6,6,6,6,6,6,6,6,6,7,6,6,6,6,7,1,4,1,7,
		7,6,6,7,6,6,7,6,6,6,6,7,7,6,6,6,7,1,4,1,7,
		7,6,6,6,6,6,6,6,6,6,6,6,6,7,7,6,6,7,6,7,6,
		7,6,6,6,7,7,6,6,6,6,6,6,6,6,6,7,7,7,6,7,6,
		7,6,7,6,7,6,6,7,6,6,6,6,6,6,6,6,6,6,6,7,6,
		7,6,6,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,7,6,
		7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,6,
		7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,6,
		7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,6,
		7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,6,
		7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,6,6,
		6,7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,6,6,
		6,7,6,6,7,7,6,6,7,7,7,7,6,6,7,7,6,6,7,6,6,
		6,7,6,6,7,7,6,6,7,6,6,7,6,6,7,7,6,6,7,6,6,
		6,7,6,7,6,7,6,7,6,6,6,7,6,7,6,7,6,7,6,6,6,
		6,6,7,6,6,6,7,6,6,6,6,6,7,6,6,6,7,6,6,6,6

	};

	for (int y = 0; y < 19; y++) {
		for (int x = 0; x < 21; x++) {
			afficher_carre(7 + 36 * x, 8 + 36 * y, 36, 36, chien[y][x]);
		}
	}





	display.refreshScreen(); // Rafraîchit la fenêtre
	Sleep(5000);

}
