/*
 * SetDeSpritesDto.cpp
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#include "SetDeSpritesDto.h"

SetDeSpritesDto::SetDeSpritesDto(string carpeta, vector<SpriteDto*> sprites) {
	this->carpeta = carpeta;
	this->sprites = sprites;
}

string SetDeSpritesDto::getCarpeta() {
	return this->carpeta;
}

vector<SpriteDto*> SetDeSpritesDto::getSprites() {
	return this->sprites;
}

SetDeSpritesDto::~SetDeSpritesDto() {
	// TODO Auto-generated destructor stub
}

