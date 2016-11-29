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
	int municiones;
	int municionesPorRepuesto;
	Proyectil* proyectil;
	SDL_Rect boxCollider;

public:
	Arma();
	virtual ~Arma();

	//si disparadoPor es 1, recibe un nombre de jugador
	virtual Proyectil* disparar(int posicionX,int posicionY, string condicion, int anguloBala, bool agachar, int disparadoPor, string nombreJugador = NULL);
	virtual bool sinMuniciones();
	virtual void sumarMuniciones();
	string nombre;
	string nombreArma;
	bool disparoDiagonal;
	bool cortoAlcance;
};


#endif /* ARMA_H_ */
