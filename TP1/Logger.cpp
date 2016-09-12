/*
 * Logger.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Logger.h"
using namespace std;

/*
 modo == 1 : INFO
 modo == 2 : DEBUG
 */

Logger::Logger(int modo) {
	this->modo = modo;
	this->archivoLog = fopen("Log.txt", "w");
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::escribir(string mensaje, int nivelDeLog) {
	cout << "Modo del logger: " << modo << endl;
	cout << "Nivel de log del mensaje: " << nivelDeLog << endl;
	if (this->modo == 1) {
		if (nivelDeLog == 1) {
			this->archivoLog = fopen("Log.txt", "a");
			cout << "Entro en modo INFO" << endl;
			cout << mensaje << endl;
			fputs(mensaje.c_str(), this->archivoLog);
			fclose(this->archivoLog);
		}
	} else {
		// si el modo del logger es DEBUG se debe loggear todoo
		this->archivoLog = fopen("Log.txt", "a");
		cout << "Entro en modo DEBUG" << endl;
		cout << mensaje << endl;
		fputs(mensaje.c_str(), this->archivoLog);
		fclose(this->archivoLog);
	}
}
