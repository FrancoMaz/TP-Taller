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
	VistaBala(int x, int y, TexturaSDL* textura, int id);
	virtual ~VistaBala();
	int x;
	int y;
	TexturaSDL* textura;
	int id;
};

#endif /* VISTABALA_H_ */
