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
#define MAX_CANT_CLIENTES 6
#define BUFFER_MAX_SIZE 1024
using namespace std;

class Servidor {

private:
	int puerto;
	int welcomeSocket;
	int socketServer;
	int cantClientesConectados = 0;
	char* nombreArchivo;
	queue<Mensaje> colaMensajesNoProcesados;
	list<Mensaje> listaMensajesProcesados;
	Logger* logger;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	list<string> usuarios;
	struct Datos{
				string nombre;
				string contrasenia;
			};
	list<Datos>* datosUsuarios;

public:
	Servidor();
	Servidor(char* nombreArchivoDeUsuarios, int puerto, int modoLogger);
	virtual ~Servidor();
	void guardarDatosDeUsuarios();
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	void autenticar(string nombre, string contrasenia, list<string>& usuarios);
	list<Cliente> obtenerClientes();
	void guardarLog(stringstream &mensaje);
	list<Mensaje> obtenerMensajes(Cliente cliente);
	void crearMensaje(Mensaje mensaje);
	void comenzarEscucha();
	void finalizarEscucha();
	queue<Mensaje> getColaMensajesNoProcesados();
	void splitDatos(char* datos,string* nombre,string* pass);
	void recibirMensaje();
	void setThreadProceso(pthread_t thrProceso);
	int aceptarConexion();
	int getCantConexiones();
	string serializarLista(list<string> datos);
	pthread_mutex_t mutex;
	stringstream mensajeStream;
	list<string> agregarDestinatarios(char* remitente);

};

#endif /* TP1_SERVIDOR_H_ */
