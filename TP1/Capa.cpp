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
}

Capa::~Capa() {
	// TODO Auto-generated destructor stub
}

void Capa::calcularVelocidad(int anchoCapaPrincipal, int velocidadCamara, int anchoVentana)
{
	this->velocidad = abs(((atoi(this->imagen->getAncho().c_str())-anchoVentana)/(anchoCapaPrincipal - anchoVentana))*velocidadCamara);
	this->rectangulo.x += this->velocidad;
}

void Capa::paralajeInfinito(int anchoVentana)
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

		if (this->rectangulo.x > atoi(this->imagen->getAncho().c_str()))
		{
			this->rectangulo.x = 0;
			this->velocidad = 0;
			this->rectangulo.w = anchoVentana;
		}
	} else {
		this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);
	}
}
