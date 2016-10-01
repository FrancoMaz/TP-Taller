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
#include <ctime>
using namespace std;

class Logger {
public:
	Logger(int modo);
	virtual ~Logger();
	void escribir(string mensaje, int nivelDeLog);
	time_t timer;
	struct tm* timeinfo;
	pthread_mutex_t mutexLog;
private:
	int modo;
	FILE* archivoLog;
};

#endif /* TP1_LOGGER_H_ */
