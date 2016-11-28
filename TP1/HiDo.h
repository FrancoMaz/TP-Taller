/*
 * HiDo.h
 *
 *  Created on: 28 nov. 2016
 *      Author: franco
 */

#ifndef HIDO_H_
#define HIDO_H_
#define VELOCIDADHIDO 3
#define VELOCIDADHIDOMAX 6
#include "Boss.h"

class HiDo: public Boss {
public:
	HiDo(string nombre, string x, string y);
	virtual ~HiDo();
	void comportamiento(SDL_Rect camara);
};

#endif /* HIDO_H_ */
