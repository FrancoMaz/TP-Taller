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
	pugi::xml_parse_result result = doc.load_file(this->xmlPath.c_str(), pugi::parse_default|pugi::parse_declaration);
	if (!result) {
		cout << "Parse error: " << result.description() << ", character pos= " << result.offset << endl;
	}
	// A valid XML document must have a single root node, in our case the root node is "Configuracion"

	this->rootNode = doc.child("Configuracion");
}

const char* XmlParser::cantidadMaximaDeJugadores() {
	return this->rootNode.child_value("CantMaxJugadores");
}

pair<const char*, const char*> XmlParser::tamanioVentana() {
	pair<const char*,const char*> dimensiones(this->rootNode.child("Ventana").child_value("Ancho"), this->rootNode.child("Ventana").child_value("Alto"));
	return dimensiones;
}

list<ImagenDto>* XmlParser::escenario() {
	list<ImagenDto>* images = new list<ImagenDto>;
	for (pugi::xml_node image = this->rootNode.child("Escenario").first_child(); image; image = image.next_sibling()) {
		ImagenDto* imagen = new ImagenDto(image.child_value("Archivo"), image.child_value("z-index"), image.child_value("Velocidad"));
		images->push_back(*imagen);
	}

	for (list<ImagenDto>::iterator i = images->begin(); i != images->end();i++) {
		cout << (*i).getPath() << (*i).getVelocidad() << (*i).getZIndex() << endl;
	}
	return images;
}

list<SetDeSpritesDto>* XmlParser::sprites() {
	list<SetDeSpritesDto>* setDeSprites = new list<SetDeSpritesDto>;
	for (pugi::xml_node set = this->rootNode.child("Sprites"); set; set = set.next_sibling()) {
		list<SpriteDto>* sprites = new list<SpriteDto>;
		for (pugi::xml_node sprite = this->rootNode.child("SpriteSalto"); sprite; sprite = sprite.next_sibling()) {
			SpriteDto* spriteDto = new SpriteDto(sprite.child_value("Id"), sprite.child_value("CantFotogramas"), sprite.child_value("Ancho"), sprite.child_value("Alto"), sprite.child_value("Imagen"), sprite.child_value("z-index"));
			sprites->push_back(*spriteDto);
		}
		SetDeSpritesDto* setDeSprites = new SetDeSpritesDto(set.child_value("Carpeta"), sprites);
	}
	return setDeSprites;
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

