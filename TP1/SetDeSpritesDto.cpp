/*
 * SetDeSpritesDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "SetDeSpritesDto.h"

SetDeSpritesDto::SetDeSpritesDto(const char* carpeta, list<SpriteDto>* sprites) {
	this->carpeta = carpeta;
	this->sprites = sprites;
}

SetDeSpritesDto::~SetDeSpritesDto() {
	// TODO Auto-generated destructor stub
}

