/*
 * Mensaje.h
 *
 *  Created on: Sep 1, 2016
 *      Author: franco
 */

#ifndef MENSAJE_H_
#define MENSAJE_H_

#include <string>
using namespace std;

class Mensaje {
public:
	Mensaje(string remitente, string destinatario, string textoMensaje);
	virtual ~Mensaje();

private:
	string clienteRemitente;
	string clienteDestinatario;
	string textoMensaje;
};

#endif /* MENSAJE_H_ */
