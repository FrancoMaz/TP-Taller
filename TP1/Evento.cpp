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
	this->sePresionoTecla = false;
}

Evento::~Evento() {
	// TODO Auto-generated destructor stub
}

void Evento::deserializar(string evento)
{
	if (evento == "Tecla Derecha")
	{
		this->tecla = SDLK_RIGHT;
	}
	if (evento == "Tecla Izquierda")
	{
		this->tecla = SDLK_LEFT;
	}
	if (evento == "Tecla Arriba")
	{
		this->tecla = SDLK_UP;
	}
	if (evento == "Tecla Abajo")
	{
		this->tecla = SDLK_DOWN;
	}
	if (evento == "Tecla Espacio")
	{
		this->tecla = SDLK_SPACE;
	}
	if (evento == "Tecla R")
	{
		this->tecla = SDLK_r;
	}
	if (evento == "Soltar Tecla Derecha")
	{
		this->tecla = SDLK_RIGHT;
	}
	if (evento == "Soltar Tecla Izquierda")
	{
		this->tecla = SDLK_LEFT;
	}
	if (evento == "Soltar Tecla Abajo")
	{
		this->tecla = SDLK_DOWN;
	}
	if (evento == "Soltar Tecla Espacio")
	{
		this->tecla = SDLK_SPACE;
	}
}

string Evento::serializar()
{
	return (this->remitente + '|' + this->eventoSerializado + '#');
}

SDL_Keycode Evento::getTecla()
{
	return this->tecla;
}

string Evento::getRemitente()
{
	return this->remitente;
}

string Evento::getEventoSerializado()
{
	return this->eventoSerializado;
}

bool Evento::getSePresionoTecla()
{
	return this->sePresionoTecla;
}

void Evento::setSePresionoTecla()
{
	if (eventoSerializado == "Tecla Derecha")
	{
		this->sePresionoTecla = true;
	}
	if (eventoSerializado == "Tecla Izquierda")
	{
		this->sePresionoTecla = true;
	}
	if (eventoSerializado == "Tecla Arriba")
	{
		this->sePresionoTecla = true;
	}
	if (eventoSerializado == "Tecla Abajo")
	{
		this->sePresionoTecla = true;
	}
	if (eventoSerializado == "Tecla Espacio")
	{
		this->sePresionoTecla = true;
	}
	if (eventoSerializado == "Soltar Tecla Derecha")
	{
		this->sePresionoTecla = false;
	}
	if (eventoSerializado == "Soltar Tecla Izquierda")
	{
		this->sePresionoTecla = false;
	}
	if (eventoSerializado == "Soltar Tecla Abajo")
	{
		this->sePresionoTecla = false;
	}
	if (eventoSerializado == "Soltar Tecla Espacio")
	{
		this->sePresionoTecla = false;
	}
}
