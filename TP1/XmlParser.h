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
#include "Boss.h"
#include "HiDo.h"
#include "TaniOh.h"
#include "AirbusterRiberts.h"

using namespace std;

class XmlParser {

private:
	string xmlPath;
	pugi::xml_node rootNode;
	const char* cantidadMaximaDeJugadores;
	pair<string, string> tamanioVentana;
	vector<ImagenDto*> escenario;
	vector<SetDeSpritesDto*> sprites;
	vector<pair<string,string>> plataformas;
	vector<Item*> itemArmas;
	Boss* boss;

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
	vector<pair<string,string>> getPlataformas();
	vector<Item*> getItemArmas();
	bool stringTerminaCon(string const &fullString, string const &ending);
	Boss* getBoss();
	Boss* devolverBoss(string nombre, string x, string y);
	vector<string> getCapas();
	vector<pair<int,int>> getEnemigos();
};

#endif /* XMLPARSER_H_ */
