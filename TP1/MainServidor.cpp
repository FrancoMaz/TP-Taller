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

int main() {
	Servidor* servidor;
	pthread_t threadListen;

	do{
		servidor = inicializarServidor();
	}
	while(!servidor->escuchando); //mientras el servidor no este escuchando, intento inicializarlo pidiendo los parametros de entrada.

	do{

	} while(servidor->escuchando);

	string user, pass;
	cout << "Ingrese el nombre del usuario: ";
	cin >> user;
	cout << "Ingrese la password: ";
	cin >> pass;
	servidor->autenticar(user, pass);
	return 0;
}
