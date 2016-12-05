/*
 * Boss.cpp
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#include "Boss.h"

Boss::Boss() {
	this->vida = 1000;
	this->estaMuerto = false;
	this->visto = false;
	this->sentido = "Normal";
}

Boss::~Boss() {
	// TODO Auto-generated destructor stub
}

void Boss::comportamiento(SDL_Rect camara,  bool tieneQueDisparar)
{

}

string Boss::getStringBoss()
{
	return (to_string(this->posX) + "|" + to_string(this->posY) + "|Boss_" + this->nombre + "|" + this->sentido + "#");
}

string Boss::getNombre()
{
	return "Boss";
}

Enemigo* Boss::liberarSoldado(int xJugador)
{
	return NULL;
}
