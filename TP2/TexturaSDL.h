/*
 * TexturaSDL.h
 *
 *  Created on: 21 sep. 2016
 *      Author: pablo
 */

#ifndef TP2_TEXTURASDL_H_
#define TP2_TEXTURASDL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class TexturaSDL {

private:
	SDL_Texture* textura;
	SDL_Renderer* render;
	vector<SDL_Rect> spriteClips;
	int frameActual;
	int ancho;
	int alto;

	void aplicarPosicionYTamanio(float x, float y, int ancho, int alto, double rotacion, SDL_RendererFlip flip);

public:
	TexturaSDL(SDL_Renderer* renderer);
	~TexturaSDL();

	//Elimina las referencias de la textura, liberando memoria
	void limpiar();

	//Carga una imagen de una ruta específica
	bool cargarImagen(string ruta);

	//Coloca la textura en una posición de la pantalla
	void aplicarPosicion(float x, float y, double rotacion, SDL_RendererFlip flip);

	//Idem, pero dicha textura se ajusta al tamaño del ancho/alto
	void aplicarPosicionConTamanio(float x, float y, int ancho, int alto);

	//Establecer transparencias (alpha = 0 transparente; alpha = 255 opaco)
	void setAlpha(Uint8 alpha);

	//Genera un sprite con la cantidad de fotogramas (frames) que se le pasa por argumento
	void generarSprite(int frames);

	//Resetea el fotograma del sprite a cero
	void frameReset();

	//Obtener las dimensiones
	int getAncho();
	int getAlto();
};

#endif /* TP2_TEXTURASDL_H_ */
