/*
 * VistaEnemigo.cpp
 *
 *  Created on: 25 nov. 2016
 *      Author: josefina
 */

#include "VistaEnemigo.h"

VistaEnemigo::VistaEnemigo(int x, int y, TexturaSDL* textura, int id, string sentido) {
	this->x = x;
	this->y = y;
	this->textura = textura;
	this->id = id;
	this->verificarSentido(sentido);
}

VistaEnemigo::~VistaEnemigo() {
	delete this->textura;
}

void VistaEnemigo::verificarSentido(string sentido)
{
	if (sentido == "Normal")
	{
		this->flip = SDL_FLIP_NONE;
	}
	else
	{
		this->flip = SDL_FLIP_HORIZONTAL;
	}
}

void VistaEnemigo::setTexturaSprite(TexturaSDL* nuevaTextura) {
	this->textura = nuevaTextura;
}
