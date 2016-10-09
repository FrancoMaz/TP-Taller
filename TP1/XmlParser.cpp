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

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

