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
	//Funcion que cicla para las opciones del cliente dentro del thread de comunicacion. Devuelve 0 si la opcion es desconectar, 1 si es salir.
	Cliente cliente = *(Cliente*) arg;
	while (cliente.getOpcionMenu() != 5 and cliente.getOpcionMenu() != 4) //mientras la opcion del menu no sea salir o desconectar..
	{
		cliente.mostrarMenuYProcesarOpcion();
	}
	if (cliente.getOpcionMenu() == 4) {
		return (void*) 0;
	}
	return (void*) 1;
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
			cout
					<< "Error: La direccion de ip o el puerto no permiten esta conexion."
					<< endl;
		}
	}
	cout << "SOCKET OK" << endl;
	Cliente* cliente = new Cliente(ip, puerto);
	cout << "SISTEMA DE MENSAJERIA" << endl;

	do {
		cout << "1) Conectar" << endl;
		cout << "2) Salir" << endl;
		cout << "Que desea hacer? " << endl;
		cin >> accion;
	} while (accion < 1 or accion > 2);

	if (accion == 2) {
		//si la accion es salir, devuelvo 0 y salgo del programa.
		cout << "Saliendo del programa.. Gracias, vuelva prontoss!" << endl;
		return 0;
	} else {
		string user, pass;
		cout << "Ingrese el nombre del usuario: ";
		cin >> user;
		cout << "Ingrese la password: ";
		cin >> pass;
		cliente->conectar(user, pass);
	}

	//si no es salir, intento conectar pidiendo usuario y contraseña.
	/*
	 * while (cliente->getClientesDisponibles().empty())
	 {
	 cout << "Ingrese nombre de usuario: ";
	 cin >> nombre;
	 cout << "Ingrese contrasenia: ";
	 cin >> contrasenia;
	 cliente->setClientesDisponibles(nombre,contrasenia);
	 if (cliente->getClientesDisponibles().empty())
	 {
	 //si despues de intentar conectar, sigue siendo una lista vacia, muestro error y pido nuevamente.
	 cout << "Error al intentar autenticar. Ingrese un nombre de usuario y una contrasenia validos." << endl;
	 }
	 }
	 */
	/*int threadOk = pthread_create(&thrComu, NULL, &cicloConexion, cliente);
	if (threadOk != 0) {
		cout << "Error al inicializar la conexion." << endl;
	} else {
		cliente->setThreadComunicacion(thrComu);
		void** resultado;
		pthread_join(cliente->getThreadComunicacion(), (void**) &resultado);
		int salir = *((int*) (&resultado));
		if (salir == 0) //si es 0, es desconectar
				{
			cout << "Desconectado del servidor.." << endl;
		} else {
			cout << "Saliendo del programa.. Gracias, vuelva prontoss!" << endl;
			return 0;
		}
	}*/
	return 0;
}
