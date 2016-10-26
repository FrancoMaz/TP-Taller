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
	int vel;
	TexturaSDL* textura;
	void calcularVelocidad(int anchoCapaPrincipal, int velocidadCamara, int anchoVentana);
	void paralajeInfinito(int anchoVentana, int indice);
	void paralajeInfinitoCapaPrincipal(int anchoVentana);
	void paralajeInfinitoOtrasCapas(int anchoVentana);
	void modificarRectangulo(int vel, int anchoVentana, int anchoEscenario);
	void resetearDatos(int anchoVentana, int indice);
};

#endif /* CAPA_H_ */
