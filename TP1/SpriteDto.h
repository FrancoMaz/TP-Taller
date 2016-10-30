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
	string getID();
	string getCantidadDeFotogramas();
	string getAncho();
	string getAlto();
	string getPath();
	string getZIndex();
	void setID(string ID);
	void setAncho(string ancho);
	void setAlto(string alto);
	void setZIndex(string zIndex);
	void setCantFotogramas(string cantFotogramas);
	virtual ~SpriteDto();
};

#endif /* SPRITEDTO_H_ */
