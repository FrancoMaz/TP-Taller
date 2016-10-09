/*
 * MainParser.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: josefina
 */

#include "XmlParser.h"
#include "pugixml.hpp"

int main() {
	XmlParser* parser = new XmlParser("/home/josefina/git/TP-Taller/TP1/Recursos/configuration.xml");
	cout << "Cantidad maxima de jugadores: " << parser->cantidadMaximaDeJugadores() << endl;
	cout << "Ancho de ventana: " << parser->tamanioVentana().first << "Alto de ventana: " << parser->tamanioVentana().second << endl;
}
