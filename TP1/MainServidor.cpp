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

using namespace std;

Servidor* inicializarServidor() {
	int puerto;
	char* archivoUsers;
	cout << "Ingrese el puerto de escucha de la conexion: ";
	cin >> puerto;
	cout << "Ingrese el nombre del archivo de usuarios: ";
	cin >> archivoUsers;
	return new Servidor(archivoUsers, puerto);
}

void* escucharEnvio(void* arg){
	pthread_mutex_lock(&mutex);
	//Enviar
	pthread_mutex_unlock(&mutex);
}

void* escucharRecibir(void* arg){
	pthread_mutex_lock(&mutex);
	//Recibir
	pthread_mutex_unlock(&mutex);
}

int main() {
	Servidor* servidor;
	pthread_t threadEnvio;
	pthread_t threadRecibir;
	pthread_t mutex;
	string variable;

	do {
		servidor = inicializarServidor();
		/*---- Listen on the socket, with 6 max connection requests queued ----*/
		servidor->comenzarEscucha();
		servidor->aceptarConexiones();
	} while (!servidor->escuchando); //mientras el servidor no este escuchando, intento inicializarlo pidiendo los parametros de entrada.



	do{
	//Autenticar/enviar/recibir/desconectar
	//pthread_create(&threadListen,NULL,&clicloEscuchar,NULL);

	//Lo siguiente es un prototipo de la implementación
	switch(variable){
		case "Enviar":{
			pthread_create(&threadEnvio,NULL,&escucharEnvio,NULL);
			pthread_join(threadEnvio,NULL);
			break;
		}
		case "Recibir":{
			pthread_create(&threadRecibir,NULL,&escucharRecibir,NULL);
			pthread_join(threadRecibir,NULL);
			break;
		}
		case "Autenticar":{
			servidor->autenticar("","");
			break;
		}
		case "Desconectar":{
			servidor->finalizarEscucha();
			break;
		}
		default:
			break;
		}

	} while(servidor->escuchando);

	/*
	// esto no va aca!
	string user, pass;
	cout << "Ingrese el nombre del usuario: ";
	cin >> user;
	cout << "Ingrese la password: ";
	cin >> pass;
	servidor->autenticar(user, pass);
	*/

	return 0;
}
