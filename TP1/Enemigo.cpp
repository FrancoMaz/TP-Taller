/*
 * Enemigo.cpp
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#include "Enemigo.h"

Enemigo::Enemigo(int posX, int posY, int id, int estado) {
	this->vida = 100;
	this->armas.push_back(new HeavyMachineGun());
	this->estaMuerto = false;
	this->posX = posX;
	this->posY = posY;
	this->id = id;
	this->boxCollider = {posX+86,posY,49,106};
	this->threadAsociado = false;
	this->condicionSprite = "Normal";
	this->anguloBala = 0;
	this->agachado = false;
	if (estado == 1)
	{
		this->spriteEnemigo = "2"; //"Enemigo_disparando"
		this->disparar = true;
	}
	else if (estado == 0)
	{
		this->spriteEnemigo = "0"; //Enemigo_quieto"
		this->disparar = false;
	}
	else if (estado == 2)
	{
		this->spriteEnemigo = "8"; //Enemigo quieto en el paracaidas
		this->disparar = false;
		this->boxCollider.h = 197;
	}
	this->estado = estado;
}

Enemigo::~Enemigo() {
	/*for (int i = 0; i < this->armas.size(); i++) {
		this->armas.at(i)->~Arma();
	}*/
}

string Enemigo::getInformacionDelEnemigo() {
	return (to_string(this->posX) + "|" + to_string(this->posY) + "|" + this->spriteEnemigo + "|" + to_string(this->id) + "|" + this->condicionSprite + "#");
}

int Enemigo::getId() {
	return this->id;
}

SDL_Rect Enemigo::getBoxCollider() {
	return this->boxCollider;
}

int Enemigo::getPosX() {
	return this->posX;
}

Proyectil* Enemigo::dispararProyectil() {
	// le paso un 2 al final para indicar que el proyectil fue disparado por un jugador
	string sentidoBala = "";
	if (condicionSprite == "Normal")
	{
		sentidoBala = "Espejado";
	}
	else
	{
		sentidoBala = "Normal";
	}
	return new Proyectil(20, 15, "0", posX + 25, posY + 10, sentidoBala, anguloBala, {posX + 25, posY + 10, 22, 13}, false, 2, "", make_pair(0,0));
}

string Enemigo::getNombre() {
	return "Enemigo";
}

int Enemigo::getPosY()
{
	return this->posY;
}
void Enemigo::setSprite(string spriteNuevo) {
	this->spriteEnemigo = spriteNuevo;
}

void Enemigo::caer()
{
	this->posY += VELOCIDADCAIDA;
}

bool Enemigo::tieneQueDejarDeCaer(vector<pair<string,string>> plataformas)
{
	if ((!this->esPlataforma(plataformas) && this->posY >= PISO - boxCollider.h) || (this->esPlataforma(plataformas) && this->posY >= PLATAFORMA - boxCollider.h))
	{
		return true;
	}
	return false;
}

bool Enemigo::esPlataforma(vector<pair<string,string>> plataformas)
{
	for (int i = 0; i < plataformas.size(); i++)
	{
		if ((this->posX + boxCollider.w >= atoi(plataformas.at(i).first.c_str())) && (this->posX <= atoi(plataformas.at(i).second.c_str())))
		{
			return true;
		}
	}
	return false;
}

void Enemigo::caminar(SDL_Rect camara)
{
	if (this->condicionSprite == "Normal")
	{
		this->posX -= VELOCIDADCAMINAR;
		this->boxCollider.x -= VELOCIDADCAMINAR;
		if (this->boxCollider.x <= camara.x)
		{
			this->condicionSprite = "Espejado";
		}
	}
	else if (this->condicionSprite == "Espejado")
	{
		this->posX += VELOCIDADCAMINAR;
		this->boxCollider.x += VELOCIDADCAMINAR;
		if (this->boxCollider.x + this->boxCollider.w >= camara.x + camara.w)
		{
			this->condicionSprite = "Normal";
		}
	}
}

void Enemigo::setId(int id)
{
	this->id = id;
}

void Enemigo::setYCaida()
{
	this->posY += 91;
	this->boxCollider.y = 91;
	this->boxCollider.h = 106;
}
