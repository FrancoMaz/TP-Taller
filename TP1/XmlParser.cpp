/*
 * XmlParser.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: josefina
 */

#include "XmlParser.h"

XmlParser::XmlParser(string xmlPath) {

	this->xmlPath = xmlPath;
	// Create empty XML document within memory
	pugi::xml_document doc;
	// Load XML file into memory
	pugi::xml_parse_result result = doc.load_file(xmlPath.c_str(), pugi::parse_default|pugi::parse_declaration);
	if (!result) {
		cout << "Parse error: " << result.description() << ", character pos= " << result.offset << endl;
	}
	// A valid XML document must have a single root node, in our case the root node is "Configuracion"

	this->rootNode = doc.child("Configuracion");
	this->cantidadMaximaDeJugadores = "";
	pair<const char*, const char*> dimensiones ("", "");
	this->tamanioVentana = dimensiones;
	this->escenario = new list<ImagenDto>();
	this->sprites = new list<SetDeSpritesDto>();
}

const char* XmlParser::getCantidadMaximaDeJugadores() {
	if (this->cantidadMaximaDeJugadores == "") {
		this->cantidadMaximaDeJugadores = this->rootNode.child_value("CantMaxJugadores");
	}
	return this->cantidadMaximaDeJugadores;
}

pair<const char*, const char*> XmlParser::getTamanioVentana() {
	if (this->tamanioVentana.first == "" && this->tamanioVentana.second == "") {
		pair<const char*,const char*> dimensiones(this->rootNode.child("Ventana").child_value("Ancho"), this->rootNode.child("Ventana").child_value("Alto"));
		this->tamanioVentana = dimensiones;
	}
	return this->tamanioVentana;
}

list<ImagenDto>* XmlParser::getEscenario() {
	if (this->escenario->empty()) {
		for (pugi::xml_node image = this->rootNode.child("Escenario").first_child(); image; image = image.next_sibling()) {
			ImagenDto* imagen = new ImagenDto(image.child_value("Archivo"), image.child_value("z-index"), image.child_value("Velocidad"));
			this->escenario->push_back(*imagen);
		}
	}

	for (list<ImagenDto>::iterator i = this->escenario->begin(); i != this->escenario->end();i++) {
		cout << (*i).getPath() << (*i).getVelocidad() << (*i).getZIndex() << endl;
	}
	return this->escenario;
}

list<SetDeSpritesDto>* XmlParser::getSprites() {
	if (this->sprites->empty()) {
		for (pugi::xml_node set = this->rootNode.child("Sprites").child("SetRojo"); set; set = set.next_sibling()) {
			list<SpriteDto>* sprites = new list<SpriteDto>;
			for (pugi::xml_node sprite = set.child("SpriteSalto"); sprite; sprite = sprite.next_sibling()) {
				SpriteDto* spriteDto = new SpriteDto(sprite.child_value("Id"), sprite.child_value("CantFotogramas"), sprite.child_value("Ancho"), sprite.child_value("Alto"), sprite.child_value("Imagen"), sprite.child_value("z-index"));
				sprites->push_back(*spriteDto);
			}
			SetDeSpritesDto* setDeSprites = new SetDeSpritesDto(set.child_value("Carpeta"), sprites);
			this->sprites->push_back(*setDeSprites);
		}
	}

	for (list<SetDeSpritesDto>::iterator i = this->sprites->begin(); i != this->sprites->end();i++) {
		cout << (*i).getCarpeta() << endl;
		for (list<SpriteDto>::iterator j = (*i).getSprites()->begin(); j != (*i).getSprites()->end();j++) {
			cout << (*j).getId() << endl;
			cout << (*j).getCantidadDeFotogramas() << endl;
			cout << (*j).getAncho() << endl;
			cout << (*j).getAlto() << endl;
			cout << (*j).getPath() << endl;
			cout << (*j).getZIndex() << endl;
		}
	}

	return this->sprites;
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

