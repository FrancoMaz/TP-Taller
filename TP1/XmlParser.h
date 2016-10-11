/*
 * XmlParser.h
 *
 *  Created on: 8 oct. 2016
 *      Author: josefina
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include "pugixml.hpp"
#include <string>
#include <list>
#include <iostream>
#include "ImagenDto.h"

using namespace std;

class XmlParser {

private:
	string xmlPath;
	pugi::xml_node rootNode;

public:
	XmlParser(string path);
	const char * cantidadMaximaDeJugadores();
	pair<const char *, const char *> tamanioVentana();
	list<ImagenDto>* escenario();
	list<SetDeSpritesDto>* sprites();
	virtual ~XmlParser();
};

#endif /* XMLPARSER_H_ */
