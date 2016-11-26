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
}

const char* XmlParser::getCantidadMaximaDeJugadores() {
	if (this->cantidadMaximaDeJugadores == "") {
		this->cantidadMaximaDeJugadores = this->rootNode.child_value("CantMaxJugadores");
	}
	return this->cantidadMaximaDeJugadores;
}

string XmlParser::serializarVentana(){
	this->getTamanioVentana();
	string ventanaConcatenada = "Ventana[";
	ventanaConcatenada += this->tamanioVentana.first; // ancho
	ventanaConcatenada += ",";
	ventanaConcatenada += this->tamanioVentana.second; // alto
	ventanaConcatenada += "]";
	return ventanaConcatenada;
}

pair<string, string> XmlParser::getTamanioVentana() {
	if (this->tamanioVentana.first == "" && this->tamanioVentana.second == "") {
		pair<string,string> dimensiones(string(this->rootNode.child("Ventana").child_value("Ancho")), string(this->rootNode.child("Ventana").child_value("Alto")));
		this->tamanioVentana = dimensiones;
	}
	return this->tamanioVentana;
}

string XmlParser::serializarEscenario(){
	this->getEscenario();
	string escenarioConcatenado = "Escenario[";
	for (int i = 0; i < this->escenario.size(); i++) {
		escenarioConcatenado += this->escenario.at(i)->getPath();
		escenarioConcatenado += ",";
		escenarioConcatenado += this->escenario.at(i)->getAncho();
		escenarioConcatenado += ",";
		escenarioConcatenado += this->escenario.at(i)->getAlto();
		escenarioConcatenado += ",";
		escenarioConcatenado += this->escenario.at(i)->getZIndex();
		escenarioConcatenado += "|";
	}
	escenarioConcatenado += "]-";
	return escenarioConcatenado;

}

vector<ImagenDto*> XmlParser::getEscenario() {
	if (this->escenario.empty()) {
		for (pugi::xml_node image = this->rootNode.child("Escenario").first_child(); image; image = image.next_sibling()) {
			ImagenDto* imagen = new ImagenDto(string(image.child_value("Archivo")), string(image.child_value("Ancho")), string(image.child_value("Alto")), string(image.child_value("z-index")));
			this->escenario.push_back(imagen);
		}

		ImagenDto* imagenTemporal;

		for (int i = 0; i < this->escenario.size(); i++) {
			for (int j = 0; j < this->escenario.size() - 1; j++) {
				if (this->escenario.at(j)->getZIndex() > this->escenario.at(j+1)->getZIndex()) {
					imagenTemporal = this->escenario.at(j);
					this->escenario.at(j) = this->escenario.at(j+1);
					this->escenario.at(j+1) = imagenTemporal;
				}
			}
		}

	}

	return this->escenario;
}


bool XmlParser::stringTerminaCon(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

string XmlParser::serializarSprites(vector<SpriteDto*> sprites, vector<string> colores) {
	string spriteConcatenado="";
	for (int i = 0; i < sprites.size(); i++)
	{
		for (int j = 0; j < colores.size(); j++)
		{
			spriteConcatenado += sprites.at(i)->getID();
			if (stringTerminaCon(sprites.at(i)->getID(),"_"))
			{
				spriteConcatenado += colores.at(j);
			}
			spriteConcatenado += ",";
			spriteConcatenado += sprites.at(i)->getCantidadDeFotogramas();
			spriteConcatenado += ",";
			spriteConcatenado += sprites.at(i)->getAncho();
			spriteConcatenado += ",";
			spriteConcatenado += sprites.at(i)->getAlto();
			spriteConcatenado += ",";
			spriteConcatenado += sprites.at(i)->getPath();
			if (stringTerminaCon(sprites.at(i)->getPath(),"_"))
			{
				spriteConcatenado += colores.at(j);
			}
			spriteConcatenado += ",";
			spriteConcatenado += sprites.at(i)->getZIndex();
			spriteConcatenado += ";";
		}
	}
	return spriteConcatenado;
}
string XmlParser::serializarSetDeSprites(vector<string> colores){
	this->getSprites();
	string setDeSpritesconcatenados = "SetSprites[";
	for (int i = 0; i < this->sprites.size(); i++) {
		setDeSpritesconcatenados += this->sprites.at(i)->getCarpeta();
		setDeSpritesconcatenados += ";";
		setDeSpritesconcatenados += this->serializarSprites(this->sprites.at(i)->getSprites(),colores);
		setDeSpritesconcatenados += "|-";
	}
	setDeSpritesconcatenados += "]";
	return setDeSpritesconcatenados;
}

vector<SetDeSpritesDto*> XmlParser::getSprites() {
	if (this->sprites.empty()) {
		for (pugi::xml_node set = this->rootNode.child("Sprites").child("SetColor"); set; set = set.next_sibling()) {
			vector<SpriteDto*> sprites;
			for (pugi::xml_node sprite = set.child("Sprite1"); sprite; sprite = sprite.next_sibling()) {
				SpriteDto* spriteDto = new SpriteDto(string(sprite.child_value("Id")),string(sprite.child_value("CantFotogramas")), string(sprite.child_value("Ancho")), string(sprite.child_value("Alto")), string(sprite.child_value("Imagen")), string(sprite.child_value("z-index")));
				sprites.push_back(spriteDto);
			}
			SetDeSpritesDto* setDeSprites = new SetDeSpritesDto(set.child_value("Carpeta"), sprites);
			this->sprites.push_back(setDeSprites);
		}
	}

	return this->sprites;
}

vector<Item*> XmlParser::getItemArmas()
{
	if (this->itemArmas.empty()) {
		for (pugi::xml_node image = this->rootNode.child("ItemArmas").first_child(); image; image = image.next_sibling()) {
			Item* item = new Item(string(image.child_value("Arma")), string(image.child_value("PosicionX")), string(image.child_value("PosicionY")));
			this->itemArmas.push_back(item);
		}
		return this->itemArmas;
	}
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

