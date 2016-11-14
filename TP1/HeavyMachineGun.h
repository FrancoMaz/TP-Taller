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
	void disparar();
};

#endif /* HEAVYMACHINEGUN_H_ */
