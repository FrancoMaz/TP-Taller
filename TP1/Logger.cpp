/*
 * Logger.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Logger.h"
using namespace std;

Logger::Logger(int modo) {
	this->modo = modo;
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::escribir(string mensaje) {
	ofstream logFile("Log.txt");

	if (logFile.is_open()) {
		logFile << mensaje;
	}

	logFile.close();
}
