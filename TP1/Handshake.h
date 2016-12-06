/*
 * Handshake.h
 *
 *  Created on: 18 oct. 2016
 *      Author: franco
 */

#ifndef HANDSHAKE_H_
#define HANDSHAKE_H_

#include "ImagenDto.h"
#include "SpriteDto.h"
#include "SetDeSpritesDto.h"
#include "vector"

class Handshake {

private:
	vector<SetDeSpritesDto*> sprites;
	string alto;
	string ancho;
	string cantidadMaximaJugadores;

public:
	Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, string ancho, string alto);
	Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, string ancho, string alto, string cantidadMaxima);
	virtual ~Handshake();
	vector<SetDeSpritesDto*> getSprites();
	vector<ImagenDto*> getImagenes();
	string getAncho();
	string getAlto();
	string getCantidadMaximaJugadores();
	vector<vector<SpriteDto*>> matrizSpritesJugadores;
	vector<ImagenDto*> imagenes;
};

#endif /* HANDSHAKE_H_ */
