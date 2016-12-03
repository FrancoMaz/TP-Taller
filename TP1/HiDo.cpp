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
				this->proyectilesADisparar.clear();
				despX = 27;
				despY = 142;
				SDL_Rect box = {this->boxCollider.x + despX,this->boxCollider.y + despY,32,30};
				for (int i = 0; i < 3; i++)
				{
					this->proyectilesADisparar.push_back(new Proyectil(20,0,"HeavyMachineGun",box.x,box.y, this->sentido, 3, box, false,2,"",make_pair(27,142)));
				}
				break;
			}
			case 1:
			{
				this->proyectilesADisparar.clear();
				despX = 27;
				despY = 142;
				SDL_Rect box = {this->boxCollider.x + despX,this->boxCollider.y + despY,32,30};
				for (int i = 0; i < 3; i++)
				{
					this->proyectilesADisparar.push_back(new Proyectil(20,0,"HeavyMachineGun",box.x,box.y, this->sentido, 3, box, false,2,"",make_pair(27,142)));
				}
				break;
			}
			case 2:
			{
				this->proyectilesADisparar.clear();
				despX = 78;
				despY = 115;
				SDL_Rect box = {this->boxCollider.x + despX,this->boxCollider.y + despY,24,50};
				for (int i = 0; i < 2; i++)
				{
					this->proyectilesADisparar.push_back(new Proyectil(20,0,"Boss_hi_do_bomba",box.x,box.y, this->sentido, 4, box, false,2,"",make_pair(78,115)));
				}
				break;
			}
		}
		this->disparando = true;
		this->armaADisparar++;
		if (this->armaADisparar > 2)
		{
			this->armaADisparar = 0;
		}
	}
}


