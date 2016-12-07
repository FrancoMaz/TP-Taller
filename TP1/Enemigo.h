/*
 * Enemigo.h
 *
 *  Created on: 13 nov. 2016
 *      Author: josefina
 */

#ifndef ENEMIGO_H_
#define ENEMIGO_H_
#define VELOCIDADCAIDA 4
#define VELOCIDADCAMINAR 4
#define PISO 520
#define PLATAFORMA 354

#include "HeavyMachineGun.h"
#include "Personaje.h"

class Enemigo: public Personaje {
private:
	int id;
	int posX;
	int posY;
	string spriteEnemigo;
	SDL_Rect boxCollider;
	int anguloBala;
	bool agachado;
	string sentido;

public:
	Enemigo(int posX, int posY, int id, int estado, int vida, bool esBoss);
	virtual ~Enemigo();
	int getId();
	void setId(int id);
	string getInformacionDelEnemigo();
	SDL_Rect getBoxCollider();
	int getPosX();
	int getPosY();
	bool threadAsociado;
	Proyectil* dispararProyectil(int angulo, string bala, int x, int y, int w, int h, int danio);
	string getNombre();
	int estado;
	void setSprite(string spriteNuevo);
	void caer();
	bool tieneQueDejarDeCaer(vector<pair<string,string>> plataformas);
	bool esPlataforma(vector<pair<string,string>> plataformas);
	void caminar(SDL_Rect camara, vector<pair<string,string>> plataformas);
	void setYCaida();
	pair<int,int> buscarPlataforma(vector<pair<string,string>>);
	string condicionSprite;
	bool esEnemigoBoss;
	void walk();
};

#endif /* ENEMIGO_H_ */
