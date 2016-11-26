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
#include <vector>
#include <algorithm>
#include "Item.h"
#include "Plataforma.h"

using namespace std;

class XmlParser {

private:
	// TODO continuar de poner como atributos todos los elementos del xml
	string xmlPath;
	pugi::xml_node rootNode;
	const char* cantidadMaximaDeJugadores;
	pair<string, string> tamanioVentana;
	vector<ImagenDto*> escenario;
	vector<SetDeSpritesDto*> sprites;
	vector<Plataforma*> plataformas;
	vector<Item*> itemArmas;

public:
	XmlParser(string path);
	const char * getCantidadMaximaDeJugadores();
	pair<string, string> getTamanioVentana();
	string serializarVentana();
	vector<ImagenDto*> getEscenario();
	string serializarEscenario();
	vector<SetDeSpritesDto*> getSprites();
	string serializarSprites(vector<SpriteDto*> sprites, vector<string> colores);
	string serializarSetDeSprites(vector<string> colores);
	void mostrarSprites(SpriteDto* sprite);
	virtual ~XmlParser();
	vector<Plataforma*> getPlataformas();
	vector<Item*> getItemArmas();
	bool stringTerminaCon(std::string const &fullString, std::string const &ending);
};

#endif /* XMLPARSER_H_ */
