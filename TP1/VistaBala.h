/*
 * VistaBala.h
 *
 *  Created on: 20 nov. 2016
 *      Author: franco
 */

#ifndef VISTABALA_H_
#define VISTABALA_H_
#include "TexturaSDL.h"

class VistaBala {
public:
	VistaBala(int x, int y, TexturaSDL* textura, int id, string sentido);
	virtual ~VistaBala();
	int x;
	int y;
	TexturaSDL* textura;
	int id;
	SDL_RendererFlip flip;
};

#endif /* VISTABALA_H_ */
