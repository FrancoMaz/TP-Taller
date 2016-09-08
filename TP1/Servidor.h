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
#include "Cliente.h"
#include "Mensaje.h"
#include "Log.h"
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
	Log archivoLog;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	list<string> usuarios;

public:
	Servidor();
	Servidor(char* nombreArchivoDeUsuarios, int puerto);
	virtual ~Servidor();
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	void autenticar(string nombre, string contrasenia, list<string>& usuarios);
	list<Cliente> obtenerClientes();
	void guardarLog();
	list<Mensaje> obtenerMensajes(Cliente cliente);
	void crearMensaje(Mensaje mensaje);
	void comenzarEscucha();
	void aceptarConexiones();
	void finalizarEscucha();
	queue<Mensaje> getColaMensajesNoProcesados();
	void splitDatos(char* datos,string* nombre,string* pass);
	void recibirMensaje();
	void setThreadProceso(pthread_t thrProceso);
	int aceptarConexion();
	int getCantConexiones();
	const char* serializarLista(list<string> datos);
};

#endif /* TP1_SERVIDOR_H_ */
