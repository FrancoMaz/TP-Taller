/*
 * Jugador.cpp
 *
 *  Created on: 8 oct. 2016
 *      Author: franco
 */

#include "Jugador.h"
#include <iostream>

Jugador::Jugador(string nombre, string equipo, int posicionX, vector<pair<string,string>> vectorPlataforma) {
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
	this->armasVacias = false;
	this->vida = 100;
	this->anguloBala = 0; //0 equivale al angulo 0, 1 al angulo PI/2, 2 al angulo PI/4
	this->estaMuerto = false;
	this->puntaje = 0;
	this->puntajeTotal = 0;
	this->subirPlataforma = false;
	if (this->equipo == "rojo"){
		equipoIndice = 0;
	}
	else if (this->equipo == "verde"){
		equipoIndice = 1;
	}
	else if (this->equipo == "amarillo"){
		equipoIndice = 2;
	}
	else if (this->equipo == "azul"){
		equipoIndice = 3;
	}
}

Jugador::~Jugador() {
	// TODO Auto-generated destructor stub
}

void Jugador::condicionesMovimiento()
{
	if (!saltar && !arriba && !disparar)
	{
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_" + this->equipo;
	}
	else if (!arriba && disparar && !saltar)
	{
		if (!this->armasVacias) {
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_disparando_" + this->equipo;
			this->anguloBala = 0;
		}
		else {
			spriteAEjecutar = "Jugador_" + this->armas.at(armaActual)->nombre + "corriendo_" + this->equipo;
		}
	}
	else if (arriba && !disparar && !saltar)
	{
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_mirando_arriba_" + this->equipo;
	}
	else if (arriba && disparar && !this->armas.at(armaActual)->disparoDiagonal && !saltar)
	{
		if (!this->armasVacias) {
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_disparando_arriba_" + this->equipo;
			this->anguloBala = 1;
		}
		else
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_mirando_arriba_" + this->equipo;
		}
	}
	else if (arriba && disparar && this->armas.at(armaActual)->disparoDiagonal && !saltar)
	{
		if (!this->armasVacias) {
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_disparando_diagonal_" + this->equipo;
			this->anguloBala = 2;
		}
		else
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_mirando_arriba_" + this->equipo;
		}
	}
	else if (saltar)
	{
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "saltando_" + this->equipo;
	}
}

void Jugador::mover(SDL_Rect camara){
	this->idSprite = obtenerIdSpriteAEjecutar();
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
		if (!agachar)
		{
			velocidades.first += velocidad;
			this->condicionesMovimiento();
		}
		condicionSprite = "Normal";
	}
	else if (movIzquierda)
	{
		if (!agachar){
			velocidades.first -= velocidad;
			this->condicionesMovimiento();
		}
		condicionSprite = "Espejado";
	}
	else if (disparar && !arriba) {
		if (!this->armasVacias) {
			if(!saltar && !movDerecha && !movIzquierda && (posicion.second == PISO || posicion.second == PLATAFORMA)){
				velocidades.first = 0;
				if(!agachar){
					spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "disparando_" + this->equipo;
					this->anguloBala = 0;
				} else {
					spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "agachado_disparando_" + this->equipo;
					this->anguloBala = 0;
				}
			}
		} else {
			spriteAEjecutar = "Jugador_" + this->equipo;
		}
	}
	else if (agachar)
	{
		velocidades.first = 0;
		if (!saltar && !disparar && (posicion.second == PISO || posicion.second == PLATAFORMA))
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "agachado_" + this->equipo;
		}
		else if (!saltar && disparar && (posicion.second == PISO || posicion.second == PLATAFORMA))
		{
			if (!armasVacias)
			{
				spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "agachado_disparando_" + this->equipo;
				this->anguloBala = 0;
			}
			else
			{
				spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "agachado_" + this->equipo;
			}
		}
	}
	else if (saltar)
	{
		if(angulo == 0){
			saltar = true;
		}
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "saltando_" + this->equipo;
	}
	else if (arriba && !disparar && !agachar && !movDerecha && !movIzquierda)
	{
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "mirando_arriba_" + this->equipo;
	}
	else if (arriba && disparar && !agachar && !movDerecha && !movIzquierda)
	{
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "disparando_arriba_" + this->equipo;
		this->anguloBala = 1;
	}
	else if(!movDerecha && ultimaTeclaPresionada == SDLK_RIGHT)
	{
		velocidades.first = 0;
		if (!saltar) {
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
		}
		condicionSprite = "Normal";
	}
	else if(!movIzquierda && ultimaTeclaPresionada == SDLK_LEFT)
	{
		velocidades.first = 0;
		if (!saltar)
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
		}
		condicionSprite = "Espejado";
	}
	else if(!agachar && ultimaTeclaPresionada == SDLK_DOWN)
	{
		velocidades.first = 0;
		if (!saltar  && (posicion.second == PISO || posicion.second == PLATAFORMA))
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
		}
	}
	else if(!disparar && ultimaTeclaPresionada == SDLK_x)
	{
		if (!saltar && (posicion.second == PISO || posicion.second == PLATAFORMA))
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
		}
	}

	else if (!arriba && ultimaTeclaPresionada == SDLK_UP)
	{
		velocidades.first = 0;
		if (!saltar)
		{
			spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
		}
	}

	if (posicion.second < PISO && !this->esPlataforma(boxCollider.x) && !saltar && (ultimaTeclaPresionada != SDLK_DOWN || ultimaTeclaPresionada != SDLK_x))
	{
		caer = true;
		spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "saltando_" + this->equipo;
	}
	if(saltar)
	{
		velocidades.second = -21*cos(angulo);
		angulo += PI/25;
		if (posicion.second <= PLATAFORMA + ((PISO+PLATAFORMA)/4) && !this->esPlataforma(boxCollider.x))
		{
			this->subirPlataforma = true;
		}
		if (angulo > (PI + (PI/25)) || (posicion.second == PLATAFORMA && this->esPlataforma(boxCollider.x) && angulo > PI/2 && subirPlataforma)){
			if (angulo > (PI + (PI/25)))
			{
				subirPlataforma = false;
			}
			angulo = 0;
			saltar = false;
			if(!agachar){
				posicion.second = posicion.second + 6;
				if (velocidades.first != 0) {
					spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_" + this->equipo;
				}
				else {
					spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
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
			subirPlataforma = false;
			velocidades.second = 0;
			if (velocidades.first != 0) {
				spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + "corriendo_" + this->equipo;
			}
			else {
				spriteAEjecutar = "Jugador" + this->armas.at(armaActual)->nombre + this->equipo;
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

bool Jugador::hayMuniciones()
{
	bool municiones = false;
	for (int i = 0; i < this->armas.size(); i++)
	{
		if (!this->armas.at(i)->sinMuniciones())
		{
			municiones = true;
			break;
		}
	}
	return municiones;
}

Proyectil* Jugador::dispararProyectil() {
	if (!saltar && !armasVacias)
	{
		if (hayMuniciones()) {
			if (!this->armas.at(this->armaActual)->sinMuniciones()) {
				// le paso un 1 al final para indicar que el proyectil fue disparado por un jugador
				return (this->armas.at(this->armaActual)->disparar(posicion.first,posicion.second,condicionSprite,anguloBala,agachar,1,this->nombre));
			} else {
				this->armaActual++;
				if (armaActual >= this->armas.size())
				{
					this->armaActual = 0;
				}
			}
		}
		else
		{
			this->armaActual = 0;
			this->armasVacias = true;
		}
	}
	return NULL;
}

bool Jugador::esPlataforma(int x)
{
	for (int i = 0; i < vectorPlataforma.size(); i++)
	{
		if ((x+boxCollider.w >= atoi(vectorPlataforma.at(i).first.c_str())) && (x <= atoi(vectorPlataforma.at(i).second.c_str())))
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
	return ("0|" + nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + to_string(equipoIndice) + "|" + to_string(idSprite) + "|" + condicionSprite + "#");
	//return ("0|" + nombre + "|" + to_string(posicion.first) + "|" + to_string(posicion.second) + "|" + spriteAEjecutar + "|" + condicionSprite + "#");
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
	if (camara.x + anchoVentana >= 7100){
		camara.x = 7100;
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
	else if (tecla == SDLK_z)
	{
		saltar = sePresiono;
	}
	else if (tecla == SDLK_DOWN)
	{
		agachar = sePresiono;
	}
	else if (tecla == SDLK_x)
	{
		disparar = sePresiono;
	}
	else if (tecla == SDLK_UP)
	{
		arriba = sePresiono;
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

void Jugador::obtenerMuniciones(string arma)
{
	for (int i = 0; i < this->armas.size(); i++)
	{
		if (this->armas.at(i)->nombreArma == arma)
		{
			this->armas.at(i)->sumarMuniciones();
			this->armaActual = i;
			this->armasVacias = false;
			break;
		}
	}
}

void Jugador::resetMov(){
	this->saltar = false;
	this->agachar = false;
	this->caer = false;
	this->movDerecha = false;
	this->movIzquierda = false;
	this->disparar = false;
}

void Jugador::setearPlataformas(vector<pair<string,string>> plataformas)
{
	this->vectorPlataforma.clear();
	this->vectorPlataforma = plataformas;
}

bool Jugador::armaActualEsCortoAlcance()
{
	return this->armas.at(this->armaActual)->cortoAlcance;
}


int Jugador::obtenerIdSpriteAEjecutar(){
	//devuelve la posicion en la matriz del sprite a ejecutar
	int idSprite;
	if (saltar){
		//CUBRE IDS 0,1 Y 2
		idSprite = 0 + this->armaActual;
	}
	else if (agachar){
		if (!disparar){
			//CUBRE IDS 18,19 Y 20
			idSprite = 18 + this->armaActual;
		}
		else{
			//CUBRE IDS 21,22 Y 23
			idSprite = 21 + this->armaActual;
		}
	}
	else if(movDerecha || movIzquierda){
		if (!disparar && !arriba){
			//CUBRE IDS 3,4 Y 5
			idSprite = 3 + this->armaActual;
		}
		else if (!disparar && arriba){
			//CUBRE IDS 25,26 Y 27
			idSprite = 25 + this->armaActual;
		}
		else if (disparar && !arriba){
			//CUBRE IDS 7,8 Y 9
			idSprite = 7 + this->armaActual;
		}
		else if (disparar && arriba){
			//CUBRE IDS 10,28,29 Y 30
			if (this->armaActual == 0){
				idSprite = 10;
			}
			else{
				idSprite = 28 + this->armaActual;
			}
		}
	}
	else{
		if (!disparar && !arriba){
			//CUBRE IDS 11,12 Y 13
			idSprite = 11 + this->armaActual;
		}
		else if (!disparar && arriba){
			//CUBRE IDS 34,35 Y 36
			idSprite = 34 + this->armaActual;
		}
		else if (disparar && !arriba){
			//CUBRE IDS 14,15 Y 16
			idSprite = 14 + this->armaActual;
		}
		else if (disparar && arriba){
			//CUBRE IDS 31,32 Y 33
			idSprite = 31 + this->armaActual;
		}
	}
	return idSprite;
}

Arma* Jugador::getArmaActual()
{
	return this->armas.at(armaActual);
}
