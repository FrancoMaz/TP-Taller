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
	vector<Enemigo*> enemigos;
	vector<Proyectil*> proyectiles;
public:
	Escenario();
	virtual ~Escenario();
	void agregarProyectil(Proyectil* proyectil, string nombre, int idProyectil);
	bool verificarColision(SDL_Rect camara, Proyectil* proyectil);
};

#endif /* ESCENARIO_H_ */
