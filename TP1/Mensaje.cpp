/*
 * Mensaje.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: franco
 */

#include "Mensaje.h"

Mensaje::Mensaje(Cliente remitente, Cliente destinatario, string textoMensaje) {
	this -> clienteRemitente = remitente;
	this -> clienteDestinatario = destinatario;
	this -> textoMensaje = textoMensaje;

}

Mensaje::~Mensaje() {
	// TODO Auto-generated destructor stub
}

