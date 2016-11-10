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
	this -> eventoSerializado = textoMensaje;

}

Mensaje::Mensaje(){

}

Mensaje::~Mensaje() {
	// TODO Auto-generated destructor stub
}

string Mensaje::getTexto(){
	return this->textoMensaje;
}

string Mensaje::getStringDatos(){
	return (this->clienteRemitente + '|' + this->clienteDestinatario + '|' + this->textoMensaje);
}

void Mensaje::setRemitente(string remitente)
{
	this->clienteRemitente = remitente;
}

void Mensaje::setDestinatario(string destinatario)
{
	this->clienteDestinatario = destinatario;
}

void Mensaje::setearDatos(char* datosMensaje){
	int pos = 0;
	bool remitenteCompleto = false;
	bool destinatarioCompleto = false;
	while (pos < strlen(datosMensaje) && datosMensaje[pos]!=',') {
			this->clienteRemitente = this->clienteRemitente + datosMensaje[pos];
			pos++;}
	pos++;
	while (pos < strlen(datosMensaje) && datosMensaje[pos]!=',') {
				this->clienteDestinatario = this->clienteDestinatario + datosMensaje[pos];
				pos++;}
	pos++;
	while (pos < strlen(datosMensaje)) {
		this->textoMensaje = this->textoMensaje + datosMensaje[pos];
		pos++;}
	}

string Mensaje::getRemitente(){
	return this->clienteRemitente;
}

string Mensaje::getDestinatario(){
	return this->clienteDestinatario;
}

SDL_Keycode Mensaje::deserializar(string evento)
{

	if (evento == "Tecla Derecha")
	{
		return SDLK_RIGHT;
	}
	else if (evento == "Tecla Izquierda")
	{
		return SDLK_LEFT;
	}
	else if (evento == "Tecla Arriba")
	{
		return SDLK_UP;
	}
	else if (evento == "Tecla Abajo")
	{
		return SDLK_DOWN;
	}
	else if (evento == "Tecla Espacio")
	{
		return SDLK_SPACE;
	}
	else if (evento == "Tecla R")
	{
		return SDLK_r;
	}
	else if (evento == "Soltar Tecla Derecha")
	{
		return SDLK_RIGHT;
	}
	else if (evento == "Soltar Tecla Izquierda")
	{
		return SDLK_LEFT;
	}
	else if (evento == "Soltar Tecla Abajo")
	{
		return SDLK_DOWN;
	}
	else if (evento == "Soltar Tecla Espacio")
	{
		return SDLK_SPACE;
	}
	return -1;
}

bool Mensaje::sePresionoTecla()
{
	if (eventoSerializado == "Tecla Derecha" or eventoSerializado == "Tecla Izquierda" or eventoSerializado == "Tecla Arriba" or eventoSerializado == "Tecla Abajo" or eventoSerializado == "Tecla Espacio")
	{
		return true;
	}
	return false;
}
