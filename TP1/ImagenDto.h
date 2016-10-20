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
	const char* ancho;
	const char* alto;
public:
	ImagenDto(const char* path, const char* ancho, const char* alto);
	virtual ~ImagenDto();
	string getPath();
	int getAncho();
	int getAlto();
};

#endif /* IMAGENDTO_H_ */
