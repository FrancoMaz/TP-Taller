/*
 * Item.cpp
 *
 *  Created on: 24 nov. 2016
 *      Author: franco
 */

#include "Item.h"

Item::Item(string arma, string x, string y) {
	this->arma = arma;
	this->boxCollider = {atoi(x.c_str()),atoi(y.c_str()),48,44};
	this->sprite = "Item_" + this->arma;
	visto = false;
	fueObtenido = false;
}


Item::Item(int x, int y, TexturaSDL* textura) {
	this->sprite = sprite;
	this->boxCollider = {x,y,48,44};
	this->textura = textura;
}
Item::~Item() {
	// TODO Auto-generated destructor stub
}

string Item::getStringItem()
{
	return (this->sprite + "|" + to_string(this->boxCollider.x) + "|" + to_string(this->boxCollider.y) + "#");
}
