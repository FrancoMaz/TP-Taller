/*
 * HiDo.cpp
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#include "HiDo.h"

HiDo::HiDo(string nombre, string x, string y) {
	this->nombre = nombre;
	this->posX = atoi(x.c_str());
	this->posY = atoi(y.c_str());
	this->boxCollider = {posX,posY,200,172};
	this->velocidades.first = 0;
	this->velocidades.second = 0;
}

HiDo::~HiDo() {
	// TODO Auto-generated destructor stub
}

void HiDo::comportamiento(SDL_Rect camara)
{
	if (this->sentido == "Normal")
	{
		this->posX -= VELOCIDADHIDO;
		this->boxCollider.x -= VELOCIDADHIDO;
		if (this->boxCollider.x <= camara.x)
		{
			this->sentido = "Espejado";
		}
	}
	else if (this->sentido == "Espejado")
	{
		this->posX += VELOCIDADHIDO;
		this->boxCollider.x += VELOCIDADHIDO;
		if (this->boxCollider.x + this->boxCollider.w >= camara.x + camara.w)
		{
			this->sentido = "Normal";
		}
	}
}
