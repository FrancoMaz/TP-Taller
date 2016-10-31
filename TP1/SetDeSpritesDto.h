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
#include <vector>

using namespace std;

class SetDeSpritesDto {

private:
	string carpeta;
	vector<SpriteDto*> sprites;

public:
	SetDeSpritesDto(string carpeta, vector<SpriteDto*> sprites);
	string getCarpeta();
	vector<SpriteDto*> getSprites();
	virtual ~SetDeSpritesDto();
};

#endif /* SETDESPRITESDTO_H_ */
