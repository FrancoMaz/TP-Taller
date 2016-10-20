/*
 * SetDeSpritesDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "SetDeSpritesDto.h"

SetDeSpritesDto::SetDeSpritesDto(const char* carpeta, vector<SpriteDto*> sprites) {
	this->carpeta = carpeta;
	this->sprites = sprites;
}

string SetDeSpritesDto::getCarpeta() {
	return string(this->carpeta);
}

vector<SpriteDto*> SetDeSpritesDto::getSprites() {
	return this->sprites;
}

SetDeSpritesDto::~SetDeSpritesDto() {
	// TODO Auto-generated destructor stub
}

