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
#include <iostream>

using namespace std;

class XmlParser {

private:
	string xmlPath;
	pugi::xml_node rootNode;

public:
	XmlParser(string path);
	const char * cantidadMaximaDeJugadores();
	pair<const char *, const char *> tamanioVentana();
	virtual ~XmlParser();
};

#endif /* XMLPARSER_H_ */
