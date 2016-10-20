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
	string id;
	int cantidadDeFotogramas;
	int ancho;
	int alto;
	const char* path;
	const char* zIndex;
public:
	SpriteDto(string id, int cantidadDeFotogramas, int ancho, int alto, const char* path, const char* zIndex);
	string getId();
	int getCantidadDeFotogramas();
	int getAncho();
	int getAlto();
	const char* getPath();
	const char* getZIndex();
	virtual ~SpriteDto();
};

#endif /* SPRITEDTO_H_ */
