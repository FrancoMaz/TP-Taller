/*
 * HeavyMachineGun.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef HEAVYMACHINEGUN_H_
#define HEAVYMACHINEGUN_H_

#include "Arma.h"

class HeavyMachineGun: public Arma {
public:
	HeavyMachineGun();
	virtual ~HeavyMachineGun();
	Proyectil* disparar(int posicionX,int posicionY, string condicion, int anguloBala, bool agachar, int disparadoPor);
	bool sinMuniciones();
	void sumarMuniciones();
};

#endif /* HEAVYMACHINEGUN_H_ */
