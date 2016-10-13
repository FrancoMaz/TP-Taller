/*
 * Jugador.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#include "Jugador.h"

Jugador::Jugador(pair<int,int> posicionInicial) {
	posicion.first = posicionInicial.first;
	posicion.second = posicionInicial.second;
}

Jugador::Jugador(string nombre) {
	this->nombre = nombre;
	posicion.first = 20;
	posicion.second = 20;
}
Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::actualizarPosicion(SDL_Keycode teclaPresionada)
{
	if (teclaPresionada == SDLK_RIGHT)
		{
			posicion.first += 1;
		}
		if (teclaPresionada == SDLK_LEFT)
		{
			posicion.first -= 1;
		}
		if (teclaPresionada == SDLK_UP)
		{
			posicion.second -= 1;
		}
}

string Jugador::getStringJugador()
{
	return (nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "#");
}

string Jugador::getNombre()
{
	return this->nombre;
}
