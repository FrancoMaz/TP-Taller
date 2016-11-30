/*
 * TaniOh.cpp
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#include "TaniOh.h"

TaniOh::TaniOh(string nombre, string x, string y) {
	this->nombre = nombre;
	this->posX = atoi(x.c_str());
	this->posY = atoi(y.c_str());
	this->boxCollider = {posX,posY,367,182};
	this->armaADisparar = 0;
}

TaniOh::~TaniOh() {
	// TODO Auto-generated destructor stub
}

void TaniOh::comportamiento(SDL_Rect camara, bool tieneQueDisparar)
{
	if (this->sentido == "Normal")
	{
		this->posX -= VELOCIDADTANIOH;
		this->boxCollider.x -= VELOCIDADTANIOH;
		if (this->boxCollider.x <= camara.x)
		{
			this->sentido = "Espejado";
		}
	}
	else if (this->sentido == "Espejado")
	{
		this->posX += VELOCIDADTANIOH;
		this->boxCollider.x += VELOCIDADTANIOH;
		if (this->boxCollider.x + this->boxCollider.w >= camara.x + camara.w)
		{
			this->sentido = "Normal";
		}
	}
	if (tieneQueDisparar && !disparando)
	{
		switch (armaADisparar)
		{
			case 0:
			{
				SDL_Rect box = {this->boxCollider.x + 20,this->boxCollider.y + 64,32,30};
				this->proyectilADisparar = new Proyectil(20,0,"HeavyMachineGun",box.x,box.y, this->sentido, 0, box, false,2,"");
				break;
			}
			case 1:
			{
				SDL_Rect box = {this->boxCollider.x + 158,this->boxCollider.y - 412,32,30};
				this->proyectilADisparar = new Proyectil(20,0,"Boss_tani_oh_disparo_laser",box.x,box.y, this->sentido, 5, box, false,2,"");
				break;
			}
		}
		this->disparando = true;
		this->armaADisparar++;
		if (this->armaADisparar > 1)
		{
			this->armaADisparar = 0;
		}
	}
}

