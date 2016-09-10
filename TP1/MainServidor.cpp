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

void* encolarMensaje(void* arg){
	Mensaje mensaje = *(Mensaje*) arg; //No sé si esto funciona bien pero al menos compila
	pthread_mutex_lock(&mutex);
	//servidor->crearMensaje(mensaje);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* cicloEscuchaCliente(void* arg) {
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*) arg;
	Servidor* servidor = parametros->serv;
	int socketCliente = parametros->socketCli;
	char buffer[1024];
	char datosRecibidos[1024];
	//char* mensaje = "SERVER: Te respondo";
	pthread_t threadEncolarMensaje;
	while (1) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		recv(socketCliente, datosRecibidos, 1024, 0);

		//largodelmensaje | metodo (puede ser un numero) | remitente | destinatario | mensaje #
		char* largoDelMensaje = strtok(datosRecibidos,"|");
		char* metodo = strtok(NULL,"|");
		char* remitente = strtok(NULL,"|");
		char* destinatario = strtok(NULL,"|");
		char* mensaje = strtok(NULL,"#");

		//Puede que después sea necesario liberar memoria acá, ya que estamos creando objetos en un ciclo
		Mensaje* mensajeNoProcesado = new Mensaje(remitente,destinatario,mensaje);

		//Debería crear una estructura para pasarla como argumento al thread
		//Que contenga mensajeNoProcesado, y el servidor para invocar el crearMensaje

		pthread_create(&threadEncolarMensaje,NULL,&encolarMensaje,&mensajeNoProcesado);
		//cout << "Recibi: " << datosRecibidos << endl;
		//strcpy(buffer, mensaje);
		//send(socketCliente, buffer, strlen(mensaje) + 1, 0);
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
		//aca deberia verificar los datos del nombre de archivo usuarios y del puerto,
		//para ver si mato al servidor o no.
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
			cout<< "cantidad de clientes: "<<servidor->getCantConexiones()<<endl;
			//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
			parametrosThreadCliente parametrosCliente;
			parametrosCliente.socketCli = socketCliente;
			parametrosCliente.serv = servidor;
			pthread_create(&thread_id[servidor->getCantConexiones()], NULL, &cicloEscuchaCliente, &parametrosCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
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
