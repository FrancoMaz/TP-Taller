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

class Handshake {

private:
	ImagenDto* imagen1;
	ImagenDto* imagen2;
	SetDeSpritesDto* setSprite1;
	SetDeSpritesDto* setSprite2;
	SetDeSpritesDto* setSprite3;
	char* alto;
	char* ancho;

public:
	Handshake(ImagenDto* imagen1, ImagenDto* imagen2, SetDeSpritesDto* setSprite1, SetDeSpritesDto* setSprite2, SetDeSpritesDto* setSprite3, char* ancho, char* alto);
	virtual ~Handshake();
	void setImagen1(ImagenDto* imagen1);
	void setImagen2(ImagenDto* imagen2);
	void setSprites1(SetDeSpritesDto* setSprite1);
	void setSprites2(SetDeSpritesDto* setSprite2);
	void setSprites3(SetDeSpritesDto* setSprite3);
	void setAlto(char* alto);
	void setAncho(char* ancho);
	SetDeSpritesDto* getSprites1();
	SetDeSpritesDto* getSprites2();
	SetDeSpritesDto* getSprites3();
	ImagenDto* getImagen1();
	ImagenDto* getImagen2();

};

#endif /* HANDSHAKE_H_ */
