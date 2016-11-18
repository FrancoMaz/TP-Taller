/*
 * Arma.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ARMA_H_
#define ARMA_H_

class Arma {

protected:
	bool disparoDiagonal;
	int municiones;
	int danioEnPorcentaje;
	int puntosPorDisparo;

public:
	Arma();
	virtual ~Arma();
	virtual void disparar();
	virtual bool sinMuniciones();
};


#endif /* ARMA_H_ */
