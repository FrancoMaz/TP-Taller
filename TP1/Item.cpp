/*
 * Item.cpp
 *
 *  Created on: 24 nov. 2016
 *      Author: franco
 */

#include "Item.h"

Item::Item(string nombre, string x, string y, int idItem, string sprite) {
	this->nombre = nombre;
	this->boxCollider = {atoi(x.c_str()),atoi(y.c_str()),48,44};
	//this->sprite = "Item_" + this->nombre;
	if (this->nombre == "HeavyMachineGun") {
		this->sprite = "0";
	} else if (this->nombre == "RocketLauncher") {
		this->sprite = "1";
	} else if(this->nombre == "FlameShot") {
		this->sprite = "2";
	} else {
		this->sprite = sprite;
	}

	visto = false;
	fueObtenido = false;
	this->idItem = idItem;
}


Item::Item(int x, int y, TexturaSDL* textura) {
	this->sprite = sprite;
	this->boxCollider = {x,y,48,44};
	this->textura = textura;
}
Item::~Item() {
	delete this->textura;
}

string Item::getStringItem()
{
	return (this->sprite + "|" + to_string(this->boxCollider.x) + "|" + to_string(this->boxCollider.y) + "#");
}
