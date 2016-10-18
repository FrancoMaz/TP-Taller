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
#define PI 3.14159265
#define VELMAX 20

using namespace std;

class Jugador {
private:
	pair<int,int> posicion;
	pair<int,int> velocidades;
	string nombre;
	bool saltar;
	double angulo;
	string spriteAEjecutar;
	bool conectado = false;
	string condicionSprite;

public:
	Jugador(pair<int,int> posicionInicial);
	Jugador(string nombre);
	virtual ~Jugador();
	void actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla);
	string getStringJugador();
	string getNombre();
	string getSpriteAEjecutar();
	bool salto();
	void setDesconectado();
	void setConectado();
	bool getConectado();
	string serializarInicio();
};
#endif /* JUGADOR_H_ */