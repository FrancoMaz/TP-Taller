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

Servidor* inicializarServidor() {
	int puerto;
	string archivoUsers;
	cout << "Ingrese el puerto de escucha de la conexion: ";
	cin >> puerto;
	cout << "Ingrese el nombre del archivo de usuarios: ";
	cin >> archivoUsers;
	char* archivo = strdup(archivoUsers.c_str());
	return new Servidor(archivo, puerto);
}

void* escucharEnvio(void* arg){
	Servidor* servidor = (Servidor*) arg;
	pthread_mutex_lock(&mutex);
	cout << "probandoEnvio" << endl;
	//send(servidor->getSocket(), "Bienvenidos", 10, 0);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* escucharRecibir(void* arg){
	Servidor* servidor = (Servidor*) arg;
	pthread_mutex_lock(&mutex);
	cout << "probandoRecibir" << endl;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main() {
	Servidor* servidor;
	pthread_t threadEnvio;
	pthread_t threadRecibir;
	int variable = 1;

	do {
		servidor = inicializarServidor();
		/*---- Listen on the socket, with 6 max connection requests queued ----*/
		servidor->comenzarEscucha();
		servidor->aceptarConexiones();
	} while (!servidor->escuchando);
	//mientras el servidor no este escuchando, intento inicializarlo pidiendo los parametros de entrada.
	servidor->recibirMensaje();
	/*do{
	switch(variable){
		case 1:{	//Enviar
			pthread_create(&threadEnvio,NULL,&escucharEnvio,&servidor);
			pthread_join(threadEnvio,NULL);
			break;
		}
		case 2:{	//Recibir
			pthread_create(&threadRecibir,NULL,&escucharRecibir,NULL);
			pthread_join(threadRecibir,NULL);
			break;
		}
		case 3:{	//Autenticar
			servidor->autenticar("","");
			break;
		}
		case 4:{	//Desconectar
			servidor->finalizarEscucha();
			break;
		}
		default:
			break;
		}
	} while(servidor->escuchando);*/
	return 0;
}
