/*
 * Proyectil.h
 *
 *  Created on: 19 nov. 2016
 *      Author: franco
 */

#ifndef PROYECTIL_H_
#define PROYECTIL_H_

#include <vector>
#include "string"
#include "string.h"
#include <pthread.h>
#include <SDL2/SDL.h>

using namespace std;
#define VELOCIDAD 5;

class Proyectil {
private:
	int danioEnPorcentaje;
	int puntosPorDisparo;
	string spriteBala;
	pthread_t threadDisparo;
	string sentido;
public:
	Proyectil(int danioEnPorcentaje, int puntosPorDisparo, string spriteBala,SDL_Rect boxCollider,string sentido);
	virtual ~Proyectil();
	pthread_t getThreadDisparo();
	void setThreadDisparo(pthread_t threadDisparo);
	void fueDisparadoPor(string nombre);
	string jugadorQueLoDisparo;
	int id;
	pair<int,int> posicion;
	void mover();
	string getStringProyectil();
};

#endif /* PROYECTIL_H_ */
