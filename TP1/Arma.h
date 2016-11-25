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
	int municionesPorRepuesto;
	Proyectil* proyectil;

public:
	Arma();
	virtual ~Arma();
	virtual Proyectil* disparar(SDL_Rect boxCollider, string condicion);
	virtual bool sinMuniciones();
	virtual void sumarMuniciones();
	string nombre;
	string nombreArma;
};


#endif /* ARMA_H_ */
