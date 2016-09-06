/*
 * Mensaje.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: franco
 */

#include "Mensaje.h"

Mensaje::Mensaje(string remitente, string destinatario, string textoMensaje) {
	this -> clienteRemitente = remitente;
	this -> clienteDestinatario = destinatario;
	this -> textoMensaje = textoMensaje;

}

Mensaje::Mensaje(){

}

Mensaje::~Mensaje() {
	// TODO Auto-generated destructor stub
}

string Mensaje::getTexto(){
	return this->textoMensaje;
}

char* Mensaje::getStringDatos(){
	return strdup((this->clienteRemitente + ',' + this->clienteDestinatario + ',' + this-> textoMensaje).c_str());
}

void Mensaje::setearDatos(char* datosMensaje){
	int pos = 0;
	bool datosCompletos = false;
	bool termino = false;
	while ((pos < strlen(datosMensaje)) && datosMensaje[pos] != ',') {
		this->clienteRemitente = this->clienteRemitente + datosMensaje[pos];
		pos++;
		}
	while ((pos < strlen(datosMensaje)) && datosMensaje[pos] != ',') {
		this->clienteDestinatario = this->clienteDestinatario + datosMensaje[pos];
		pos++;
		}
	while (pos < strlen(datosMensaje)) {
		this->textoMensaje = this->textoMensaje + datosMensaje[pos];
		pos++;
		}
}
