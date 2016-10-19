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
	vector<ImagenDto*> imagenes;
	vector<SetDeSpritesDto*> sprites;
	char* alto;
	char* ancho;

public:
	Handshake(vector<ImagenDto*> imagenes, vector<SetDeSpritesDto*> sprites, char* ancho, char* alto);
	virtual ~Handshake();
	vector<SetDeSpritesDto*> getSprites();
	vector<ImagenDto*> getImagenes();

};

#endif /* HANDSHAKE_H_ */
