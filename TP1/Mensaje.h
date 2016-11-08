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
#include <SDL2/SDL.h>
using namespace std;

class Mensaje {
public:
	Mensaje(string remitente, string destinatario, string textoMensaje);
	Mensaje();
	virtual ~Mensaje();
	string getTexto();
	string getStringDatos();
	void setearDatos(char* datosMensaje);
	void setRemitente(string remitente);
	void setDestinatario(string destinatario);
	string getRemitente();
	string getDestinatario();
	SDL_Keycode deserializar(string evento);
	bool sePresionoTecla();

private:
	string clienteRemitente;
	string clienteDestinatario;
	string textoMensaje;
	string eventoSerializado;
};

#endif /* MENSAJE_H_ */
