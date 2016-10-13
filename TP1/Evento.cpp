/*
 * Evento.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#include "Evento.h"

Evento::Evento(string remitente, string eventoSerializado) {
	this->remitente = remitente;
	this->deserializar(eventoSerializado);
	this->eventoSerializado = eventoSerializado;
}

Evento::~Evento() {
	// TODO Auto-generated destructor stub
}

void Evento::deserializar(string evento)
{
	if (evento == "Tecla Derecha")
	{
		this->teclaPresionada = SDLK_RIGHT;
	}
	if (evento == "Tecla Izquierda")
	{
		this->teclaPresionada = SDLK_LEFT;
	}
	if (evento == "Tecla Arriba")
	{
		this->teclaPresionada = SDLK_UP;
	}
	if (evento == "Tecla R")
	{
		this->teclaPresionada = SDLK_r;
	}
}

string Evento::serializar()
{
	return (this->remitente + '|' + this->eventoSerializado + '#');
}

SDL_Keycode Evento::getTeclaPresionada()
{
	return this->teclaPresionada;
}

string Evento::getRemitente()
{
	return this->remitente;
}

string Evento::getEventoSerializado()
{
	return this->eventoSerializado;
}
