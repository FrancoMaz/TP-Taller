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
#include "SpriteDto.h"
#include "SetDeSpritesDto.h"

using namespace std;

class XmlParser {

private:
	// TODO continuar de poner como atributos todos los elementos del xml
	string xmlPath;
	pugi::xml_node rootNode;
	const char* cantidadMaximaDeJugadores;
	pair<const char*, const char*> tamanioVentana;
	list<ImagenDto>* escenario;
	list<SetDeSpritesDto*> sprites;

public:
	XmlParser(string path);
	const char * getCantidadMaximaDeJugadores();
	pair<const char *, const char *> getTamanioVentana();
	string serializarVentana();
	list<ImagenDto>* getEscenario();
	string serializarEscenario();
	list<SetDeSpritesDto*> getSprites();
	string serializarSprites(list<SpriteDto*> sprites);
	string serializarSetDeSprites();
	void mostrarSprites(SpriteDto* sprite);
	virtual ~XmlParser();
};

#endif /* XMLPARSER_H_ */
