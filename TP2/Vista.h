/*
 * Vista.h
 *
 *  Created on: 30 sep. 2016
 *      Author: pablo
 */

#ifndef TP2_VISTA_H_
#define TP2_VISTA_H_

#include "VentanaSDL.h"
#include "TexturaSDL.h"
#include <SDL2/SDL.h>
#include <vector>

using namespace std;

class Vista {
private:
	VentanaSDL* ventana;
	vector<TexturaSDL*> textura;

public:
	Vista();
	~Vista();
	void cargarPrimeraPantalla();
};

#endif /* TP2_VISTA_H_ */
