/*
 * ImagenDto.h
 *
 *  Created on: 9 oct. 2016
 *      Author: josefina
 */

#ifndef IMAGENDTO_H_
#define IMAGENDTO_H_

#include <string>
#include <list>
#include <iostream>

using namespace std;

class ImagenDto {
private:
	string path;
	string ancho;
	string alto;
	string zIndex;
public:
	ImagenDto(string path, string ancho, string alto, string zIndex);
	virtual ~ImagenDto();
	string getPath();
	string getAncho();
	string getAlto();
	string getZIndex();
	void setPath(string path);
	void setAncho(string ancho);
	void setAlto(string alto);
	void setZIndex(string zIndex);
};

#endif /* IMAGENDTO_H_ */
