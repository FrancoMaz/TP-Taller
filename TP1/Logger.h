/*
 * Logger.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_LOG_H_
#define TP1_LOG_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Logger {
public:
	Logger(int modo);
	virtual ~Logger();
	void escribir(string mensaje);

private:
	int modo;
};

#endif /* TP1_LOGGER_H_ */
