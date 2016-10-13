/*
 * MainParser.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: josefina
 */

#include "XmlParser.h"

int main() {
	XmlParser* parser = new XmlParser("Recursos/configuration.xml");
	cout << "Cantidad maxima de jugadores: " << parser->getCantidadMaximaDeJugadores() << endl;
	cout << "Ancho de ventana: " << parser->getTamanioVentana().first << " Alto de ventana: " << parser->getTamanioVentana().second << endl;
	parser->getEscenario();
	parser->getSprites();

    return 0;
}
