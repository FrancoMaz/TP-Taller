/*
 * RocketLauncher.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ROCKETLAUNCHER_H_
#define ROCKETLAUNCHER_H_

#include "Arma.h"

class RocketLauncher: public Arma {
public:
	RocketLauncher();
	virtual ~RocketLauncher();
	void disparar();
	bool sinMuniciones();
};

#endif /* ROCKETLAUNCHER_H_ */
