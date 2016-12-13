/*
 * Capa.cpp
 *
 *  Created on: 24 oct. 2016
 *      Author: franco
 */

#include "Capa.h"

Capa::Capa(ImagenDto* imagen, SDL_Rect rectangulo, TexturaSDL* textura) {
	this->imagen = imagen;
	this->rectangulo = rectangulo;
	this->textura = textura;
	this->vel = 0;
}

Capa::~Capa() {
	delete this->textura;
}

void Capa::paralajeInfinito(int anchoVentana, int indice)
{
	if ((this->rectangulo.x + anchoVentana) > atoi(this->imagen->getAncho().c_str()))
	{
		int posicionAuxiliar = 0;
		this->rectangulo.w = atoi(this->imagen->getAncho().c_str()) - this->rectangulo.x;
		this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);
		posicionAuxiliar = this->rectangulo.x;

		this->rectangulo.w = anchoVentana - (atoi(this->imagen->getAncho().c_str()) - this->rectangulo.x);
		this->rectangulo.x = 0;
		this->textura->aplicarPosicionDePorcion(anchoVentana - this->rectangulo.w,0,&this->rectangulo,0,SDL_FLIP_NONE);
		this->rectangulo.x = posicionAuxiliar;

		this->resetearDatos(anchoVentana, indice);
	}
	else
	{
		if (this->rectangulo.x == 0)
		{
			this->rectangulo.w = anchoVentana;
		}
		this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);
	}
}

void Capa::resetearDatos(int anchoVentana, int indice)
{
	if (indice == 0)
	{
		if (this->rectangulo.w > anchoVentana){
			this->rectangulo.w = anchoVentana;
			this->rectangulo.x = 0;
		}
	}
	else
	{
		if (this->rectangulo.x > atoi(this->imagen->getAncho().c_str()))
		{
			this->rectangulo.x = 0;
			this->vel = 0;
			this->rectangulo.w = anchoVentana;
		}
	}
}
