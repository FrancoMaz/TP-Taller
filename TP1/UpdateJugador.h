/*
 * UpdateJugador.h
 *
 *  Created on: 22 oct. 2016
 *      Author: franco
 */

#ifndef UPDATEJUGADOR_H_
#define UPDATEJUGADOR_H_

#include <string.h>
#include "SpriteDto.h"
using namespace std;

class UpdateJugador {

private:
	string remitente;
	string destinatario;
	string x;
	string y;
	string condicion;
	SpriteDto* spriteAEjecutar;
public:
	UpdateJugador();
	virtual ~UpdateJugador();
	void setRemitente(string remitente);
	void setDestinatario(string destinatario);
	void setX(string x);
	void setY(string y);
	void setCondicion(string condicion);
	void setSprite(SpriteDto* sprite);
	string getRemitente();
	string getDestinatario();
	string getX();
	string getY();
	string getCondicion();
	SpriteDto* getSprite();

};

#endif /* UPDATEJUGADOR_H_ */
