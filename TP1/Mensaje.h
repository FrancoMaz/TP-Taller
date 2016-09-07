/*
 * Mensaje.h
 *
 *  Created on: Sep 1, 2016
 *      Author: franco
 */

#ifndef MENSAJE_H_
#define MENSAJE_H_

#include <string>
#include <string.h>
#include <sstream>
using namespace std;

class Mensaje {
public:
	Mensaje(string remitente, string destinatario, string textoMensaje);
	Mensaje();
	virtual ~Mensaje();
	string getTexto();
	char* getStringDatos();
	void setearDatos(char* datosMensaje);
	string getRemitente();
	string getDestinatario();

private:
	string clienteRemitente;
	string clienteDestinatario;
	string textoMensaje;
};

#endif /* MENSAJE_H_ */
