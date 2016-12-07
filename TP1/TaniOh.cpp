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
	this->disparando = false;
	this->tiempoEntreArmas = 3;
	this->liberaSoldados = false;
	this->sentidoMovimientoTaniOh = "Izquierda";
}

TaniOh::~TaniOh() {
	// TODO Auto-generated destructor stub
}

void TaniOh::comportamiento(SDL_Rect camara, bool tieneQueDisparar)
{
	if (this->sentidoMovimientoTaniOh == "Izquierda")
	{
		this->posX -= VELOCIDADTANIOH;
		this->boxCollider.x -= VELOCIDADTANIOH;
		if (this->boxCollider.x <= camara.x)
		{
			this->sentidoMovimientoTaniOh = "Derecha";
		}
	}
	else if (this->sentidoMovimientoTaniOh == "Derecha")
	{
		this->posX += VELOCIDADTANIOH;
		this->boxCollider.x += VELOCIDADTANIOH;
		if (this->boxCollider.x + this->boxCollider.w >= camara.x + camara.w)
		{
			this->sentidoMovimientoTaniOh = "Izquierda";
		}
	}

	if (tieneQueDisparar && !disparando)
	{
		switch (armaADisparar)
		{
			case 0:
			{
				this->proyectilesADisparar.first.clear();
				this->proyectilesADisparar.second = 1;
				for (int i = 0; i < 1; i++)
				{
					SDL_Rect proyectilIzquierda = {this->boxCollider.x + 25,this->boxCollider.y + 70,78,13};
					SDL_Rect proyectilDerecha = {this->boxCollider.x + 288,this->boxCollider.y + 63,78,13};
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"5",proyectilIzquierda.x,proyectilIzquierda.y, "Normal", 6, proyectilIzquierda, false,2,"",make_pair(25,70)));
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"5",proyectilDerecha.x,proyectilDerecha.y, "Espejado", 6, proyectilDerecha, false,2,"",make_pair(288,70)));
				}
				break;
			}
			case 1:
			{
				this->proyectilesADisparar.first.clear();
				this->proyectilesADisparar.second = 1;
				for (int i = 0; i < 1; i++)
				{
					SDL_Rect proyectilIzquierda = {this->boxCollider.x + 25,this->boxCollider.y + 70,78,13};
					SDL_Rect proyectilDerecha = {this->boxCollider.x + 288,this->boxCollider.y + 70,78,13};
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"5",proyectilIzquierda.x,proyectilIzquierda.y, "Normal", 6, proyectilIzquierda, false,2,"",make_pair(25,70)));
					this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"5",proyectilDerecha.x,proyectilDerecha.y, "Espejado", 6, proyectilDerecha, false,2,"",make_pair(288,70)));
				}
				break;
			}
			case 2:
			{
				this->proyectilesADisparar.first.clear();
				this->proyectilesADisparar.second = 1;
				string sentidoTanque;
				SDL_Rect box = {this->boxCollider.x + 158,this->boxCollider.y - 390,70,425};
				if(this->sentidoMovimientoTaniOh == "Izquierda"){
					sentidoTanque = "Normal";
				} else {
					sentidoTanque = "Espejado";
				}
				this->proyectilesADisparar.first.push_back(new Proyectil(20,0,"4",box.x,box.y, sentidoTanque, 5, box, true,2,"",make_pair(158,-412)));
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
