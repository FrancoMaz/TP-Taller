/*
 * Arma.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ARMA_H_
#define ARMA_H_
#include "Proyectil.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

class Arma {

protected:
	bool disparoDiagonal;
	int municiones;
	Proyectil* proyectil;

public:
	Arma();
	virtual ~Arma();
	virtual Proyectil* disparar(SDL_Rect boxCollider, string condicion);
	virtual bool sinMuniciones();
	Proyectil* getProyectil();
	string nombre;
};


#endif /* ARMA_H_ */
