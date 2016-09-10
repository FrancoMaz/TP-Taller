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

using namespace std;

pthread_mutex_t mutex;

struct parametrosThreadCliente {
	//estructura que sirve para guardar los parametros que se le pasan a la funcion del thread.
	Servidor* serv;
	int socketCli;
};

struct parametrosThreadEncolarMensaje{
	Mensaje* mensajeNoProcesado;
	Servidor* servidor;
};

void* encolarMensaje(void* arg){
	parametrosThreadEncolarMensaje parametrosEncolarMensaje = *(parametrosThreadEncolarMensaje*) arg;
	pthread_mutex_lock(&mutex);
	parametrosEncolarMensaje.servidor->crearMensaje(*parametrosEncolarMensaje.mensajeNoProcesado);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* cicloEscuchaCliente(void* arg) {
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*) arg;
	Servidor* servidor = parametros->serv;
	int socketCliente = parametros->socketCli;
	char datosRecibidos[1024];
	pthread_t threadEncolarMensaje;
	while (1) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		recv(socketCliente, datosRecibidos, 1024, 0);

		//el formato del mensaje recibido en datosRecibidos es el siguiente:
		//largodelmensaje | metodo (puede ser un numero) | remitente | destinatario | mensaje #
		//A continuación hago un split de cada campo
		char* largoDelMensaje = strtok(datosRecibidos,"|");
		char* metodo = strtok(NULL,"|");
		char* remitente = strtok(NULL,"|");
		char* destinatario = strtok(NULL,"|");
		char* mensaje = strtok(NULL,"#");

		parametrosThreadEncolarMensaje parametrosEncolarMensaje;
		parametrosEncolarMensaje.mensajeNoProcesado = new Mensaje(remitente,destinatario,mensaje);
		parametrosEncolarMensaje.servidor = servidor;

		pthread_create(&threadEncolarMensaje,NULL,&encolarMensaje,&parametrosEncolarMensaje);
	}
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	int puerto;
	string archivoUsers;
	Servidor* servidor;
	cout << "Ingrese el puerto de escucha de la conexion: " << endl;
	cin >> puerto;
	cout << "Ingrese el nombre del archivo de usuarios: " << endl;
	cin >> archivoUsers;
	char* archivo = strdup(archivoUsers.c_str());
	do {
		servidor = new Servidor(archivo, puerto);
		servidor->comenzarEscucha();
	} while (!servidor->escuchando);

	pthread_t thread_id[MAX_CANT_CLIENTES]; //la cantidad maxima de clientes es 6, voy a crear, como mucho 6 threads para manejar dichas conexiones.
	for (int i = 0; i < MAX_CANT_CLIENTES; i++) {
		thread_id[i] = NULL; //inicializo todos en null
	}
	while (1) {
		//se va a generar un thread para que cada cliente tenga su comunicacion con el servidor.
		int socketCliente;
		socketCliente = servidor->aceptarConexion();
		//genero un nuevo thread dinamicamente para este cliente
		if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES) {
			cout << "Te conecte" << endl;
			//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
			parametrosThreadCliente parametrosCliente;
			parametrosCliente.socketCli = socketCliente;
			parametrosCliente.serv = servidor;
			pthread_detach(thread_id[servidor->getCantConexiones()]); //lo marco como detach

			pthread_create(&thread_id[servidor->getCantConexiones()], NULL,
					&cicloEscuchaCliente, &parametrosCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
		}
	}
}
int inicializarServidor() {
	pthread_t thrProceso;
	int threadOk = pthread_create(&thrProceso, NULL, &cicloEscucharConexionesNuevasThreadProceso, NULL);
	return threadOk;
}

int main() {
	Servidor* servidor;

	int ok = inicializarServidor();
	if (ok != 0) {
		cout << "Error inicializando el thread de proceso" << endl;
	} else {
		cout << "Thread inicializado OK" << endl;
	}
	while (1) {
		sleep(5); //esto esta a modo de prueba para que no corte los otros threads.
	}
	return 0;
}
