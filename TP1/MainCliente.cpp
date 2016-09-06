/*
 * mainCliente.cpp
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
#include <netdb.h>
#include "Cliente.h"

using namespace std;

bool chequearSocket(string ip, int puerto) {
	//string ipServer = "192.168.1.10";
	string ipServer = "127.0.0.1";
	int puertoDeEscucha = 7891;

	return (ip == ipServer && puerto == puertoDeEscucha);
}

void* cicloConexion(void* arg) {
	//Funcion que cicla para las opciones del cliente dentro del thread de comunicacion. Devuelve 1 si la opcion es desconectar, 0 si es salir.
	Cliente* cliente = (Cliente*) arg;
	string user, pass;
	while (cliente->getClientesDisponibles().empty())
	{
		cout << "Ingrese el nombre del usuario: ";
		cin >> user;
		cout << "Ingrese la password: ";
		cin >> pass;
		cliente->conectar(user, pass);
	}
	while (cliente->getOpcionMenu() != 5 and cliente->getOpcionMenu() != 4) //mientras la opcion del menu no sea salir o desconectar..
	{
		cliente->mostrarMenuYProcesarOpcion();
	}
	if (cliente->getOpcionMenu() == 4) {
		return (void*) 1;
	}
	return (void*) 0;
}

int main() {
	char* ip;
	int puerto, accion;
	bool socketOk = false;
	pthread_t thrComu;
	string nombre, contrasenia;
	while (!socketOk) {
		cout << "Ingrese el puerto para la conexion: ";
		cin >> puerto;
		cout << "Ingrese la ip del servidor: ";
		cin >> ip;
		socketOk = chequearSocket(ip, puerto); //FALTA IMPLEMENTAR METODO DE CHEQUEAR IP/PUERTO. ESTA MAS ABAJO LA FUNCION.
		if (!socketOk) {
			//SI NO SE PUDO COMPROBAR LA CONEXION, DOY ERROR Y VUELVO A PEDIR IP Y CONEXION
			cout << "Error: La direccion de ip o el puerto no permiten esta conexion." << endl;
		}
	}
	cout << "SOCKET OK" << endl;
	Cliente* cliente = new Cliente(ip, puerto);
	cout << "SISTEMA DE MENSAJERIA" << endl;

	do
	{
		do {
			cout << "1) Conectar" << endl;
			cout << "2) Salir" << endl;
			cout << "Que desea hacer? " << endl;
			cin >> accion;
		} while (accion < 1 or accion > 2);
		if (accion != 2) {
			//si no es salir, creo el thread de comunicacion que intenta conectar.
			int threadOk = pthread_create(&thrComu, NULL, &cicloConexion, cliente);
			if (threadOk != 0) {
				cout << "Error al inicializar la conexion." << endl;
			}
			else
			{
				cliente->setThreadComunicacion(thrComu);
				void** resultado;
				pthread_join(cliente->getThreadComunicacion(), (void**) &resultado); //espero que termine el thread de comunicacion que fue invocado..
				accion = *((int*) (&resultado));
				if (accion == 1) //si es 1, es desconectar y vuelve a ingresar al loop que ofrece conectar y desconectar
				{
					cout << "Desconectado del servidor.." << endl;
				}//si es 0, va a salir automaticamente del loop y del programa.
			}
		}
		else{
			accion = 0; //si la accion es 2, la pongo en 0 para que salga del while
		}
	}while (accion != 0); //si la accion es 0, es salir.
	cliente->salir(); //cierra el socket y realiza trabajos de limpieza de memoria
	cout << "Saliendo del programa.. Gracias, vuelva prontoss!" << endl;
	return 0;
}
