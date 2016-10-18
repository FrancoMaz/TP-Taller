/*
 * SpriteDto.h
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#ifndef SPRITEDTO_H_
#define SPRITEDTO_H_

#include <string>
#include <list>
#include <iostream>

using namespace std;

class SpriteDto {
private:
	const char* id;
	const char* cantidadDeFotogramas;
	const char* ancho;
	const char* alto;
	const char* path;
	const char* zIndex;
public:
	SpriteDto(const char* id, const char* cantidadDeFotogramas, const char* ancho, const char* alto, const char* path, const char* zIndex);
	string getId();
	const char* getCantidadDeFotogramas();
	const char* getAncho();
	const char* getAlto();
	const char* getPath();
	const char* getZIndex();
	virtual ~SpriteDto();
};

#endif /* SPRITEDTO_H_ */
