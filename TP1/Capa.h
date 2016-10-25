/*
 * Capa.h
 *
 *  Created on: 24 oct. 2016
 *      Author: franco
 */

#ifndef CAPA_H_
#define CAPA_H_

#include "TexturaSDL.h"
#include "ImagenDto.h"

class Capa {
public:
	Capa(ImagenDto* imagen, SDL_Rect rectangulo, TexturaSDL* textura);
	virtual ~Capa();
	SDL_Rect rectangulo;
	ImagenDto* imagen;
	int velocidad;
	TexturaSDL* textura;
	void calcularVelocidad(int anchoCapaPrincipal, int velocidadCamara, int anchoVentana);
	void paralajeInfinito(int anchoVentana);
};

#endif /* CAPA_H_ */
