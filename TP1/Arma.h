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
	int municionesPorRepuesto;
	SDL_Rect boxCollider;

public:
	Arma();
	virtual ~Arma();

	//si disparadoPor es 1, recibe un nombre de jugador
	virtual Proyectil* disparar(int posicionX,int posicionY, string condicion, int anguloBala, bool agachar, int disparadoPor, string nombreJugador = "");
	virtual bool sinMuniciones();
	virtual void sumarMuniciones();
	string nombre = "lala gorsu";
	string nombreArma = "miss";
	bool disparoDiagonal = false;
	bool cortoAlcance = false;
	int municiones;
};


#endif /* ARMA_H_ */
