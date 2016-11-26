/*
 * Escenario.h
 *
 *  Created on: 19 nov. 2016
 *      Author: franco
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_
#include "Enemigo.h"
#include "Proyectil.h"
#include "string.h"
#include <SDL2/SDL.h>

using namespace std;

class Escenario {
private:
	vector<pair<int, int>> enemigosPorNivel;
	vector<Enemigo*> enemigosActivos;
	vector<Proyectil*> proyectiles;
public:
	Escenario();
	virtual ~Escenario();
	int idEnemigo;
	void agregarProyectil(Proyectil* proyectil, string nombre, int idProyectil);
	bool verificarColision(SDL_Rect camara, Proyectil* proyectil);
	void despertarEnemigos(SDL_Rect* camara);
	Enemigo* getEnemigoActivo();
	bool enemigoVivo(int idEnemigo);
};

#endif /* ESCENARIO_H_ */
