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
	for (int i = 0; i < sprites.size(); i++)
	{
		delete sprites.at(i);
	}
	sprites.clear();
	vector<SpriteDto*>().swap(sprites);
}
