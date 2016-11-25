/*
 * Proyectil.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: franco
 */

#include "Proyectil.h"
#include <iostream>
using namespace std;

Proyectil::Proyectil(int danioEnPorcentaje, int puntosPorDisparo, string spriteBala,SDL_Rect boxCollider, string sentido, int angulo) {
	this->danioEnPorcentaje = danioEnPorcentaje;
	this->puntosPorDisparo = puntosPorDisparo;
	this->spriteBala = spriteBala;
	this->posicion.first = -1;
	this->sentido=sentido;
	if (sentido == "Normal")
	{
		this->posicion.first = boxCollider.x+boxCollider.w+10;
	}
	else
	{
		this->posicion.first = boxCollider.x-10;
	}
	this->posicion.second = boxCollider.y+20;
	this->angulo = angulo;
}

Proyectil::~Proyectil() {
	// TODO Auto-generated destructor stub
}

pthread_t Proyectil::getThreadDisparo()
{
	return this->threadDisparo;
}

void Proyectil::setThreadDisparo(pthread_t threadDisparo)
{
	this->threadDisparo = threadDisparo;
}

void Proyectil::fueDisparadoPor(string nombreJugador)
{
	this->jugadorQueLoDisparo = nombreJugador;
}

void Proyectil::mover()
{
	if (this->sentido == "Normal")
	{
		switch (this->angulo)
		{
			case 0:
			{
				this->posicion.first += VELOCIDAD;
				break;
			}
			case 1:
			{
				this->posicion.second -= VELOCIDAD;
				break;
			}
			case 2:
			{
				this->posicion.first += VELOCIDAD;
				this->posicion.second -= VELOCIDAD;
				break;
			}
		}
	}
	else
	{
		switch (this->angulo)
		{
			case 0:
			{
				this->posicion.first -= VELOCIDAD;
				break;
			}
			case 1:
			{
				this->posicion.second -= VELOCIDAD;
				break;
			}
			case 2:
			{
				this->posicion.first -= VELOCIDAD;
				this->posicion.second -= VELOCIDAD;
				break;
			}
		}
	}
}

string Proyectil::getStringProyectil()
{
	return (to_string(this->posicion.first) + "|" + to_string(this->posicion.second) + "|" + this->spriteBala + "|" + to_string(this->id) + "|" + this->sentido + "|" + to_string(this->angulo) + "#");
}

int Proyectil::getDanio() {
	return this->danioEnPorcentaje;
}
