/*
 * Capa.cpp
 *
 *  Created on: 24 oct. 2016
 *      Author: franco
 */

#include "Capa.h"

Capa::Capa(ImagenDto* imagen, SDL_Rect rectangulo, TexturaSDL* textura) {
	this->imagen = imagen;
	this->rectangulo = rectangulo;
	this->textura = textura;
	this->velocidad = 0;
}

Capa::~Capa() {
	// TODO Auto-generated destructor stub
}

void Capa::paralajeInfinito(int anchoVentana, int indice)
{
	if (indice == 0)
	{
		this->paralajeInfinitoCapaPrincipal(anchoVentana);
	}
	else {
		this->paralajeInfinitoOtrasCapas(anchoVentana);
	}

}

void Capa::paralajeInfinitoCapaPrincipal(int anchoVentana)
{
	if((this->rectangulo.x + anchoVentana) > atoi(this->imagen->getAncho().c_str())){
		if(this->rectangulo.x < atoi(this->imagen->getAncho().c_str())){
			this->rectangulo.w = atoi(this->imagen->getAncho().c_str()) - this->rectangulo.x;
			this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);

			this->rectangulo.w = anchoVentana - (atoi(this->imagen->getAncho().c_str()) - this->rectangulo.x);
			this->rectangulo.x = (atoi(this->imagen->getAncho().c_str()) - this->rectangulo.w);
			this->textura->aplicarPosicionDePorcion((anchoVentana - this->rectangulo.w),0,&this->rectangulo,0,SDL_FLIP_HORIZONTAL);
		} else {
			this->rectangulo.w = anchoVentana;
			this->rectangulo.x = (atoi(this->imagen->getAncho().c_str()) - anchoVentana) - (this->rectangulo.x - atoi(this->imagen->getAncho().c_str()));

			if(this->rectangulo.x < 0){
				this->rectangulo.w = anchoVentana + this->rectangulo.x;
				this->rectangulo.x = 0;
				this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_HORIZONTAL);

				this->rectangulo.w = anchoVentana - (this->rectangulo.w);
				this->textura->aplicarPosicionDePorcion((anchoVentana - this->rectangulo.w),0,&this->rectangulo,0,SDL_FLIP_NONE);

				if (this->rectangulo.w > anchoVentana){
					//camara.x = 0;
					this->rectangulo.w = anchoVentana;
					this->rectangulo.x = 0;
					//jugador_X = ANCHO_VENTANA/2 - texturaJugador->getAnchoSprite()/2;
				}
			} else {
				this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_HORIZONTAL);
			}
		}
	} else {
		this->rectangulo.w = anchoVentana;
		this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);
	}
}

void Capa::paralajeInfinitoOtrasCapas(int anchoVentana)
{
	if ((this->rectangulo.x + anchoVentana) > atoi(this->imagen->getAncho().c_str()))
	{
		int posicionAuxiliar = 0;
		this->rectangulo.w = atoi(this->imagen->getAncho().c_str()) - this->rectangulo.x;
		this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);
		posicionAuxiliar = this->rectangulo.x;

		this->rectangulo.w = anchoVentana - (atoi(this->imagen->getAncho().c_str()) - this->rectangulo.x);
		this->rectangulo.x = 0;
		this->textura->aplicarPosicionDePorcion(anchoVentana - this->rectangulo.w,0,&this->rectangulo,0,SDL_FLIP_NONE);
		this->rectangulo.x = posicionAuxiliar;

		if (this->rectangulo.x > atoi(this->imagen->getAncho().c_str()))
		{
			this->rectangulo.x = 0;
			this->velocidad = 0;
			this->rectangulo.w = anchoVentana;
		}
	} else {
		this->textura->aplicarPosicionDePorcion(0,0,&this->rectangulo,0,SDL_FLIP_NONE);
	}
}

void Capa::modificarRectangulo(int vel, int anchoVentana, int anchoEscenario)
{

	this->velocidad += vel;
	this->rectangulo.x = abs((this->velocidad)*(atoi(this->imagen->getAncho().c_str()) - anchoVentana)/(anchoEscenario - anchoVentana));
}
