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
#define VELOCIDAD 25;

class Proyectil {
private:
	int danioEnPorcentaje;
	int puntosPorDisparo;
	string spriteBala;
	pthread_t threadDisparo;
	string sentido;
	int angulo;
	bool aumentoAncho;
public:
	Proyectil(int danioEnPorcentaje, int puntosPorDisparo, string spriteBala,int posicionX,int posicionY,string sentido,int angulo,SDL_Rect boxCollider,bool cortoAlcance,int disparadoPor, string nombreJugador, pair<int,int> desp);
	virtual ~Proyectil();
	pthread_t getThreadDisparo();
	void setThreadDisparo(pthread_t threadDisparo);
	void fueDisparadoPor(string nombre);
	string jugadorQueLoDisparo;
	string jugadorQueRecibioDisparo;
	int vidaDelJugadorImpactado;
	int id;
	pair<int,int> posicion;
	void mover();
	string getStringProyectil();
	int getDanio();
	SDL_Rect getBoxCollider();
	bool cortoAlcance;
	int disparadoPor;
	int getPuntosPorImpacto();
	bool colisionPersonaje;
	pair<int,int> desp;
	SDL_Rect boxCollider;
};

#endif /* PROYECTIL_H_ */
