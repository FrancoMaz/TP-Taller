/*
 * Jugador.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#include "Jugador.h"

#define PI 3.14159265
#define VELMAX 20

Jugador::Jugador(pair<int,int> posicionInicial) {
	posicion.first = posicionInicial.first;
	posicion.second = posicionInicial.second;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	angulo = 0;
}

Jugador::Jugador(string nombre) {
	this->nombre = nombre;
	posicion.first = 20;
	posicion.second = 415;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	angulo = 0;
}
Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::actualizarPosicion(SDL_Keycode tecla, bool sePresionoTecla)
{
	velocidades.first = 0;
	if (tecla == SDLK_RIGHT && sePresionoTecla)
	{
		velocidades.first += velocidad;
		spriteAEjecutar = "Jugador Corriendo Derecha";
	}
	if (tecla == SDLK_LEFT && sePresionoTecla)
	{
		velocidades.first -= velocidad;
		spriteAEjecutar = "Jugador Corriendo Izquierda";
	}
	if (tecla == SDLK_UP && sePresionoTecla)
	{
		if(angulo == 0)
		{
			saltar = true;
			spriteAEjecutar = "Jugador Saltando";
		}
	}
	if(tecla == SDLK_RIGHT && !sePresionoTecla)
	{
		velocidades.first -= velocidad;
		spriteAEjecutar = "Jugador Derecha";
	}
	if(tecla == SDLK_LEFT && !sePresionoTecla)
	{
		velocidades.first += velocidad;
		spriteAEjecutar = "Jugador Izquierda";
	}

	if(saltar)
	{
		velocidades.second = -7*cos(angulo);
		angulo += PI/50;
		if (angulo > (PI + (PI/50))){
			angulo = 0;
			saltar = false;
		}
	} else{
		velocidades.second = 0;
	}

	posicion.first += velocidades.first;
	posicion.second += velocidades.second;
}

string Jugador::getStringJugador()
{
	return (nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + spriteAEjecutar + "#");
}

string Jugador::getNombre()
{
	return this->nombre;
}

string Jugador::getSpriteAEjecutar()
{
	return this->spriteAEjecutar;
}
