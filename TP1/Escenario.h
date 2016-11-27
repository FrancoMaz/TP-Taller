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
#include "XmlParser.h"

using namespace std;

class Escenario {
private:
	vector<pair<int, int>> enemigosPorNivel;
	vector<Enemigo*> enemigosActivos;
	vector<Proyectil*> proyectiles;
public:
	Escenario(string rutaXml);
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
	vector<pair<string,string>> plataformas;
	uint64_t rdtsc();
	bool levelClear;
};

#endif /* ESCENARIO_H_ */
