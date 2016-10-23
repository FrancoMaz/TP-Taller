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
	string cantidadDeFotogramas;
	string ancho;
	string alto;
	string path;
	string zIndex;
public:
	SpriteDto(string id, string cantidadDeFotogramas, string ancho, string alto, string path, string zIndex);
	string getId();
	string getCantidadDeFotogramas();
	string getAncho();
	string getAlto();
	string getPath();
	string getZIndex();
	virtual ~SpriteDto();
};

#endif /* SPRITEDTO_H_ */
