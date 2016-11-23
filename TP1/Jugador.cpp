/*
 * Jugador.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#include "Jugador.h"
#include <iostream>

Jugador::Jugador(string nombre, string equipo, int posicionX, vector<pair<int,int>> vectorPlataforma) {
	this->nombre = nombre;
	this->equipo = equipo;
	posicion.first = posicionX;
	posicion.second = PISO;
	velocidades.first = 0;
	velocidades.second = 0;
	saltar = false;
	agachar = false;
	disparar = false;
	caer = false;
	angulo = 0;
	condicionSprite = "Normal";
	spriteAEjecutar = "Jugador_" + equipo;
	this->vectorPlataforma = vectorPlataforma;
	boxCollider = {posicion.first+MARGENBOXCOLLIDER,posicion.second+114,50,106};
	this->inicializarVectorArmas();
}

Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::mover(SDL_Rect camara){
	if (velocidades.first > VELMAX)
	{
		velocidades.first = VELMAX;
	}
	else if (velocidades.first < -VELMAX)
	{
		velocidades.first = -VELMAX;
	}
	if (movDerecha)
	{
		if (!agachar){
			velocidades.first += velocidad;
			if (!saltar) {
				spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
			}
			else {
				spriteAEjecutar = "Jugador_saltando_" + this->equipo;
			}
		}
		condicionSprite = "Normal";
	}
	else if (movIzquierda)
	{
		if (!agachar){
			velocidades.first -= velocidad;
			if (!saltar) {
				spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
			}
			else {
					spriteAEjecutar = "Jugador_saltando_" + this->equipo;
				}
		}
		condicionSprite = "Espejado";
	}
	else if (disparar)
	{
		if(!saltar && (posicion.second == PISO || posicion.second == PLATAFORMA)){
			velocidades.first = 0;
			if(!agachar){
				spriteAEjecutar = "Jugador_disparando_" + this->equipo;
			} else {
				spriteAEjecutar = "Jugador_agachado_disparando_" + this->equipo;
			}
		}
		//this->dispararProyectil();
	}
	else if (agachar)
	{
		if (!saltar && !disparar && (posicion.second == PISO || posicion.second == PLATAFORMA))
			{
				velocidades.first = 0;
				spriteAEjecutar = "Jugador_agachado_" + this->equipo;
			}
	}
	else if (saltar)
	{
		if(angulo == 0){
			saltar = true;
		}
		spriteAEjecutar = "Jugador_saltando_" + this->equipo;
	}
	else if(!movDerecha && ultimaTeclaPresionada == SDLK_RIGHT)
	{
		velocidades.first = 0;
		if (!saltar) {
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
		condicionSprite = "Normal";
	}
	else if(!movIzquierda && ultimaTeclaPresionada == SDLK_LEFT)
	{
		velocidades.first = 0;
		if (!saltar)
		{
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
		condicionSprite = "Espejado";
	}
	else if(!agachar && ultimaTeclaPresionada == SDLK_DOWN)
	{
		velocidades.first = 0;
		if (!saltar  && (posicion.second == PISO || posicion.second == PLATAFORMA))
		{
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
	}
	else if(!disparar && ultimaTeclaPresionada == SDLK_SPACE)
	{
		if (!saltar && (posicion.second == PISO || posicion.second == PLATAFORMA))
		{
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
	}

	cout << "Saltar: " << saltar << endl;
	cout << "Caer: " << caer << endl;
	cout << "Disparar: " << disparar << endl;
	cout << "Agachar: " << agachar << endl;
	cout << "MovIzquierda: " << movIzquierda << endl;
	cout << "MovDerecha: " << movDerecha << endl;
	cout << "Angulo: " << angulo << endl;
	cout << "Posicion jugador/boxCollider: x: " << posicion.first << "/" << boxCollider.x << " ,y: " << posicion.second << "/" << boxCollider.y << endl;
	cout << "Alto/Ancho boxCollider: w: " << boxCollider.w << " ,h: " << boxCollider.h << endl;

	if (posicion.second < PISO && !this->esPlataforma(boxCollider.x) && !saltar && (ultimaTeclaPresionada != SDLK_DOWN || ultimaTeclaPresionada != SDLK_SPACE))
	{
		caer = true;
		spriteAEjecutar = "Jugador_saltando_" + this->equipo;
	}
	if(saltar)
	{
		velocidades.second = -21*cos(angulo);
		angulo += PI/25;
		if (angulo > (PI + (PI/25)) || (posicion.second == PLATAFORMA && this->esPlataforma(boxCollider.x) && angulo > PI/2)){
			angulo = 0;
			saltar = false;
			if(!agachar){
				posicion.second = posicion.second + 6;
				if (velocidades.first != 0) {
					spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
				}
				else {
					spriteAEjecutar = "Jugador_" + this->equipo;
				}
			}
		}
	} else
	{
		velocidades.second = 0;
	}
	if (caer)
	{
		velocidades.second = GRAVEDAD;
		if (posicion.second == PISO)
		{
			caer = false;
			velocidades.second = 0;
			if (velocidades.first != 0) {
				spriteAEjecutar = "Jugador_corriendo_" + this->equipo;
			}
			else {
				spriteAEjecutar = "Jugador_" + this->equipo;
			}
		}
	}

	posicion.first += velocidades.first;
	posicion.second += velocidades.second;

	//Establezco el boxCollider
	boxCollider.x = posicion.first+MARGENBOXCOLLIDER;
	if(!agachar){
		boxCollider.y = posicion.second + 114;
		boxCollider.h = 106;
	} else {
		boxCollider.y = posicion.second + 154;
		boxCollider.h = 66;
	}

	if (posicion.second >= PISO)
	{
		posicion.second = PISO;
	}
	else if (posicion.second >= PLATAFORMA && this->esPlataforma(boxCollider.x) && !saltar && !caer)
	{
		posicion.second = PLATAFORMA;
	}

	if (boxCollider.x-MARGENIZQ < camara.x or boxCollider.x + boxCollider.w > camara.x + ANCHO_VENTANA - MARGENIZQ)
	{
		posicion.first -= velocidades.first;
		boxCollider.x = posicion.first+MARGENBOXCOLLIDER;
	}
}

Proyectil* Jugador::dispararProyectil()
{
	if (!this->armas.at(this->armaActual)->sinMuniciones())
	{
		return (this->armas.at(this->armaActual)->disparar(boxCollider));
	}
	else
	{
		this->armaActual += 1;
		if (this->armaActual >= this->armas.size())
		{
			this->armaActual = 0;
		}
		return (this->armas.at(this->armaActual)->disparar(boxCollider));
	}
}

bool Jugador::esPlataforma(int x)
{
	for (int i = 0; i < vectorPlataforma.size(); i++)
	{
		if ((x+boxCollider.w >= vectorPlataforma.at(i).first) && (x <= vectorPlataforma.at(i).second))
		{
			return true;
		}
	}
	return false;
}

void Jugador::setPosicion(int x, int y)
{
	posicion.first = x;
	posicion.second = y;
}

void Jugador::setConectado()
{
	this->conectado = true;
}

void Jugador::setDesconectado()
{
	this->conectado = false;
}

string Jugador::getStringJugador()
{
	return ("0|" + nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + spriteAEjecutar + "|" + condicionSprite + "#");
}

string Jugador::getNombre()
{
	return this->nombre;
}

string Jugador::getSpriteAEjecutar()
{
	return this->spriteAEjecutar;
}

bool Jugador::salto()
{
	return this->saltar;
}

bool Jugador::getConectado(){
	return conectado;
}

string Jugador::serializarInicio()
{
	return (nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + this->spriteAEjecutar);
}

bool Jugador::chequearCambiarCamara(SDL_Rect camara, int anchoVentana, pair<int,int> posicionesExtremos, int anchoSprite)
{

	int posicionMasAtras = posicionesExtremos.first;
	int posicionMasAdelante = posicionesExtremos.second;
	if (camara.x < 0){
		camara.x = 0;
		return false;
	}
	if (camara.x >= posicionMasAtras)
	{
		camara.x = posicionMasAtras;
		return false;
	}

	if (this->velocidades.first <= 0)
	{
		return false;
	}
 	if(this->boxCollider.x - camara.x > anchoVentana/2 && posicionMasAtras >= (camara.x + MARGENIZQ))
 	{
 		return true;
 	}
	return false;
}

pair<int,int> Jugador::getPosicion()
{
	return posicion;
}

int Jugador::getVelocidadX()
{
	return this->velocidades.first;
}

void Jugador::resetearPosicion(int anchoCapaPrincipal)
{
	posicion.first = posicion.first - anchoCapaPrincipal;
}

void Jugador::setPosicion(int posicion)
{
	this->posicion.first = posicion;
}

void Jugador::setSprite(string sprite, bool equipo, string condicion)
{
	spriteAEjecutar = sprite;
	if (equipo && condicion != "")
	{
		spriteAEjecutar = sprite + "_" + this->equipo;
		condicionSprite = condicion;
	}
}

vector<bool> Jugador::getMov(){
	vector<bool> movimiento;
	movimiento.at(DER) = movDerecha;
	movimiento.at(IZQ) = movIzquierda;
	movimiento.at(ARRIBA) = saltar;
	movimiento.at(ABAJO) = agachar;
	movimiento.at(ESPACIO) = disparar;
	return movimiento;
}

void Jugador::setMov(SDL_Keycode tecla, bool sePresiono){
	this->ultimaTeclaPresionada = tecla;
	if (tecla == SDLK_RIGHT){
		movDerecha = sePresiono;
	}
	else if (tecla == SDLK_LEFT){
		movIzquierda = sePresiono;
	}
	else if (tecla == SDLK_UP)
	{
		saltar = sePresiono;
	}
	else if (tecla == SDLK_DOWN)
	{
		agachar = sePresiono;
	}
	else if (tecla == SDLK_SPACE)
	{
		disparar = sePresiono;
	}
}

string Jugador::getEquipo()
{
	return this->equipo;
}

pthread_t Jugador::getThreadMovimiento()
{
	return thrMov;
}

void Jugador::setThreadMovimiento(pthread_t thrMovimiento){
	thrMov = thrMovimiento;
}

void Jugador::inicializarVectorArmas()
{
	this->armas.push_back(new HeavyMachineGun());
	this->armas.push_back(new RocketLauncher());
	this->armas.push_back(new Flameshot());
	this->armaActual = 0;
}

bool Jugador::estaDisparando(){
	return this->disparar;
}

Proyectil* Jugador::getProyectilDisparado()
{
	return this->armas.at(this->armaActual)->getProyectil();
}
