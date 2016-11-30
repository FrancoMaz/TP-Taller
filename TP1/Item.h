/*
 * Item.h
 *
 *  Created on: 24 nov. 2016
 *      Author: franco
 */

#ifndef ITEM_H_
#define ITEM_H_

#include <string>
#include <SDL2/SDL.h>
#include "TexturaSDL.h"
using namespace std;

class Item {
private:
	string sprite;
public:
	Item(string nombre, string x, string y, int idItem);
	Item(int x, int y, TexturaSDL* textura);
	virtual ~Item();
	string getStringItem();
	bool visto;
	SDL_Rect boxCollider;
	TexturaSDL* textura;
	string nombre;
	bool fueObtenido;
	int idItem;
};

#endif /* ITEM_H_ */
