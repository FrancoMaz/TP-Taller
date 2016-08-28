/*
 * Log.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_LOG_H_
#define TP1_LOG_H_

#include <string>
using namespace std;

class Log {
public:
	Log();
	virtual ~Log();
	void guardarMensajeDeError(string mensaje);
	void guardarMensajeDeExito(string mensaje);

private:
	string rutaArchivo;
};

#endif /* TP1_LOG_H_ */
