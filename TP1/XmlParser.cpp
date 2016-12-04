/*
 * XmlParser.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: josefina
 */

#include "XmlParser.h"
using namespace std;

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
		if (stringTerminaCon(sprites.at(i)->getID(),"_"))
		{
			for (int j = 0; j < colores.size(); j++)
			{
				spriteConcatenado += sprites.at(i)->getID();
				spriteConcatenado += colores.at(j);
				spriteConcatenado += ",";
				spriteConcatenado += sprites.at(i)->getCantidadDeFotogramas();
				spriteConcatenado += ",";
				spriteConcatenado += sprites.at(i)->getAncho();
				spriteConcatenado += ",";
				spriteConcatenado += sprites.at(i)->getAlto();
				spriteConcatenado += ",";
				spriteConcatenado += sprites.at(i)->getPath();
				spriteConcatenado += colores.at(j);
				spriteConcatenado += ",";
				spriteConcatenado += sprites.at(i)->getZIndex();
				spriteConcatenado += ";";
			}
		}
		else
		{
			spriteConcatenado += sprites.at(i)->getID();
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
		for (pugi::xml_node item = this->rootNode.child("ItemArmas").first_child(); item; item = item.next_sibling()) {
			Item* itemArma = new Item(string(item.child_value("Arma")), string(item.child_value("PosicionX")), string(item.child_value("PosicionY")),0,"");
			this->itemArmas.push_back(itemArma);
		}
	}
	return this->itemArmas;
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

vector<pair<string,string>> XmlParser::getPlataformas()
{
	if (this->plataformas.empty()) {
		for (pugi::xml_node plataforma = this->rootNode.child("Plataformas").first_child(); plataforma; plataforma = plataforma.next_sibling()) {
			pair<string,string> plat;
			plat.first = string(plataforma.child_value("Comienzo"));
			plat.second = string(plataforma.child_value("Final"));
			this->plataformas.push_back(plat);
		}
	}
	return this->plataformas;
}

Boss* XmlParser::devolverBoss(string nombre, string x, string y)
{
	if (nombre == "hi_do")
	{
		return new HiDo(nombre, x, y);
	}
	else if (nombre == "tani_oh")
	{
		return new TaniOh(nombre, x, y);
	}
	else if (nombre == "airbuster_riberts")
	{
		return new AirbusterRiberts(nombre, x, y);
	}
	return NULL;
}

Boss* XmlParser::getBoss()
{
	string nombre = string(this->rootNode.child("Boss").child_value("Nombre"));
	string x = string(this->rootNode.child("Boss").child_value("PosicionX"));
	string y = string(string(this->rootNode.child("Boss").child_value("PosicionY")));
	Boss* boss = devolverBoss(nombre, x, y);
	this->boss = boss;
	return this->boss;
}

vector<string> XmlParser::getCapas()
{
	vector<string> capas;
	capas.push_back(string(this->rootNode.child("Capas").child_value("Capa1")));
	capas.push_back(string(this->rootNode.child("Capas").child_value("Capa2")));
	capas.push_back(string(this->rootNode.child("Capas").child_value("Capa3")));
	capas.push_back(string(this->rootNode.child("Capas").child_value("Capa4")));
	return capas;
}

vector<pair<int,int>> XmlParser::getEnemigos()
{
	vector<pair<int,int>> enemigos;
	for (pugi::xml_node enemy = this->rootNode.child("Enemigos").first_child(); enemy; enemy = enemy.next_sibling())
	{
		pair<int,int> coordenadas;
		const char* x = enemy.child_value("PosicionX");
		const char* y = enemy.child_value("PosicionY");
		coordenadas.first = atoi(x);
		coordenadas.second = atoi(y);
		enemigos.push_back(coordenadas);
	}
	return enemigos;
}
