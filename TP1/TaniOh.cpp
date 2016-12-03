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
	this->disparoDoble = true;
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
				this->proyectilesADisparar.first.clear();
				this->proyectilesADisparar.second = 2;
				for (int i = 0; i < 3; i++)
				{
					SDL_Rect box = {this->boxCollider.x + 27,this->boxCollider.y + 142,32,30};
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"0",box.x,box.y, this->sentido, 6, box, false,2,"",make_pair(27,142)));
					box = {this->boxCollider.x + 300,this->boxCollider.y + 55,32,30};
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"0",box.x,box.y, this->sentido, 0, box, false,2,"",make_pair(300,55)));
				}
				break;
			}
			case 1:
			{
				this->proyectilesADisparar.first.clear();
				this->proyectilesADisparar.second = 2;
				for (int i = 0; i < 3; i++)
				{
					SDL_Rect box = {this->boxCollider.x + 27,this->boxCollider.y + 142,32,30};
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"0",box.x,box.y, this->sentido, 6, box, false,2,"",make_pair(27,142)));
					box = {this->boxCollider.x + 300,this->boxCollider.y + 55,32,30};
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"0",box.x,box.y, this->sentido, 0, box, false,2,"",make_pair(300,55)));
				}
				break;
			}
			case 2:
			{
				this->proyectilesADisparar.first.clear();
				this->proyectilesADisparar.second = 1;
				SDL_Rect box = {this->boxCollider.x + 158,this->boxCollider.y - 412,32,30};
				this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"4",box.x,box.y, this->sentido, 5, box, false,2,"",make_pair(158,-412)));
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

