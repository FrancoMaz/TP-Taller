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
	Mensaje(Cliente remitente, Cliente destinatario, string textoMensaje);
	virtual ~Mensaje();

private:
	Cliente clienteRemitente;
	Cliente clienteDestinatario;
	string textoMensaje;
};

#endif /* MENSAJE_H_ */
