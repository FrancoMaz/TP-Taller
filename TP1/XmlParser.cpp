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
	//this->escenario = new list<ImagenDto>();
	//this->sprites = new list<SetDeSpritesDto*>();
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

pair<const char*, const char*> XmlParser::getTamanioVentana() {
	if (this->tamanioVentana.first == "" && this->tamanioVentana.second == "") {
		pair<const char*,const char*> dimensiones(this->rootNode.child("Ventana").child_value("Ancho"), this->rootNode.child("Ventana").child_value("Alto"));
		this->tamanioVentana = dimensiones;
	}
	return this->tamanioVentana;
}

string XmlParser::serializarEscenario(){
	this->getEscenario();
	string escenarioConcatenado = "Escenario[";
	for (int i = 0; i < this->escenario.size(); i++) {
		escenarioConcatenado += this->escenario.at(i)->getPath();
		//cout << (*i).getPath() << endl;
		escenarioConcatenado += ",";
		escenarioConcatenado += this->escenario.at(i)->getAncho();
		//cout << (*i).getZIndex() << endl;
		escenarioConcatenado += ",";
		escenarioConcatenado += this->escenario.at(i)->getAlto();
		//cout << (*i).getVelocidad() << endl;
		escenarioConcatenado += "|";
	}
	//string escenarioC = escenarioConcatenado.substr(0, escenarioConcatenado.length() -1);
	escenarioConcatenado += "]-";
	return escenarioConcatenado;

}

vector<ImagenDto*> XmlParser::getEscenario() {
	if (this->escenario.empty()) {
		for (pugi::xml_node image = this->rootNode.child("Escenario").first_child(); image; image = image.next_sibling()) {
			ImagenDto* imagen = new ImagenDto(image.child_value("Archivo"), image.child_value("Ancho"), image.child_value("Alto"));
			this->escenario.push_back(imagen);
		}
	}

	return this->escenario;
}

string XmlParser::serializarSprites(vector<SpriteDto*> sprites) {
	string spriteConcatenado="";
	for (int i = 0; i < sprites.size(); i++)
	{
		spriteConcatenado += sprites.at(i)->getId();
		spriteConcatenado += ",";
		spriteConcatenado += sprites.at(i)->getCantidadDeFotogramas();
		spriteConcatenado += ",";
		spriteConcatenado += sprites.at(i)->getAncho();
		spriteConcatenado += ",";
		spriteConcatenado += sprites.at(i)->getAlto();
		spriteConcatenado += ",";
		spriteConcatenado += sprites.at(i)->getPath();
		spriteConcatenado += ",";
		spriteConcatenado += sprites.at(i)->getZIndex();
		spriteConcatenado += ";";
	}
	return spriteConcatenado;
}
string XmlParser::serializarSetDeSprites(){
	this->getSprites();
	string setDeSpritesconcatenados = "SetSprites[";
	for (int i = 0; i < this->sprites.size(); i++) {
		setDeSpritesconcatenados += this->sprites.at(i)->getCarpeta();
		setDeSpritesconcatenados += ";";
		setDeSpritesconcatenados += this->serializarSprites(this->sprites.at(i)->getSprites());
		//setDeSpritesconcatenados = setDeSpritesconcatenados.substr(0, setDeSpritesconcatenados.length() -1);
		setDeSpritesconcatenados += "|-";
	}
	//setDeSpritesconcatenados = setDeSpritesconcatenados.substr(0, setDeSpritesconcatenados.length() -1);
	setDeSpritesconcatenados += "]";
	return setDeSpritesconcatenados;
}

vector<SetDeSpritesDto*> XmlParser::getSprites() {
	if (this->sprites.empty()) {
		for (pugi::xml_node set = this->rootNode.child("Sprites").child("SetRojo"); set; set = set.next_sibling()) {
			vector<SpriteDto*> sprites;
			for (pugi::xml_node sprite = set.child("SpriteSalto"); sprite; sprite = sprite.next_sibling()) {
				SpriteDto* spriteDto = new SpriteDto(string(sprite.child_value("Id")),string(sprite.child_value("CantFotogramas")), string(sprite.child_value("Ancho")), string(sprite.child_value("Alto")), sprite.child_value("Imagen"), sprite.child_value("z-index"));
				sprites.push_back(spriteDto);
			}
			SetDeSpritesDto* setDeSprites = new SetDeSpritesDto(set.child_value("Carpeta"), sprites);
			this->sprites.push_back(setDeSprites);
		}
	}

	return this->sprites;
}
void XmlParser::mostrarSprites(SpriteDto* sprite){

	cout<<"Id: "<<sprite->getId()<<endl;
	cout<<"cantFotogramas: "<<sprite->getCantidadDeFotogramas()<<endl;
	cout<<"Ancho: "<<sprite->getAncho()<<endl;
	cout<<"Alto: "<<sprite->getAlto()<<endl;
	cout<<"Imagen: "<<sprite->getPath()<<endl;
	cout<<"Zindex: "<<sprite->getZIndex()<<endl;
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

