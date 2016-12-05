/*
 * AirbusterRiberts.cpp
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#include "AirbusterRiberts.h"

AirbusterRiberts::AirbusterRiberts(string nombre, string x, string y) {
	this->nombre = nombre;
	this->posX = atoi(x.c_str());
	this->posY = atoi(y.c_str());
	this->boxCollider = {posX,posY,326,200};
	this->disparando = false;
	this->tiempoEntreArmas = 7;
	this->liberaSoldados = true;
}

AirbusterRiberts::~AirbusterRiberts() {
	// TODO Auto-generated destructor stub
}

void AirbusterRiberts::comportamiento(SDL_Rect camara,  bool tieneQueDisparar)
{
	if (this->sentido == "Espejado")
	{
		this->posX -= VELOCIDADAIRBUSTER;
		this->boxCollider.x -= VELOCIDADAIRBUSTER;
		if (this->boxCollider.x <= camara.x)
		{
			this->sentido = "Normal";
		}
	}
	else if (this->sentido == "Normal")
	{
		this->posX += VELOCIDADAIRBUSTER;
		this->boxCollider.x += VELOCIDADAIRBUSTER;
		if (this->boxCollider.x + this->boxCollider.w >= camara.x + camara.w)
		{
			this->sentido = "Espejado";
		}
	}

}

Enemigo* AirbusterRiberts::liberarSoldado(int xJugador)
{
	Enemigo* enemigoADevolver = new Enemigo(this->posX + 88, this->posY + 15, 0, 2, 200);
	if (xJugador >= enemigoADevolver->getPosX())
	{
		enemigoADevolver->condicionSprite = "Espejado";
	}
	else
	{
		enemigoADevolver->condicionSprite = "Normal";
	}
	this->disparando = true;
	return enemigoADevolver;
}
