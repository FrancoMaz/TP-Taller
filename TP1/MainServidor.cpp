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


struct parametrosThreadCliente {
	//estructura que sirve para guardar los parametros que se le pasan a la funcion del thread.
	Servidor* serv;
	int socketCli;
};

struct parametrosThreadEncolarMensaje{
	Mensaje* mensajeNoProcesado;
	Servidor* servidor;
};

bool stringTerminaCon (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void* encolar(void* arg){
	parametrosThreadEncolarMensaje parametrosEncolarMensaje = *(parametrosThreadEncolarMensaje*) arg;
	pthread_mutex_lock(&parametrosEncolarMensaje.servidor->mutex);
	parametrosEncolarMensaje.servidor->crearMensaje(*parametrosEncolarMensaje.mensajeNoProcesado);
	pthread_mutex_unlock(&parametrosEncolarMensaje.servidor->mutex);
	return NULL;
}

void encolarMensaje(char* remitente, char* destinatario, char* mensaje, Servidor* servidor)
{
	pthread_t threadEncolarMensaje;
	parametrosThreadEncolarMensaje parametrosEncolarMensaje;
	parametrosEncolarMensaje.mensajeNoProcesado = new Mensaje(remitente,destinatario,mensaje);
	parametrosEncolarMensaje.servidor = servidor;
	cout << "Encolando mensaje: " + std::string(mensaje) + ". De: " + std::string(remitente) + ". Para: " + std::string(destinatario) + "." << endl;
	servidor->mensajeStream << "Encolando mensaje: " + std::string(mensaje) + ". De: " + std::string(remitente) + ". Para: " + std::string(destinatario) + ". \n";
	servidor->guardarLog(servidor->mensajeStream);
	//pthread_detach(threadEncolarMensaje); //lo marco
	pthread_create(&threadEncolarMensaje,NULL,&encolar,&parametrosEncolarMensaje);
}

void* cicloEscuchaCliente(void* arg) {
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*) arg;
	Servidor* servidor = parametros->serv;
	int socketCliente = parametros->socketCli;
	char bufferRecibido[BUFFER_MAX_SIZE];
	while (1) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		string datosRecibidos;
		int largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE, 0); //recibo por primera vez
		datosRecibidos.append(bufferRecibido,largoRequest);
		cout << largoRequest << endl;
		while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos,"#"))
		{
			//mientras haya cosas que leer, sigo recibiendo.
			largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE, 0);
			datosRecibidos.append(bufferRecibido,largoRequest);
		}
		//en el formato siempre recibimos primero el metodo primero que es un entero.
		//1 representa enviar un mensaje, 2 representa recibir mis mensajes, 3 desconectar.
		char* metodo = strtok(strdup(datosRecibidos.c_str()),"|");
		int accion = atoi(metodo); //convierto a entero el metodo recibido por string
		switch (accion){
		case 1:{ //1 es enviar
			char* remitente = strtok(NULL,"|");
			char* destinatario = strtok(NULL,"|");
			char* mensaje = strtok(NULL,"#");
			encolarMensaje(remitente, destinatario, mensaje,servidor);
			break;
		}
		case 2: { //2 es recibir
			char* usuarioQueSolicita = strtok(NULL,"|");
			break;
		}
		case 3: { //3 es desconectar
			break;
		}

		}
	}
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	int puerto;
	string archivoUsers;
	Servidor* servidor;
	int modoLogger;
	cout << "Ingrese el puerto de escucha de la conexion: " << endl;
	cin >> puerto;
	cout << "Ingrese el nombre del archivo de usuarios: " << endl;
	cin >> archivoUsers;
	cout << "Elija el modo en el que quiere loggear los eventos: " << endl;
	cout << "1) INFO" << endl;
	cout << "2) DEBUG" << endl;
	cin >> modoLogger;

	char* archivo = strdup(archivoUsers.c_str());
	do {
		servidor = new Servidor(archivo, puerto, modoLogger);
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
