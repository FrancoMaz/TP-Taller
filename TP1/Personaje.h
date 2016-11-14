/*
 * Personaje.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_
#include "Arma.h"
#include <vector>

using namespace std;

class Personaje {
private:
	int vida;
	vector<Arma> armas;
public:
	Personaje();
	virtual ~Personaje();
	virtual void disparar();
	virtual void daniarseCon(Arma* arma);
};

#endif /* PERSONAJE_H_ */
