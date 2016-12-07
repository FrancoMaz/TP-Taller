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
	this->boxCollider = {posX+41,posY+65,90,100};
	this->velocidades.first = 0;
	this->velocidades.second = 0;
	this->disparando = false;
	this->armas = {0,1,2};
	this->armaADisparar = 0;
	this->proyectilesADisparar.second = 1;
	this->tiempoEntreArmas = 3;
	this->liberaSoldados = false;
}

HiDo::~HiDo() {
	// TODO Auto-generated destructor stub
}

void HiDo::comportamiento(SDL_Rect camara,  bool tieneQueDisparar)
{
	if (this->sentido == "Normal"){
		boxCollider = {posX+41,posY+65,100,100};
	} else {
		boxCollider = {posX+62,posY+65,100,100};
	}

	if (this->sentido == "Normal")
	{
		this->posX -= VELOCIDADHIDO;
		this->boxCollider.x -= VELOCIDADHIDO;
		if (this->posX <= camara.x)
		{
			this->sentido = "Espejado";
		}
	}
	else if (this->sentido == "Espejado")
	{
		this->posX += VELOCIDADHIDO;
		this->boxCollider.x += VELOCIDADHIDO;
		if (this->posX + 200 >= camara.x + camara.w)
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
				SDL_Rect box;
				int desplazamientoX;
				int desplazamientoY = 142;
				this->proyectilesADisparar.first.clear();
				if(this->sentido == "Normal"){
					desplazamientoX = 27;
					box = {this->posX + desplazamientoX,this->posY + desplazamientoY,32,30};
				} else {
					desplazamientoX = 120;
					box = {this->posX + desplazamientoX,this->posY + desplazamientoY,32,30};
				}

				for (int i = 0; i < 1; i++)
				{
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"1",box.x,box.y, this->sentido, 3, box, false,2,"",make_pair(desplazamientoX,desplazamientoY)));
				}
				break;
			}
			case 1:
			{
				SDL_Rect box;
				int desplazamientoX;
				int desplazamientoY = 142;
				this->proyectilesADisparar.first.clear();
				if(this->sentido == "Normal"){
					desplazamientoX = 27;
					box = {this->posX + desplazamientoX,this->posY + desplazamientoY,32,30};
				} else {
					desplazamientoX = 120;
					box = {this->posX + desplazamientoX,this->posY + desplazamientoY,32,30};
				}

				for (int i = 0; i < 1; i++)
				{
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"1",box.x,box.y, this->sentido, 3, box, false,2,"",make_pair(desplazamientoX,desplazamientoY)));
				}
				break;
			}
			case 2:
			{
				this->proyectilesADisparar.first.clear();
				SDL_Rect box = {this->posX + 78,this->posY + 115,24,50};
				for (int i = 0; i < 2; i++)
				{
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"3",box.x,box.y, this->sentido, 4, box, false,2,"",make_pair(78,115)));
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


