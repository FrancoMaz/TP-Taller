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
#include "Item.h"
#include "string.h"
#include "Jugador.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

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
	bool verificarColisionConItem(Jugador* jugador);
	bool colisionaronObjetos(SDL_Rect boxCollider1, SDL_Rect boxCollider2);
	vector<Item*> itemArmas;
	uint64_t rdtsc();
};

#endif /* ESCENARIO_H_ */
