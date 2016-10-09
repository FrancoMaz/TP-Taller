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
}
Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::actualizarPosicion(pair<int,int> nuevaPosicion)
{
	posicion.first = nuevaPosicion.first;
	posicion.second = nuevaPosicion.second;
}

string Jugador::getStringJugador()
{
	return (nombre + to_string(posicion.first) + "|" + to_string(posicion.second) + "#");
}

string Jugador::getNombre()
{
	return this->nombre;
}
