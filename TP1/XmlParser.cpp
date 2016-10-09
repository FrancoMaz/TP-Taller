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
	// A valid XML document must have a single root node, in our case the root node is "Configuration"
	this->rootNode = doc.document_element();
}

const char* XmlParser::cantidadMaximaDeJugadores() {
	string tagName = "CantMaxJugadores";
	return this->rootNode.child(tagName.c_str()).value();
}

pair<const char*, const char*> XmlParser::tamanioVentana() {
	string tagName = "Ventana";
	pugi::xml_node ventana = this->rootNode.child(tagName.c_str());
	pair<const char*,const char*> dimensiones(ventana.child("Ancho").value(), ventana.child("Alto").value());
	return dimensiones;
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

