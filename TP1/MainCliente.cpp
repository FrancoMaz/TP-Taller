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

struct ComunicacionCliente{
			Cliente* cliente;
			bool termino;
		};

bool chequearSocket(string ip, int puerto) {
	//string ipServer = "192.168.1.11";
	string ipServer = "127.0.0.1";
	int puertoDeEscucha = 7891;

	return (ip == ipServer && puerto == puertoDeEscucha);
}
void* verificarConexion(void * arg){
	ComunicacionCliente* comunicacion = (ComunicacionCliente*)arg;
	Cliente* cliente = comunicacion->cliente;
	cliente->corroborarConexion();
    //comunicacion->termino = cliente->corroborarConexion();
}

void* cicloConexion(void* arg) {
	//Funcion que cicla para las opciones del cliente dentro del thread de comunicacion. Devuelve 1 si la opcion es desconectar, 0 si es salir.
	Cliente* cliente = (Cliente*) arg;
	string user, pass;
	pthread_t threadVerificarConexion;
	ComunicacionCliente comunicacion;
	comunicacion.cliente =cliente;
	comunicacion.termino = false;

	bool termino = false;
	while (cliente->getClientesDisponibles().empty()) {
		cout << "Ingrese nombre de usuario: ";
		cin >> user;
		cout << "Ingrese password: ";
		cin >> pass;
		cliente->conectar(user, pass);
	}
	//se crea esta hilo para poder verificar la conexion con el servidor
	pthread_create(&threadVerificarConexion, NULL,&verificarConexion,&comunicacion);
	pthread_detach(threadVerificarConexion);
	 //void** escuchando;
	 //pthread_join(threadVerificarConexion,(void**)&escuchando);
	 //termino = *((bool*) (&escuchando));
	cliente->setOpcionMenu(0);
	while (cliente->getOpcionMenu() != 5 and cliente->getOpcionMenu() != 4 and !cliente->getTermino()) {
		cliente->mostrarMenuYProcesarOpcion();
		if (!cliente->getTermino()) {
			cliente->elegirOpcionDelMenu(cliente->getOpcionMenu());
		}
	}
	if (cliente->getTermino() and cliente->getOpcionMenu() != 5)
	{
		int opcion;
		while (opcion != 5) {
			cout << endl;
			cout << "Se cerro la conexion con el servidor. Presione 5 para salir" << endl;
			cin >> opcion;
			}
			cliente->setOpcionMenu(5);
		}

		//if(accion == 5){ cliente->setOpcionMenu(accion);}

	if (cliente->getOpcionMenu() == 4) {
		return (void*) 1;
	}
	//pthread_detach(threadVerificarConexion);
	return (void*) 0;
}

int main() {
	bool esValido = false;
	string ip;
	int puerto, accion;
	bool socketOk = false;
	pthread_t thrComu;
	string nombre, contrasenia;
	while (!socketOk) {
		do {
			cout << "Ingrese el puerto para la conexion: ";
			cin >> puerto;
			if (cin.good()) {esValido = true;}
			else {
				cin.clear();
				cin.ignore();
				cout << "Error: el dato ingresado debe ser un numero" << endl;
				}
			}
		while (!esValido);
		cout << "Ingrese la ip del servidor: ";
		cin >> ip;
		socketOk = chequearSocket(ip, puerto); //FALTA IMPLEMENTAR METODO DE CHEQUEAR IP/PUERTO. ESTA MAS ABAJO LA FUNCION.
		if (!socketOk) {
			//SI NO SE PUDO COMPROBAR LA CONEXION, DOY ERROR Y VUELVO A PEDIR IP Y CONEXION
			cout << "Error: La direccion de ip o el puerto no permiten esta conexion." << endl;
		}
	}
	cout << "Socket OK" << endl;
	Cliente* cliente = new Cliente(ip, puerto);
	cout << "Bienvenido al sistema de mensajería" << endl;

	do {
			bool accionValida = false;
			cout << "1) Conectar" << endl;
			cout << "2) Salir" << endl;
		do {
			cout << "¿Qué desea hacer? " << endl;
			cin >> accion;
			if (cin.good() && (accion == 1 || accion == 2)) {accionValida = true;}
				else {
					cin.clear();
					cin.ignore();
					cout << "Error: la opcion ingresada no es valida" << endl;
					}
		} while (!accionValida);
		if (accion != 2) {
			//si no es salir, creo el thread de comunicacion que intenta conectar.
			int threadOk = pthread_create(&thrComu, NULL, &cicloConexion,cliente);
			if (threadOk != 0) {
				cout << "Error al inicializar la conexión." << endl;
			} else {
				cliente->setThreadComunicacion(thrComu);
				void** resultado;
				pthread_join(cliente->getThreadComunicacion(),
						(void**) &resultado); //espero que termine el thread de comunicacion que fue invocado..
				accion = *((int*) (&resultado));
				if (accion == 1) { //si es 1, es desconectar y vuelve a ingresar al loop que ofrece conectar y desconectar
					cout << "Desconectado del servidor.." << endl;
					cliente->vaciarClientesDisponibles();
				} //si es 0, va a salir automaticamente del loop y del programa.
			}

		} else {
			accion = 0; //si la accion es 2, la pongo en 0 para que salga del while
		}
	} while (accion != 0); //si la accion es 0, es salir.
	cliente->salir(); //cierra el socket y realiza trabajos de limpieza de memoria
	cout << "Saliendo del programa..." << endl;
	return 0;
}
