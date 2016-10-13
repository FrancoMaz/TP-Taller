/*
 * Jugador.h
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#ifndef JUGADOR_H_
#define JUGADOR_H_

#include <utility>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "VentanaSDL.h"
#include "TexturaSDL.h"

#define velocidad 5

using namespace std;

class Jugador {
private:
	pair<int,int> posicion;
	pair<int,int> velocidades;
	string nombre;
	bool saltar;
	double angulo;
	string spriteAEjecutar;

public:
	Jugador(pair<int,int> posicionInicial);
	Jugador(string nombre);
	virtual ~Jugador();
	void actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla);
	string getStringJugador();
	string getNombre();
	string getSpriteAEjecutar();

};

#endif /* JUGADOR_H_ */
