/*
 * Proyectil.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: franco
 */

#include "Proyectil.h"
#include <iostream>
using namespace std;

Proyectil::Proyectil(int danioEnPorcentaje, int puntosPorDisparo, string spriteBala,int posicionX, int posicionY, string sentido, int angulo, SDL_Rect boxCollider, bool cortoAlcance, int disparadoPor) {
	this->danioEnPorcentaje = danioEnPorcentaje;
	this->puntosPorDisparo = puntosPorDisparo;
	this->spriteBala = spriteBala;
	this->boxCollider = boxCollider;
	if (disparadoPor == 2) {
		this->posicion.first = posicionX+25;
		this->posicion.second = posicionY-106;
		this->boxCollider.x += 25;
		this->boxCollider.y -= 106;
	} else {
		this->posicion.first = posicionX;
		this->posicion.second = posicionY;
	}
	this->sentido = sentido;
	this->angulo = angulo;
	this->cortoAlcance = cortoAlcance;
	this->aumentoAncho = true;
	this->disparadoPor = disparadoPor;
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
	if (!cortoAlcance)
	{
		if (this->sentido == "Normal")
		{
			switch (this->angulo)
			{
				case 0:
				{
					this->posicion.first += VELOCIDAD;
					this->boxCollider.x += VELOCIDAD;
					break;
				}
				case 1:
				{
					this->posicion.second -= VELOCIDAD;
					this->boxCollider.y -= VELOCIDAD;
					break;
				}
				case 2:
				{
					int velocidad_Y = VELOCIDAD;
					this->posicion.first += VELOCIDAD;
					this->boxCollider.x += VELOCIDAD;
					this->posicion.second -= (velocidad_Y/2);
					this->boxCollider.y -= (velocidad_Y/2);
					break;
				}
				case 3:
				{
					int velocidad_Y = VELOCIDAD;
					this->posicion.first -= VELOCIDAD;
					this->boxCollider.x -= VELOCIDAD;
					this->posicion.second += VELOCIDAD;
					this->boxCollider.y += VELOCIDAD;
					break;
				}
				case 4:
				{
					int velocidad_Y = VELOCIDAD;
					this->posicion.second += VELOCIDAD;
					this->boxCollider.y += VELOCIDAD;
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
					this->boxCollider.x -= VELOCIDAD;
					break;
				}
				case 1:
				{
					this->posicion.second -= VELOCIDAD;
					this->boxCollider.y -= VELOCIDAD;
					break;
				}
				case 2:
				{
					int velocidad_Y = VELOCIDAD;
					this->posicion.first -= VELOCIDAD;
					this->boxCollider.x -= VELOCIDAD;
					this->posicion.second -= (velocidad_Y/2);
					this->boxCollider.y -= (velocidad_Y/2);
					break;
				}
				case 3:
				{
					int velocidad_Y = VELOCIDAD;
					this->posicion.first += VELOCIDAD;
					this->boxCollider.x += VELOCIDAD;
					this->posicion.second += VELOCIDAD;
					this->boxCollider.y += VELOCIDAD;
					break;
				}
				case 4:
				{
					int velocidad_Y = VELOCIDAD;
					this->posicion.second += VELOCIDAD;
					this->boxCollider.y += VELOCIDAD;
					break;
				}
			}
		}
	}
	else
	{
		if (aumentoAncho || this->boxCollider.w == 91)
		{
			this->boxCollider.w += 91;
			aumentoAncho = true;
		}
		else
		{
			if (!aumentoAncho || this->boxCollider.w == 364)
			{
				this->boxCollider.w -= 91;
				aumentoAncho = false;
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

SDL_Rect Proyectil::getBoxCollider() {
	return this->boxCollider;
}
