/*
 * VistaEnemigo.h
 *
 *  Created on: 25 nov. 2016
 *      Author: josefina
 */

#ifndef VISTAENEMIGO_H_
#define VISTAENEMIGO_H_

#include "TexturaSDL.h"

class VistaEnemigo {
public:
	VistaEnemigo(int x, int y, TexturaSDL* textura, int id, string sentido);
	int x;
	int y;
	TexturaSDL* textura;
	int id;
	SDL_RendererFlip flip;
	void verificarSentido(string sentido);
	virtual ~VistaEnemigo();
};

#endif /* VISTAENEMIGO_H_ */
