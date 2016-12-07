/*
 * mainServidor.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: franco
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include "Servidor.h"
#include <pthread.h>
#include <cmath>
#include "ParametrosMovimiento.h"
using namespace std;

pthread_mutex_t mutexIdProyectil = PTHREAD_MUTEX_INITIALIZER;
int idProyectil = 0;
struct parametrosThreadCliente {
	//estructura que sirve para guardar los parametros que se le pasan a la funcion del thread.
	Servidor* serv;
	int socketCli;
	string nombre;
	int clienteID;
};

struct parametrosThreadEncolarMensaje {
	Mensaje* mensajeNoProcesado;
	Servidor* servidor;
};

struct parametrosThreadEnviarMensajeProcesado {
	Servidor* servidor;
	string usuario;
	int socketCliente;
};

bool stringTerminaCon(std::string const &fullString,
		std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0
				== fullString.compare(fullString.length() - ending.length(),
						ending.length(), ending));
	} else {
		return false;
	}
}

bool fileExists(string fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}

void* encolar(void* arg) {

	parametrosThreadEncolarMensaje parametrosEncolarMensaje =
			*(parametrosThreadEncolarMensaje*) arg;
	Servidor* servidor = parametrosEncolarMensaje.servidor;
	Mensaje* mensaje = parametrosEncolarMensaje.mensajeNoProcesado;

	if (mensaje != NULL)
	{
			pthread_mutex_lock(
					&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
			servidor->crearMensaje(*mensaje);
			pthread_mutex_unlock(
					&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
			mensaje->~Mensaje();
	}
	return NULL;
}

void encolarMensaje(string remitente, string destinatario, string mensaje, Servidor* servidor) {
	pthread_t threadEncolarMensaje;
	parametrosThreadEncolarMensaje parametrosEncolarMensaje;
	parametrosEncolarMensaje.mensajeNoProcesado = new Mensaje(remitente, destinatario, mensaje);
	parametrosEncolarMensaje.servidor = servidor;

	encolar((void*)&parametrosEncolarMensaje);
}

void* procesar(void* arg) {
	parametrosThreadEnviarMensajeProcesado* parametros = (parametrosThreadEnviarMensajeProcesado*) arg;
	Servidor* servidor = parametros->servidor;
	string usuario = parametros->usuario;
	int socket = parametros->socketCliente;
	pthread_mutex_lock(&servidor->mutexListaProcesados);
	string mensajesProcesados = servidor->traerMensajesProcesados(usuario);
	pthread_mutex_unlock(&servidor->mutexListaProcesados);

	int largo = strlen(mensajesProcesados.c_str());
	stringstream iss;
	iss << largo;
	string largoString = iss.str();
	char buffer[BUFFER_MAX_SIZE];
	memset(buffer,'\0',strlen(buffer));
	int inicio = 0;
	int ok;
	if (largo > BUFFER_MAX_SIZE) {
		while (BUFFER_MAX_SIZE < (largo - inicio)) {
			string mensajeSpliteado = mensajesProcesados.substr(inicio, BUFFER_MAX_SIZE);
			strcpy(buffer, mensajeSpliteado.c_str());
			ok = send(socket, buffer, strlen(buffer), 0);
			inicio += BUFFER_MAX_SIZE;
		}
		string mensajeSpliteado = mensajesProcesados.substr(inicio, largo);
		strcpy(buffer, mensajeSpliteado.c_str());
	} else {
		strcpy(buffer, mensajesProcesados.c_str());
	}
	ok = send(socket, buffer, strlen(buffer), 0);
	//cout << "mensaje: " << buffer << endl;
	return NULL;
}

void enviarMensajesProcesadosA(string usuario, Servidor* servidor, int socket) {

	pthread_t threadEnviarMensajesProcesados;
	parametrosThreadEnviarMensajeProcesado parametrosMensajesProcesados;
	if (usuario != "") {
		parametrosMensajesProcesados.usuario = usuario;
		parametrosMensajesProcesados.servidor = servidor;
		parametrosMensajesProcesados.socketCliente = socket;
	}
	procesar((void*)&parametrosMensajesProcesados);
}

void* cicloProcesarMensajes(void* arg) {
	Servidor* servidor = (Servidor*) arg;
	while (servidor->escuchando) {
		usleep(100);
		servidor->procesarMensajes();
	}
}

void* disparoProyectil(void* arg)
{
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	Proyectil* proyectil = parametros->proyectil;

	Mensaje* mensajeProyectil;
	string mensajeProyectilString = "2|0|";
	mensajeProyectilString += proyectil->getStringProyectil();
	mensajeProyectil = new Mensaje(parametros->nombrePersonaje,"Todos",mensajeProyectilString);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);

	// disparado por un jugador
	if (proyectil->disparadoPor == 1) {
		while (!servidor->getNivelActual()->verificarColision(servidor->camara, proyectil, jugador->estaDisparando())) {
			usleep(50000);
			proyectil->mover();
			mensajeProyectilString = "2|1|";
			mensajeProyectilString += proyectil->getStringProyectil();
			mensajeProyectil = new Mensaje(jugador->getNombre(),"Todos",mensajeProyectilString);
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
			mensajeProyectil->~Mensaje();
		}
		if (proyectil->colisionPersonaje)
		{ //si colisiono con un personaje (un enemigo) y no contra un margen, sumo puntos.
			jugador->puntaje += proyectil->getPuntosPorImpacto();
			string mensajePuntajeString = "5|" + jugador->getNombre() + "|" + to_string(jugador->puntaje) + "#";
			Mensaje* mensajePuntaje = new Mensaje(jugador->getNombre(),"Todos", mensajePuntajeString);
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajePuntaje,mensajePuntajeString); //envia mensaje de actualizar puntaje
			mensajePuntaje->~Mensaje();
		}
	} else {
		// disparado por un enemigo
		auto start_time = chrono::high_resolution_clock::now();
		int tiempo = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count();
		while ((!proyectil->cortoAlcance && !servidor->verificarColision(servidor->camara, proyectil, true)) || (proyectil->cortoAlcance && tiempo % 2 == 0)) {
			usleep(50000);
			proyectil->mover();
			mensajeProyectilString = "2|1|";
			mensajeProyectilString += proyectil->getStringProyectil();
			mensajeProyectil = new Mensaje(parametros->nombrePersonaje,"Todos",mensajeProyectilString);
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
			//servidor->verificarColision(servidor->camara, proyectil, true);
			mensajeProyectil->~Mensaje();

			tiempo = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count();
		}
		if (proyectil->colisionPersonaje)
		{ //si colisiono con un personaje (jugador) y no contra un margen, resto la vida.
			string mensajeVidaString = "6|" + proyectil->jugadorQueRecibioDisparo + "|" + to_string(proyectil->vidaDelJugadorImpactado) + "#";
			Mensaje* mensajeVida = new Mensaje(parametros->nombrePersonaje,"Todos", mensajeVidaString);
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeVida,mensajeVidaString);
			mensajeVida->~Mensaje();
		}
	}
	mensajeProyectilString = "2|2|";
	mensajeProyectilString += proyectil->getStringProyectil();
	mensajeProyectil = new Mensaje(parametros->nombrePersonaje,"Todos",mensajeProyectilString);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
	mensajeProyectil->~Mensaje();
	proyectil->~Proyectil();
	pthread_exit(NULL);
}

//funciones auxiliares para actualizar jugadores proyectiles y enemigos

void actualizarPosicionProyectil(ParametrosMovimiento* paramDisparo) {
	//Aca solo se corrobora que un jugador esta disparando, lo del enemigo se hace en otro metodo.
	if (paramDisparo->jugador->estaDisparando()) {
		Proyectil* proyectil = paramDisparo->jugador->dispararProyectil();
		if (proyectil != NULL) {

			pthread_mutex_lock(&mutexIdProyectil);
			proyectil->id = idProyectil;
			idProyectil += 1;
			pthread_mutex_unlock(&mutexIdProyectil);

			paramDisparo->proyectil = proyectil;
			pthread_t threadDisparo = proyectil->getThreadDisparo();
			pthread_create(&threadDisparo, NULL, &disparoProyectil, paramDisparo);
			pthread_detach(threadDisparo);
		}
		if (!paramDisparo->jugador->armaActualEsCortoAlcance()){
			//si no es corto alcance, la dejo como esta.
			paramDisparo->jugador->setDisparando(false);
		}
		usleep(50000);//no se si tiene sentido este usleep
	}
}

void actualizarPosicionCamara(Servidor* servidor, Jugador* jugador) {

	Mensaje* mensajeCamara;
	string mensajeCamaraString;

	pair<int,int> posicionesExtremos = servidor->obtenerPosicionesExtremos();
	int anchoSprite = servidor->getAnchoSprite(jugador->getSpriteAEjecutar());
	bool necesitaCambiarCamara = jugador->chequearCambiarCamara(servidor->camara, atoi(servidor->handshake->getAncho().c_str()), posicionesExtremos, anchoSprite);
	if (necesitaCambiarCamara) {
		if (servidor->camara.x > atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str())){
			servidor->camara.x = 0;
			for (int i = 0; i < servidor->abscisasCapas.size(); i++)
			{
				servidor->abscisasCapas.at(i).first = 0;
			}
			for (int i = 0; i < servidor->jugadores->size(); i++)
			{
				servidor->jugadores->at(i)->resetearPosicion(atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str()));
			}
		} else {
			servidor->camara.x += jugador->getVelocidadX();
			for (int i = 0; i < servidor->abscisasCapas.size(); i++) {
				if (i == 0) {
					servidor->abscisasCapas.at(i).first = servidor->camara.x;
				} else {
					servidor->abscisasCapas.at(i).second += jugador->getVelocidadX();
					servidor->abscisasCapas.at(i).first = abs((servidor->abscisasCapas.at(i).second)*(atoi(servidor->handshake->getImagenes().at(i)->getAncho().c_str()) - atoi(servidor->handshake->getAncho().c_str()))/(atoi(servidor->handshake->getImagenes().at(0)->getAncho().c_str()) - atoi(servidor->handshake->getAncho().c_str())));
					if (servidor->abscisasCapas.at(i).first > atoi(servidor->handshake->getImagenes().at(i)->getAncho().c_str()))
					{
						servidor->abscisasCapas.at(i).first = 0;
						servidor->abscisasCapas.at(i).second = 0;
					}
				}
			}
		}
		mensajeCamaraString = "1|" + to_string(servidor->camara.x) + "|" + to_string(servidor->camara.y) + "|" + servidor->serializarCapas() + "#";
		mensajeCamara = new Mensaje(jugador->getNombre(),"Todos",mensajeCamaraString);
		servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeCamara,mensajeCamaraString);
		mensajeCamara->~Mensaje();
	}
}

void* enviarObjetosEnCamara(void* arg)
{
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	Mensaje* mensajeObjetos;
	string mensajeObjetosString = "";
	while (servidor->escuchando){
		usleep(50000);
		pthread_mutex_lock(&servidor->getNivelActual()->mutexItems);
		for (int i = 0; i < servidor->getNivelActual()->items.size(); i++)
		{
			SDL_Rect box = servidor->getNivelActual()->items.at(i)->boxCollider;
			bool visto = servidor->getNivelActual()->items.at(i)->visto;
			if (box.x <= (servidor->camara.x + servidor->camara.w) && !visto)
			{
				mensajeObjetosString = "3|0|";
				servidor->getNivelActual()->items.at(i)->visto = true;
				mensajeObjetosString += servidor->getNivelActual()->items.at(i)->getStringItem();
				mensajeObjetos = new Mensaje(jugador->getNombre(),"Todos",mensajeObjetosString);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeObjetos,mensajeObjetosString);
				mensajeObjetos->~Mensaje();
			}
		}
		for (int i = 0; i < servidor->getNivelActual()->items.size(); i++) {
			if (servidor->getNivelActual()->items.at(i)->fueObtenido) {
				mensajeObjetosString = "3|1|";
				mensajeObjetosString += servidor->getNivelActual()->items.at(i)->getStringItem();
				mensajeObjetos = new Mensaje(jugador->getNombre(),"Todos",mensajeObjetosString);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeObjetos,mensajeObjetosString);
				mensajeObjetos->~Mensaje();
				servidor->getNivelActual()->items.erase(servidor->getNivelActual()->items.begin()+i);
			}
		}
		pthread_mutex_unlock(&servidor->getNivelActual()->mutexItems);
	}
}

void* actualizarPosicionesJugador(void* arg)
{
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	parametros->nombrePersonaje = jugador->getNombre();
	string mensajeJugadorPosActualizada = "";
	while (jugador->getConectado() && !jugador->getEstaMuerto()){
		pthread_mutex_lock(&servidor->mutexVectorJugadores);
		jugador->mover(servidor->camara);
		//se controla la vida de los proyectiles
		actualizarPosicionProyectil(parametros);

		//Aca se encolan mensajes todo el tiempo del estado del jugador si esta corriendo quieto o lo que sea
		mensajeJugadorPosActualizada = jugador->getStringJugador();
		Mensaje* mensajeAProcesar = new Mensaje();
		mensajeAProcesar->setRemitente(jugador->getNombre());
		pthread_mutex_unlock(&servidor->mutexVectorJugadores);
		mensajeAProcesar->setDestinatario("Todos");
		servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeAProcesar,mensajeJugadorPosActualizada);
		if (servidor->getNivelActual()->verificarColisionConItem(jugador) && servidor->getNivelActual()->bonusColisionado == 2)
		{
			string mensajeVidaString = "6|" + jugador->getNombre() + "|" + to_string(jugador->getVida()) + "#";
			Mensaje* mensajeVida = new Mensaje(jugador->getNombre(),"Todos", mensajeVidaString);
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeVida,mensajeVidaString);
			mensajeVida->~Mensaje();
		}
		mensajeAProcesar->~Mensaje();
		//----------------------------------------------------------------------------------------------------

		actualizarPosicionCamara(servidor,jugador);
		usleep(50000);
	}
}

void generarBonus(int estado, int x, int y, Escenario* nivelActual)
{
	switch (estado)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			nivelActual->agregarItemBonus(new Item("KillAll",to_string(x+87),to_string(y+30),1,"3"));
			break;
		}
		case 2:
		{
			nivelActual->agregarItemBonus(new Item("Power",to_string(x+85),to_string(y+38),2,"4"));
			break;
		}
	}
}

int calcularDistancia(Enemigo* enemigo, Jugador* jugador)
{
	return (abs(enemigo->getPosX() - jugador->getPosicion().first));
}


void* enemigoActivo(void* arg) {
	ParametrosMovimiento* parametrosEnemigo = (ParametrosMovimiento*) arg;
	Servidor* servidor = parametrosEnemigo->servidor;
	Enemigo* enemigo = parametrosEnemigo->enemigo;
	Jugador* jugador = parametrosEnemigo->jugador;
	string nombre = "Enemigo";
	string mensajeEnemigo = "4|0|";
	mensajeEnemigo += enemigo->getInformacionDelEnemigo();
	Mensaje* mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
	mensaje->~Mensaje();
	mensajeEnemigo = "4|1|";
	mensajeEnemigo += enemigo->getInformacionDelEnemigo();
	mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
	mensaje->~Mensaje();
	auto start_time = chrono::high_resolution_clock::now();
	bool yaDisparo = false;
	while (!enemigo->getEstaMuerto() && (!servidor->getNivelActual()->enemigoPerdido(enemigo->getId(),&servidor->camara)) && !servidor->getNivelActual()->levelClear) {
		int tiempoTranscurrido = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count();
		usleep(50000);
		switch (enemigo->estado)
		{
			case 1:
			{
				if (tiempoTranscurrido %3 == 0 && enemigo->estaDisparando() && !yaDisparo) {

					enemigo->setSprite("2");
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();

					Proyectil* proyectil1 = enemigo->dispararProyectil(6,"0",25,10,22,13,10);
					yaDisparo = true;
					pthread_mutex_lock(&mutexIdProyectil);
					proyectil1->id = idProyectil;
					idProyectil += 1;
					pthread_mutex_unlock(&mutexIdProyectil);
					ParametrosMovimiento* parametrosProyectil1 = new ParametrosMovimiento(servidor, NULL);
					parametrosProyectil1->servidor = servidor;
					parametrosProyectil1->enemigo = enemigo;
					parametrosProyectil1->proyectil = proyectil1;
					parametrosProyectil1->nombrePersonaje = "Enemigo";
					pthread_t threadDisparo1;
					pthread_create(&threadDisparo1, NULL, &disparoProyectil, parametrosProyectil1);
					pthread_detach(threadDisparo1);
					parametrosProyectil1->~ParametrosMovimiento();
					usleep(400000);
					Proyectil* proyectil2 = enemigo->dispararProyectil(6,"0",25,10,22,13,10);
					pthread_mutex_lock(&mutexIdProyectil);
					proyectil2->id = idProyectil;
					idProyectil += 1;
					pthread_mutex_unlock(&mutexIdProyectil);
					ParametrosMovimiento* parametrosProyectil2 = new ParametrosMovimiento(servidor, NULL);
					parametrosProyectil2->servidor = servidor;
					parametrosProyectil2->enemigo = enemigo;
					parametrosProyectil2->proyectil = proyectil2;
					parametrosProyectil2->nombrePersonaje = "Enemigo";
					pthread_t threadDisparo2;
					pthread_create(&threadDisparo2, NULL, &disparoProyectil, parametrosProyectil2);
					pthread_detach(threadDisparo2);
					parametrosProyectil2->~ParametrosMovimiento();

					usleep(400000);
					enemigo->setSprite("1");
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();
				} else if (tiempoTranscurrido %2 != 0) {
					yaDisparo = false;
				}
				//enemigo->walk();
				enemigo->caminar(servidor->camara,servidor->getNivelActual()->plataformas);
				mensajeEnemigo = "4|1|";
				mensajeEnemigo += enemigo->getInformacionDelEnemigo();
				mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
				mensaje->~Mensaje();
				/*if (calcularDistancia(enemigo,jugador) <= 300)
				{
					enemigo->estado = 5;
					enemigo->setSprite("5");
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();
				}*/
				break;
			}
			case 2:
			{
				enemigo->caer();
				mensajeEnemigo = "4|1|";
				mensajeEnemigo += enemigo->getInformacionDelEnemigo();
				mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
				mensaje->~Mensaje();
				if (tiempoTranscurrido % 2 == 0 && !yaDisparo && tiempoTranscurrido != 0)
				{
					enemigo->estado = 4;
					enemigo->setSprite("6");
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();
				}
				else if (tiempoTranscurrido % 2 != 0)
				{
					yaDisparo = false;
				}
				if (enemigo->tieneQueDejarDeCaer(servidor->getNivelActual()->plataformas))
				{
					enemigo->estado = 3; //Empieza a caminar
					enemigo->setSprite("4");
					enemigo->setYCaida();
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();
				}
				break;
			}
			case 3:
			{
				//enemigo->walk();
				enemigo->caminar(servidor->camara,servidor->getNivelActual()->plataformas);
				mensajeEnemigo = "4|1|";
				mensajeEnemigo += enemigo->getInformacionDelEnemigo();
				mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
				mensaje->~Mensaje();
				if (calcularDistancia(enemigo,jugador) <= 300)
				{
					enemigo->estado = 5;
					enemigo->setSprite("5");
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();
				}
				break;
			}
			case 4:
			{
				Proyectil* proyectil = enemigo->dispararProyectil(3,"5",44,105,78,13,20);
				pthread_mutex_lock(&mutexIdProyectil);
				proyectil->id = idProyectil;
				idProyectil += 1;
				pthread_mutex_unlock(&mutexIdProyectil);
				ParametrosMovimiento* parametrosProyectil = new ParametrosMovimiento(servidor, NULL);
				parametrosProyectil->servidor = servidor;
				parametrosProyectil->enemigo = enemigo;
				parametrosProyectil->proyectil = proyectil;
				parametrosProyectil->nombrePersonaje = "Enemigo";
				pthread_t threadDisparo;
				pthread_create(&threadDisparo, NULL, &disparoProyectil, parametrosProyectil);
				pthread_detach(threadDisparo);
				parametrosProyectil->~ParametrosMovimiento();
				yaDisparo = true;
				enemigo->estado = 2;
				enemigo->setSprite("8");
				break;
			}
			case 5:
			{
				if (tiempoTranscurrido % 2 == 0 && !yaDisparo && tiempoTranscurrido != 0)
				{
					Proyectil* proyectil = enemigo->dispararProyectil(6,"5",25,10,78,13,20);
					pthread_mutex_lock(&mutexIdProyectil);
					proyectil->id = idProyectil;
					idProyectil += 1;
					pthread_mutex_unlock(&mutexIdProyectil);
					ParametrosMovimiento* parametrosProyectil = new ParametrosMovimiento(servidor, NULL);
					parametrosProyectil->servidor = servidor;
					parametrosProyectil->enemigo = enemigo;
					parametrosProyectil->proyectil = proyectil;
					parametrosProyectil->nombrePersonaje = "Enemigo";
					pthread_t threadDisparo;
					pthread_create(&threadDisparo, NULL, &disparoProyectil, parametrosProyectil);
					pthread_detach(threadDisparo);
					parametrosProyectil->~ParametrosMovimiento();
					yaDisparo = true;
				}
				else if (tiempoTranscurrido % 2 != 0)
				{
					yaDisparo = false;
				}
				if (calcularDistancia(enemigo,jugador) > 300)
				{
					enemigo->estado = 3;
					enemigo->setSprite("4");
					mensajeEnemigo = "4|2|";
					mensajeEnemigo += enemigo->getInformacionDelEnemigo();
					mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
					servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
					mensaje->~Mensaje();
				}
				break;
			}
		}
	}
	mensajeEnemigo = "4|2|";
	if (!enemigo->esEnemigoBoss)
	{
		enemigo->setSprite("3");
	}
	else
	{
		enemigo->setSprite("7");
	}
	mensajeEnemigo += enemigo->getInformacionDelEnemigo();
	mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
	usleep(900000);
	mensajeEnemigo = "4|3|";
	mensajeEnemigo += enemigo->getInformacionDelEnemigo();
	mensaje->setTexto(mensajeEnemigo);
	srand(parametrosEnemigo->servidor->getNivelActual()->rdtsc());
	int estadoBonus = rand() % 3;
	if (!enemigo->esEnemigoBoss)
	{
		generarBonus(estadoBonus, enemigo->getPosX(), enemigo->getPosY(), parametrosEnemigo->servidor->getNivelActual());
	}
	servidor->getNivelActual()->eliminarEnemigoActivo(enemigo->getId());
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
	mensaje->~Mensaje();
	pthread_exit(NULL);
}

void* controlDeEnemigos(void* arg) {

	ParametrosMovimiento* parametroRecibido= (ParametrosMovimiento*) arg;
	Servidor* servidor = parametroRecibido->servidor;
	while (servidor->escuchando) {
		usleep(50000);
		servidor->getNivelActual()->despertarEnemigos(&servidor->camara);
		pthread_mutex_lock(&servidor->getNivelActual()->mutexEnemigosActivos);
		for (int i = 0; i < servidor->getNivelActual()->getEnemigosActivos().size(); i++) {
			Enemigo* enemigo = servidor->getNivelActual()->getEnemigoActivo(i);
			if (enemigo != NULL && !enemigo->threadAsociado && !enemigo->esEnemigoBoss) {
				enemigo->threadAsociado = true;
				pthread_t threadEnemigo;
				ParametrosMovimiento* parametrosEnemigo = new ParametrosMovimiento(servidor,parametroRecibido->jugador);
				parametrosEnemigo->enemigo = enemigo;
				pthread_create(&threadEnemigo, NULL, &enemigoActivo, parametrosEnemigo);
				pthread_detach(threadEnemigo);
				parametrosEnemigo->~ParametrosMovimiento();
			}
		}
		pthread_mutex_unlock(&servidor->getNivelActual()->mutexEnemigosActivos);
	}
}

void* disparoBoss(void* arg)
{
	ParametrosMovimiento* parametrosBoss = (ParametrosMovimiento*) arg;
	Servidor* servidor = parametrosBoss->servidor;
	Boss* boss = parametrosBoss->boss;
	pair<vector<Proyectil*>,int> proyectiles = parametrosBoss->proyectiles;
	for (int i = 0; i < (proyectiles.first.size()/proyectiles.second); i++)
	{
		pthread_t threadDisparoBoss;
		parametrosBoss->proyectil = proyectiles.first.at(i);
		parametrosBoss->personaje = boss;
		for (int j = 0; j < proyectiles.second; j++)
		{
			parametrosBoss->proyectil->posicion = make_pair(boss->posX + parametrosBoss->proyectil->desp.first,boss->posY + parametrosBoss->proyectil->desp.second);
			parametrosBoss->proyectil->boxCollider.x = boss->posX + parametrosBoss->proyectil->desp.first;
			parametrosBoss->proyectil->boxCollider.y = boss->posY + parametrosBoss->proyectil->desp.second;
			pthread_mutex_lock(&mutexIdProyectil);
			parametrosBoss->proyectil->id = idProyectil;
			idProyectil += 1;
			pthread_mutex_unlock(&mutexIdProyectil);
			pthread_create(&threadDisparoBoss, NULL, &disparoProyectil, parametrosBoss);
			pthread_detach(threadDisparoBoss);
		}
		usleep (300000);
	}
}

void* bossActivo(void* arg)
{
	ParametrosMovimiento* parametrosBoss = (ParametrosMovimiento*) arg;
	Servidor* servidor = parametrosBoss->servidor;
	string nombre = parametrosBoss->jugador->getNombre();
	Boss* bossNivel = parametrosBoss->boss;
	srand(time(NULL));
	string mensajeBoss = "7|0|";
	mensajeBoss += bossNivel->getStringBoss();
	Mensaje* mensaje = new Mensaje(nombre,"Todos",mensajeBoss);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeBoss);
	bool tieneQueDisparar = false;
	auto start_time = chrono::high_resolution_clock::now();
	while (!bossNivel->getEstaMuerto()) {
		usleep(50000);
		int tiempoTranscurrido = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count();
		if (tiempoTranscurrido % bossNivel->tiempoEntreArmas == 0 && !bossNivel->disparando && !tieneQueDisparar)
		{
			tieneQueDisparar = true;
		}
		else if (tiempoTranscurrido % bossNivel->tiempoEntreArmas != 0)
		{
			bossNivel->disparando = false;
		}
		bossNivel->comportamiento(servidor->camara, tieneQueDisparar);
		mensajeBoss = "7|1|";
		mensajeBoss += bossNivel->getStringBoss();
		mensaje = new Mensaje(nombre,"Todos",mensajeBoss);
		servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeBoss);
		mensaje->~Mensaje();
		if (tieneQueDisparar)
		{
			pthread_t bossDisparar;
			if (!bossNivel->liberaSoldados)
			{
				parametrosBoss->proyectiles = bossNivel->proyectilesADisparar;
				pthread_create(&bossDisparar, NULL, &disparoBoss, parametrosBoss);
				pthread_detach(bossDisparar);
			}
			else
			{
				if (tiempoTranscurrido != 0)
				{
					parametrosBoss->enemigo = bossNivel->liberarSoldado(parametrosBoss->jugador->getPosicion().first);
					parametrosBoss->enemigo->setId(servidor->getNivelActual()->idEnemigo);
					servidor->getNivelActual()->agregarEnemigoActivo(parametrosBoss->enemigo);
					pthread_create(&bossDisparar, NULL, &enemigoActivo, parametrosBoss);
					pthread_detach(bossDisparar);
					servidor->getNivelActual()->idEnemigo++;
				}
			}
			tieneQueDisparar = false;
		}
	}
	bossNivel->nombre += "_destruido";
	mensajeBoss = "7|2|";
	mensajeBoss += bossNivel->getStringBoss();
	mensaje = new Mensaje(nombre,"Todos",mensajeBoss);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeBoss);
	servidor->getNivelActual()->levelClear = true;
	mensaje->~Mensaje();
	pthread_exit(NULL);
}

void* verificarBossEnCamara(void* arg)
{
	ParametrosMovimiento* parametroRecibido= (ParametrosMovimiento*) arg;
	Servidor* servidor = parametroRecibido->servidor;
	Jugador* jugador = parametroRecibido->jugador;
	Boss* bossNivel = servidor->getNivelActual()->boss.at(0);
	ParametrosMovimiento* parametrosBoss = new ParametrosMovimiento(servidor, jugador);
	while (!servidor->getNivelActual()->levelClear) {
		usleep(50000);
		//servidor->getNivelActual()->despertarBoss(servidor->camara);
		if (bossNivel != NULL && bossNivel->boxCollider.x <= (servidor->camara.x + servidor->camara.w) && !bossNivel->visto) {
			parametrosBoss->boss = bossNivel;
			pthread_t threadBoss;
			pthread_create(&threadBoss, NULL, &bossActivo, parametrosBoss);
			pthread_detach(threadBoss);
			bossNivel->visto = true;
		}
	}

}

void* verificarPasarDeNivel(void* arg)
{
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	while(servidor->escuchando)
	{
		usleep(50000);
		if (servidor->getNivelActual()->levelClear)
		{
			cout << "Level clear" << endl;
			usleep(2000000);
			servidor->calcularPuntajes();
		}
		if (servidor->getNivelActual()->avanzoDeNivel)
		{
			servidor->avanzarDeNivel();
			pthread_t threadVerificarBoss;
			pthread_create(&threadVerificarBoss, NULL, &verificarBossEnCamara, parametros);
			pthread_detach(threadVerificarBoss);
			servidor->getNivelActual()->avanzoDeNivel = false;
		}
	}
}

void iniciarThreadMovimientoJugador(Servidor* servidor, string nombre)
{
	pthread_mutex_lock(&servidor->mutexVectorJugadores);
	Jugador* jugador = servidor->obtenerJugador(nombre);
	pthread_mutex_unlock(&servidor->mutexVectorJugadores);
	ParametrosMovimiento * parametros = new ParametrosMovimiento(servidor,jugador);/*ParametrosActPosicion parametros;
	parametros.jugador = jugador;
	parametros.servidor = this;*/
	pthread_t threadMov = jugador->getThreadMovimiento();
	pthread_t threadNiveles;
	pthread_t threadEnemigosPetutos;
	pthread_t threadVerificarBoss;
	pthread_t threadObjetos;
	pthread_create(&threadMov, NULL, &actualizarPosicionesJugador, parametros);
	pthread_detach(threadMov);
	pthread_create(&threadNiveles, NULL, &verificarPasarDeNivel, parametros);
	pthread_detach(threadNiveles);
	pthread_create(&threadEnemigosPetutos, NULL, &controlDeEnemigos, parametros);
	pthread_detach(threadEnemigosPetutos);
	pthread_create(&threadVerificarBoss, NULL, &verificarBossEnCamara, parametros);
	pthread_detach(threadVerificarBoss);
	pthread_create(&threadObjetos, NULL, &enviarObjetosEnCamara, parametros);
	pthread_detach(threadObjetos);
	jugador->setThreadMovimiento(threadMov);
}



void* cicloEscuchaCliente(void* arg) {
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	pthread_mutex_t mutexSocket;
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*) arg;
	Servidor* servidor = parametros->serv;
	string nombre = parametros->nombre;
	int socketCliente = parametros->socketCli;
	servidor->guardarLog("Escuchando al cliente: " + nombre + string(".\n"),
			INFO);
	char bufferRecibido[BUFFER_MAX_SIZE];
	bool conectado = true;
	servidor->setJugadorConectado(nombre);
	while (conectado and servidor->escuchando) {
		usleep(100);
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		string datosRecibidos;
		int largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE,0); //recibo por primera vez
		if (largoRequest > 0) {
			datosRecibidos.append(bufferRecibido, largoRequest);
			while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")) {
				//mientras haya cosas que leer, sigo recibiendo.
				largoRequest = recv(socketCliente, bufferRecibido,BUFFER_MAX_SIZE, 0);
				datosRecibidos.append(bufferRecibido, largoRequest);
			}
			if (largoRequest < 0) {
				conectado = false;
				//servidor->setJugadorDesconectado(nombre);
				servidor->restarCantidadClientesConectados();
				servidor->verificarDesconexion(nombre);
			}
			else if (largoRequest == 0){
				conectado = false;
				//servidor->setJugadorDesconectado(nombre);
				servidor->verificarDesconexion(nombre);
			}
			else
			{
				//en el formato siempre recibimos primero el metodo que es un entero.
				//1 representa enviar un mensaje, 2 representa recibir mis mensajes, 3 verificar la conexion.

				char* datos = strdup(datosRecibidos.c_str());
				char* metodo = strtok(datos, "|");
				while (metodo != NULL) {
					int accion = atoi(metodo); //convierto a entero el metodo recibido por string
					switch (accion) {
						case 1: { //1 el cliente me envia un mensaje
							char* remitente = strtok(NULL, "|");
							char* destinatario = strtok(NULL, "|");
							char* mensaje = strtok(NULL, "#");
							if (remitente != NULL && destinatario != NULL && mensaje != NULL) {
								encolarMensaje(string(remitente), string(destinatario), string(mensaje), servidor);
							}
							//usleep(50000);
							break;
						}
						case 2: { //2 el cliente quiere recibir sus mensajes
							char* usuarioQueSolicita = strtok(NULL, "#");
							//usleep(50000);
							if (usuarioQueSolicita != NULL) {
								enviarMensajesProcesadosA(string(usuarioQueSolicita), servidor, socketCliente);
							}
							break;
						}
						case 3: {//3 es verificar conexion
							char buffer[BUFFER_MAX_SIZE] = "Escuchando";
							int ok = send(socketCliente,buffer,strlen(buffer),0);
							if (ok > 0) {

							}
							else if (ok == 0){
								servidor->guardarLog("Se cerró la conexión con el cliente " + nombre + string(".\n"),DEBUG);
							}
							else{
								servidor->guardarLog("ERROR: Ocurrió un problema con el socket del cliente: " + nombre + string(".\n"),DEBUG);
							}
							break;
						}
						case 4: {//4 es se desconecto el cliente, es un mensaje de log diferente al si se corta la conexion
							servidor->restarCantidadClientesConectados();
							servidor->verificarDesconexion(nombre);
							break;
						}
						case 5: {
							char comenzoJuego[BUFFER_MAX_SIZE];
							string comenzo;
							if (servidor->getJugadoresConectados()->size() == servidor->getCantJugadoresConectadosMax()){
								string jugadoresInicio = servidor->getEstadoInicialSerializado();
								comenzo = "0|"  + jugadoresInicio + "@";
								servidor->iniciarCamara();
								iniciarThreadMovimientoJugador(servidor,nombre);
							}
							else {
								comenzo = "1@";
							}
							strcpy(comenzoJuego,comenzo.c_str());
							send(socketCliente,comenzoJuego,strlen(comenzoJuego),0);
							break;
						}
						case 6: {//6 es enviarHandShake
							char* cliente = strtok(NULL, "|");
							if (cliente != NULL)
							{
								servidor->enviarHandshake(socketCliente,cliente);
								//usleep(1000000);
							}
							break;
						}
					}
					metodo = strtok(NULL, "|");
				}
			}
		} else {
			string mensaje ="";
			if (largoRequest == 0){
				mensaje = "Se cerró la conexión con el cliente: " + nombre + string(".\n");
			}
			else{
				mensaje = "ERROR: Ocurrió un problema con el socket del cliente: " + nombre + string(".\n");
			}
			//servidor->setJugadorDesconectado(nombre);
			cout << "Se desconecto el cliente " << nombre << endl;
			conectado = false;
			servidor->verificarDesconexion(nombre);
			servidor->guardarLog(mensaje, DEBUG);
			servidor->restarCantidadClientesConectados();
			std::ostringstream oss;
			oss << servidor->getCantConexiones();
			string conectados = oss.str();
			servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
		}
	}
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	Servidor* servidor = (Servidor*) arg;
	pthread_t threadProceso;

	int ok = pthread_create(&threadProceso, NULL, &cicloProcesarMensajes, (void*) servidor);
	servidor->setThreadProceso(threadProceso);

	pthread_t thread_id[MAX_CANT_CLIENTES]; //la cantidad maxima de clientes es 6, voy a crear, como mucho 6 threads para manejar dichas conexiones.
	for (int i = 0; i < MAX_CANT_CLIENTES; i++) {
		thread_id[i] = NULL; //inicializo todos en null
	}
	while (servidor->escuchando) {
		//se va a generar un thread para que cada cliente tenga su comunicacion con el servidor.
		pair<int, string> cliente = servidor->aceptarConexion();
		int socketCliente = cliente.first;
		string nombreCliente = cliente.second;
		if (socketCliente <= 0) {
			cout << "Error al conectar al cliente" << endl;
			servidor->guardarLog("No se pudo conectar al cliente.\n", INFO);
			servidor->guardarLog(
					"ERROR: Problema con la conexion del socket.\n", DEBUG);
		} else {
			servidor->guardarLog("Cliente conectado: " + nombreCliente + string("\n"), INFO);
			std::ostringstream oss;
			oss << servidor->getCantConexiones();
			string conectados = oss.str();
			servidor->guardarLog("Cantidad de clientes conectados: " + conectados + string("\n"),INFO);
			//genero un nuevo thread dinamicamente para este cliente
			if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES
					&& servidor->getCantConexiones() > 0) {
				//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
				parametrosThreadCliente parametrosCliente;
				parametrosCliente.socketCli = socketCliente;
				parametrosCliente.serv = servidor;
				parametrosCliente.nombre = nombreCliente;
				parametrosCliente.clienteID = servidor->getCantConexiones() - 1;
				int ok = pthread_create(&thread_id[parametrosCliente.clienteID], NULL,
						&cicloEscuchaCliente, &parametrosCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
				if (ok != 0)
				{
					servidor->guardarLog("ERROR: No se pudo crear el thread de comunicación con el cliente.\n",DEBUG);
				}
				else{
					servidor->guardarLog("Thread de comunicación con el cliente creado correctamente.\n",DEBUG);
				}
				pthread_detach(thread_id[parametrosCliente.clienteID]); //lo marco como detach
			}
		}
	}
}

Servidor* inicializarServidor() {
	pthread_t thrConexiones;
	int puerto;
	string archivoUsers;
	int modoLogger;
	bool puertoValido = false;
	bool modoLoggerValido = false;
	do {
		cout << "Elija el modo en el que quiere loggear los eventos: " << endl;
		cout << "1) INFO" << endl;
		cout << "2) DEBUG" << endl;
		cin >> modoLogger;
		if (cin.good() && (modoLogger == 1 || modoLogger == 2)) {
			modoLoggerValido = true;
		} else {
			cin.clear();
			cin.ignore();
			cout << "Error: la opcion ingresada no es valida" << endl;
		}
	} while (!modoLoggerValido);
	Logger* logger = new Logger(modoLogger);
	stringstream modo;
	modo << modoLogger;
	logger->escribir(
			"El nivel de log seleccionado es: " + modo.str() + string(". \n"),
			INFO);
	do {
		cout << "Ingrese el puerto de escucha de la conexion: " << endl;
		cin >> puerto;
		if (cin.good()) {
			puertoValido = true;
			stringstream puertoStr;
			puertoStr << puerto;
			logger->escribir(
					"Puerto ingresado: " + puertoStr.str() + string(". \n"),
					INFO);
		} else {
			cin.clear();
			cin.ignore();
			cout << "Error: el dato ingresado debe ser un numero" << endl;
			logger->escribir("ERROR: Puerto inválido. \n", INFO);
		}
	} while (!puertoValido);
	bool existeArchivo;
	do {
		cout << "Ingrese el nombre del archivo de usuarios: " << endl;
		cin >> archivoUsers;
		logger->escribir("Archivo ingresado: " + archivoUsers + string(". \n"),
				INFO);
		existeArchivo = fileExists(archivoUsers);
		if (!existeArchivo) {
			logger->escribir("ERROR: El archivo no existe. \n", INFO);
			cout << "Error: El archivo no existe." << endl;
		}
	} while (!existeArchivo);
	logger->escribir("Archivo encontrado.\n", DEBUG);
	char* archivo = strdup(archivoUsers.c_str());
	Servidor* servidor = new Servidor(archivo, puerto, logger);

	do {
		servidor->comenzarEscucha();
	} while (!servidor->escuchando);

	int threadOk = pthread_create(&thrConexiones, NULL,
			&cicloEscucharConexionesNuevasThreadProceso, servidor);
	if (threadOk != 0) {
		cout << "Error inicializando el thread de escucha de conexiones"
				<< endl;
		servidor->guardarLog(
				"ERROR: No se pudo crear el thread de escucha de conexiones.\n",
				DEBUG);
	} else {
		servidor->guardarLog(
				"Creación de thread de escucha de conexiones OK. \n", DEBUG);
	}
	pthread_detach(thrConexiones);
	return servidor;
}

int main() {
	Servidor* servidor = inicializarServidor();
	cout << "Escriba q y presione enter para salir.. \n";
	string salir = "";
	while (servidor->escuchando) {
		cin >> salir;
		if (salir == "q") {
			string mensaje = "Se ha cerrado el servidor";
			servidor->guardarLog(mensaje, INFO);
			servidor->finalizarEscucha();
		}
	}
	return 0;
}
