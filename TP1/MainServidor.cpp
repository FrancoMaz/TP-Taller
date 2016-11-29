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
#include "Servidor.h"
#include <pthread.h>
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
	//cout << "mensaje: " << mensajesProcesados << endl;
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
	Personaje* personaje = parametros->personaje;
	Jugador* jugador = parametros->jugador;
	Proyectil* proyectil = parametros->proyectil;
	Mensaje* mensajeProyectil;
	//servidor->getNivelActual()->agregarProyectil(proyectil,jugador->getNombre(),idProyectil);
	pthread_mutex_lock(&mutexIdProyectil);
	proyectil->id = idProyectil;
	servidor->getNivelActual()->agregarProyectil(proyectil,"random");
	idProyectil += 1;
	pthread_mutex_unlock(&mutexIdProyectil);
	string mensajeProyectilString = "2|0|";
	mensajeProyectilString += proyectil->getStringProyectil();
	mensajeProyectil = new Mensaje(personaje->getNombre(),"Todos",mensajeProyectilString);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
	cout << "Disparado por: " << proyectil->disparadoPor << endl;
	if (proyectil->disparadoPor == 1) { // disparado por un jugador
		while (!servidor->getNivelActual()->verificarColision(servidor->camara, proyectil, personaje->estaDisparando())) {
			usleep(50000);
			proyectil->mover();
			mensajeProyectilString = "2|1|";
			mensajeProyectilString += proyectil->getStringProyectil();
			mensajeProyectil = new Mensaje(personaje->getNombre(),"Todos",mensajeProyectilString);
			servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
			mensajeProyectil->~Mensaje();
		}
	} else { // disparado por un enemigo
	    bool disparando = personaje->estaDisparando();
		while (!servidor->verificarColision(servidor->camara, proyectil, disparando)) {
			usleep(50000);
			proyectil->mover();
			mensajeProyectilString = "2|1|";
			mensajeProyectilString += proyectil->getStringProyectil();
			if (personaje != NULL) {
				mensajeProyectil = new Mensaje(personaje->getNombre(),"Todos",mensajeProyectilString);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
				mensajeProyectil->~Mensaje();
			} else {
				break;
			}
		}
	}
	mensajeProyectilString = "2|2|";
	/*Nunca se elemina el proyectil que ya murio del vector de proyectiles, y no sabemos como influye esto
	 * cuando invocamos al destructor de proyectil sin haberlo sacado del vector previamente.
	 */
	mensajeProyectilString += proyectil->getStringProyectil();
	mensajeProyectil = new Mensaje(personaje->getNombre(),"Todos",mensajeProyectilString);
	servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeProyectil,mensajeProyectilString);
	mensajeProyectil->~Mensaje();
	proyectil->~Proyectil();
	pthread_exit(NULL);
}

//funciones auxiliares para actualizar jugadores proyectiles y enemigos

void actualizarPosicionProyectil(ParametrosMovimiento* paramDisparo) {

	if (paramDisparo->personaje->estaDisparando()) {
		Proyectil* proyectil = paramDisparo->personaje->dispararProyectil();
		if (proyectil != NULL) {
			paramDisparo->proyectil = proyectil;
			pthread_t threadDisparo = proyectil->getThreadDisparo();
			if (proyectil->disparadoPor == 2) {
				usleep(2000000);
			}
			pthread_create(&threadDisparo, NULL, &disparoProyectil, paramDisparo);
			pthread_detach(threadDisparo);
			//proyectil->setThreadDisparo(threadDisparo);
		}
		usleep(50000);
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
	while (jugador->getConectado()){
		usleep(50000);
		for (int i = 0; i < servidor->getNivelActual()->itemArmas.size(); i++)
		{
			SDL_Rect box = servidor->getNivelActual()->itemArmas.at(i)->boxCollider;
			bool visto = servidor->getNivelActual()->itemArmas.at(i)->visto;
			if (box.x <= (servidor->camara.x + servidor->camara.w) && !visto)
			{
				mensajeObjetosString = "3|0|";
				servidor->getNivelActual()->itemArmas.at(i)->visto = true;
				mensajeObjetosString += servidor->getNivelActual()->itemArmas.at(i)->getStringItem();
				mensajeObjetos = new Mensaje(jugador->getNombre(),"Todos",mensajeObjetosString);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeObjetos,mensajeObjetosString);
			}
			if (servidor->getNivelActual()->itemArmas.at(i)->fueObtenido)
			{
				mensajeObjetosString = "3|1|";
				mensajeObjetosString += servidor->getNivelActual()->itemArmas.at(i)->getStringItem();
				mensajeObjetos = new Mensaje(jugador->getNombre(),"Todos",mensajeObjetosString);
				servidor->encolarMensajeProcesadoParaCadaCliente(*mensajeObjetos,mensajeObjetosString);
				servidor->getNivelActual()->itemArmas.erase(servidor->getNivelActual()->itemArmas.begin()+i);
			}
		}
	}
}

void* verificarPasarDeNivel(void* arg)
{
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	while(jugador->getConectado())
	{
		usleep(50000);
		if (servidor->getNivelActual()->levelClear)
		{
			servidor->getNivelActual()->~Escenario();
			servidor->avanzarDeNivel();
		}
	}
}

void* actualizarPosicionesJugador(void* arg)
{
	ParametrosMovimiento* parametros = (ParametrosMovimiento*)arg;
	Servidor* servidor = parametros->servidor;
	Jugador* jugador = parametros->jugador;
	parametros->personaje = jugador;
	//Mensaje* mensajeCamara;
	//string mensajeCamaraString;
	string mensajeJugadorPosActualizada = "";
	pthread_t threadObjetos;
	pthread_t threadNiveles;
	pthread_create(&threadNiveles, NULL, &verificarPasarDeNivel, parametros);
	pthread_detach(threadNiveles);
	pthread_create(&threadObjetos, NULL, &enviarObjetosEnCamara, parametros);
	pthread_detach(threadObjetos);
	while (jugador->getConectado()){
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
		servidor->getNivelActual()->verificarColisionConItem(jugador);
		mensajeAProcesar->~Mensaje();
		//----------------------------------------------------------------------------------------------------

		actualizarPosicionCamara(servidor,jugador);
		usleep(50000);
	}
}

void* enemigoActivo(void* arg) {
	ParametrosMovimiento* parametrosEnemigo = (ParametrosMovimiento*) arg;
	Servidor* servidor = parametrosEnemigo->servidor;
	string nombre = parametrosEnemigo->jugador->getNombre();
	parametrosEnemigo->personaje = parametrosEnemigo->enemigo;
	string mensajeEnemigo = "4|0|";
	mensajeEnemigo += parametrosEnemigo->enemigo->getInformacionDelEnemigo();
	Mensaje* mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
	parametrosEnemigo->servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
	Enemigo* enemigo = parametrosEnemigo->enemigo;
	while (!enemigo->getEstaMuerto() && (!parametrosEnemigo->servidor->getNivelActual()->enemigoPerdido(enemigo->getId(),&servidor->camara))) {
		usleep(50000);
		mensajeEnemigo = "4|1|";
		mensajeEnemigo += enemigo->getInformacionDelEnemigo();
		mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
		parametrosEnemigo->servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
		mensaje->~Mensaje();
		if(enemigo->estado == 1){
			actualizarPosicionProyectil(parametrosEnemigo);
		}
	}
	parametrosEnemigo->servidor->getNivelActual()->eliminarEnemigoActivo(enemigo->getId());
	mensajeEnemigo = "4|2|";
	mensajeEnemigo += enemigo->getInformacionDelEnemigo();
	mensaje = new Mensaje(nombre,"Todos",mensajeEnemigo);
	parametrosEnemigo->servidor->encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeEnemigo);
	enemigo->~Enemigo();
	mensaje->~Mensaje();
	pthread_exit(NULL);
}

void* controlDeEnemigos(void* arg) {
	ParametrosMovimiento* parametroRecibido= (ParametrosMovimiento*) arg;
	Servidor* servidor = parametroRecibido->servidor;
	Jugador* jugador = parametroRecibido->jugador;
	ParametrosMovimiento* parametrosEnemigo = new ParametrosMovimiento(servidor, jugador);
	//usleep(4000000);
	while (servidor->escuchando) {
		servidor->getNivelActual()->despertarEnemigos(&servidor->camara);
		for (int i = 0; i < parametrosEnemigo->servidor->getNivelActual()->getEnemigosActivos().size(); i++) {
			Enemigo* enemigo = parametrosEnemigo->servidor->getNivelActual()->getEnemigoActivo(i);
			if (enemigo != NULL && !enemigo->threadAsociado) {
				enemigo->threadAsociado = true;
				parametrosEnemigo->enemigo = enemigo;
				pthread_t threadEnemigo;
				pthread_create(&threadEnemigo, NULL, &enemigoActivo, parametrosEnemigo);
				pthread_detach(threadEnemigo);
			}
		}
		usleep(50000);
	}
	parametrosEnemigo->~ParametrosMovimiento();
}

void iniciarThreadMovimientoJugador(Servidor* servidor, string nombre)
{
	pthread_mutex_lock(&servidor->mutexVectorJugadores);
	Jugador* jugador = servidor->obtenerJugador(nombre);
	pthread_mutex_unlock(&servidor->mutexVectorJugadores);
	ParametrosMovimiento * parametros = new ParametrosMovimiento(servidor,jugador);/*ParametrosActPosicion parametros;
	parametros.jugador = jugador;
	parametros.servidor = this;*/
	pthread_t threadEnemigosPetutos;
	pthread_t threadMov = jugador->getThreadMovimiento();
	pthread_create(&threadMov, NULL, &actualizarPosicionesJugador, parametros);
	pthread_detach(threadMov);

	pthread_create(&threadEnemigosPetutos, NULL, &controlDeEnemigos, parametros);
	pthread_detach(threadEnemigosPetutos);
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
			if (servidor->escuchando) {cout << "Servidor escuchando" << endl;}
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
