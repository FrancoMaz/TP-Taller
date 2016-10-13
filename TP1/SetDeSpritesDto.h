/*
 * SetDeSpritesDto.h
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#ifndef SETDESPRITESDTO_H_
#define SETDESPRITESDTO_H_

#include <string>
#include <list>
#include <iostream>
#include "SpriteDto.h"

using namespace std;

class SetDeSpritesDto {

private:
	const char* carpeta;
	list<SpriteDto>* sprites;

public:
	SetDeSpritesDto(const char* carpeta, list<SpriteDto>* sprites);
	const char* getCarpeta();
	list<SpriteDto>* getSprites();
	virtual ~SetDeSpritesDto();
};

#endif /* SETDESPRITESDTO_H_ */
