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
	const char* path;
	const char* zIndex;
	const char* velocidad;
public:
	ImagenDto(const char* path, const char* zIndex, const char* velocidad);
	virtual ~ImagenDto();
	string getPath();
	const char* getZIndex();
	const char* getVelocidad();
};

#endif /* IMAGENDTO_H_ */
