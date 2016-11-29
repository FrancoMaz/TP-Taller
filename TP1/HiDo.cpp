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
	this->disparando = false;
	this->armas = {0,1,2};
	this->armaADisparar = 0;
}

HiDo::~HiDo() {
	// TODO Auto-generated destructor stub
}

void HiDo::comportamiento(SDL_Rect camara,  bool tieneQueDisparar)
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
	if (tieneQueDisparar && !disparando)
	{
		switch (armaADisparar)
		{
			case 0:
			{
				SDL_Rect box = {((this->boxCollider.x + this->boxCollider.w)/2)-25,((this->boxCollider.y + this->boxCollider.h)/2)+106,32,30};
				this->proyectilADisparar = new Proyectil(20,0,"Boss_hi_do_disparo",((this->boxCollider.x + this->boxCollider.w)/2)-25,((this->boxCollider.y + this->boxCollider.h)/2)+106, this->sentido, 3, box, false,2,"");
				break;
			}
			case 1:
			{
				SDL_Rect box = {((this->boxCollider.x + this->boxCollider.w)/2)-25,((this->boxCollider.y + this->boxCollider.h)/2)+106,32,30};
				this->proyectilADisparar = new Proyectil(20,0,"Boss_hi_do_disparo",((this->boxCollider.x + this->boxCollider.w)/2)-25,((this->boxCollider.y + this->boxCollider.h)/2)+106, this->sentido, 3, box, false,2,"");
				break;
			}
			case 2:
			{
				SDL_Rect box = {((this->boxCollider.x + this->boxCollider.w)/2)-25,(this->boxCollider.y+this->boxCollider.h)+106,24,50};
				this->proyectilADisparar = new Proyectil(20,0,"Boss_hi_do_bomba",((this->boxCollider.x + this->boxCollider.w)/2)-25,(this->boxCollider.y+this->boxCollider.h)+106, this->sentido, 4, box, false,2,"");
				break;
			}
		}
		cout << "Ya disparo" << endl;
		this->disparando = true;
		this->armaADisparar++;
		if (this->armaADisparar > 2)
		{
			this->armaADisparar = 0;
		}
	}
}


