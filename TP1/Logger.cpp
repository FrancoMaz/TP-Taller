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
	if (this->modo == 1) {
		if (nivelDeLog == 1) {
			pthread_mutex_lock(&mutexLog);
			this->archivoLog = fopen("Log.txt", "a");
			//fputs(mensaje.c_str(), this->archivoLog);
			//lo de abajo se tiene que borrar despues
			stringstream ss;
			time(&timer);
			char buffer [80];
			timeinfo = localtime(&timer);
			strftime(buffer,80, "%H:%M:%S", timeinfo);
			ss << buffer;
			fputs((("[" + ss.str() + "] ") + mensaje).c_str(), this->archivoLog);
					//hasta aca
			fclose(this->archivoLog);

			pthread_mutex_unlock(&mutexLog);
		}
	} else {
		// si el modo del logger es DEBUG se debe loggear todo

		pthread_mutex_lock(&mutexLog);
		this->archivoLog = fopen("Log.txt", "a");
		stringstream ss;
		time(&timer);
		char buffer [80];
		timeinfo = localtime(&timer);
		strftime(buffer,80, "%H:%M:%S", timeinfo);
		ss << buffer;
		fputs((("[" + ss.str() + "] ") + mensaje).c_str(), this->archivoLog);
		fclose(this->archivoLog);

		pthread_mutex_unlock(&mutexLog);
	}
}
