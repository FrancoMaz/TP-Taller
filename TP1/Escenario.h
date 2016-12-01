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
#include <unistd.h>

using namespace std;

class Escenario {
private:
	vector<Proyectil*> proyectiles;
	//XmlParser* parserNivel;
public:
	Escenario(string rutaXml);
	virtual ~Escenario();
	int idEnemigo;
	void agregarProyectil(Proyectil* proyectil, string nombre);
	bool verificarColision(SDL_Rect camara, Proyectil* proyectil, bool disparando);
	void despertarEnemigos(SDL_Rect* camara);
	bool despertarBoss(SDL_Rect camara);
	Enemigo* getEnemigoActivo(int posicion);
	bool verificarColisionConItem(Jugador* jugador);
	bool colisionaronObjetos(SDL_Rect boxCollider1, SDL_Rect boxCollider2);
	vector<Item*> items;
	vector<pair<string,string>> plataformas;
	uint64_t rdtsc();
	bool levelClear;
	bool verificarColisionConEnemigo(Proyectil* proyectil);
	void ordenarEnemigos();
	vector<Enemigo*> getEnemigosActivos();
	bool enemigoPerdido(int id, SDL_Rect* camara);
	void eliminarEnemigoActivo(int id);
	pthread_mutex_t mutexEnemigosActivos = PTHREAD_MUTEX_INITIALIZER;
	vector<Boss*> boss;
	vector<string> capas;
	void agregarItemBonus(Item* item);
	int bonusColisionado;
	bool avanzoDeNivel;
	vector<pair<int, int>> enemigosPorNivel;
	vector<Enemigo*> enemigosActivos;
	void vaciarVectores();
	void inicializarDatosNivel();
};

#endif /* ESCENARIO_H_ */
