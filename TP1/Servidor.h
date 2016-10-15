/*
 * Servidor.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_SERVIDOR_H_
#define TP1_SERVIDOR_H_

#include <string>
#include <iostream>
#include <list>
#include <queue>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include "Cliente.h"
#include "Logger.h"
#include "Mensaje.h"
#include "Jugador.h"
#define MAX_CANT_CLIENTES 6
#define BUFFER_MAX_SIZE 200
const int INFO = 1;
const int DEBUG = 2;

using namespace std;

class Servidor {

private:
	int puerto;
	int welcomeSocket;
	int socketServer;
	int cantClientesConectados = 0;
	char* nombreArchivo;
	queue<Mensaje> colaMensajesNoProcesados;
	Logger* logger;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	list<string> usuarios;
	struct Datos {
		string nombre;
		string contrasenia;
	};
	struct MensajesProcesados {
		string destinatario;
		Jugador* jugador;
		queue<Mensaje>* mensajes;
	};
	list<Datos>* datosUsuarios;
	list<MensajesProcesados>* listaMensajesProcesados;

public:
	Servidor();
	Servidor(char* nombreArchivoDeUsuarios, int puerto, Logger* logger);
	virtual ~Servidor();
	void guardarDatosDeUsuarios();
	bool fileExists(string fileName);
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	void autenticar(string nombre, string contrasenia, list<string>& usuarios);
	list<Cliente> obtenerClientes();
	void guardarLog(string mensaje, const int nivelDeLog);
	list<Mensaje> obtenerMensajes(Cliente cliente);
	void crearMensaje(Mensaje mensaje);
	void comenzarEscucha();
	void finalizarEscucha();
	queue<Mensaje> getColaMensajesNoProcesados();
	void splitDatos(char* datos, string* nombre, string* pass);
	void recibirMensaje();
	void setThreadProceso(pthread_t thrProceso);
	pair<int,string> aceptarConexion();
	int getCantConexiones();
	string serializarLista(list<string> datos);
	pthread_mutex_t mutexColaNoProcesados;
	pthread_mutex_t mutexListaProcesados;
	pthread_mutex_t mutexColasProcesadas;
	pthread_mutex_t mutexEnviarMensajes;
	string mensaje;
	list<string> agregarDestinatarios(string remitente);
	void procesarMensaje(Mensaje mensaje);
	string traerMensajesProcesados(char* nombreCliente);
	string concatenarMensajes(queue<Mensaje>* colaDeMensajes);
	void procesarMensajes();
	void restarCantidadClientesConectados();
};

#endif /* TP1_SERVIDOR_H_ */
