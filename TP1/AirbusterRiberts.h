/*
 * AirbusterRiberts.h
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#ifndef AIRBUSTERRIBERTS_H_
#define AIRBUSTERRIBERTS_H_
#include "Boss.h"

class AirbusterRiberts: public Boss {
public:
	AirbusterRiberts(string nombre, string x, string y);
	virtual ~AirbusterRiberts();
	void comportamiento(SDL_Rect camara);
};

#endif /* AIRBUSTERRIBERTS_H_ */
