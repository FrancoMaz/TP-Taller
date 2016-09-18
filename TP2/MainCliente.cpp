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
#include <SDL2/SDL.h>

using namespace std;

//Dimensiones de la pantalla
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
SDL_Window* ventana = NULL;
SDL_Surface* capaPrincipal = NULL;
SDL_Surface* imagen = NULL;

bool inicializarVentana(){
	//Esta función crea la ventana principal del juego con sus resoluciones

	//Condición de inicialización (si la función devuelve false, ocurrió un error)
	bool finalizado = true;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL no pudo inicializarse. Error: " << SDL_GetError() << endl;
		finalizado = false;
	} else {
		ventana = SDL_CreateWindow("Metal Slug (Alpha Version: 0.001)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_VENTANA, ALTO_VENTANA, SDL_WINDOW_SHOWN);
		if (ventana == NULL){
			cout << "La ventana no pudo crearse. Error: " << SDL_GetError() << endl;
			finalizado = false;
		} else {
			capaPrincipal = SDL_GetWindowSurface(ventana);
		}
	}
	return finalizado;
}

bool cargarImagen(){
	//Esta función carga una imagen externa (de una ruta de archivo) a la ventana

	//Condición de inicialización (si la función devuelve false, ocurrió un error)
	bool finalizado = true;
	imagen = SDL_LoadBMP("Recursos/test.bmp");
	if (imagen == NULL){
		cout << "No se pudo cargar la imagen del directorio Recursos/test.jpg" << endl;
		cout << "Error: " << SDL_GetError() << endl;
		finalizado = false;
	}

	return finalizado;
}

void cerrarVentana(){
	//Esta función cierra el SDL, liberando la memoria ocupada tanto por la ventana que por las surface/imágenes
	SDL_DestroyWindow(ventana);
	ventana = NULL;

	SDL_Quit();
}

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
	while (cliente->getClientesDisponibles().empty()) {
		cout << "Ingrese nombre de usuario: ";
		cin >> user;
		cout << "Ingrese password: ";
		cin >> pass;
		cliente->conectar(user, pass);
	}
	do
	{
		cliente->mostrarMenuYProcesarOpcion();
	} while (cliente->getOpcionMenu() != 5 and cliente->getOpcionMenu() != 4); //mientras la opcion del menu no sea salir o desconectar..
	if (cliente->getOpcionMenu() == 4) {
		return (void*) 1;
	}
	return (void*) 0;
}

int main() {
	//Parte gráfica
	if (!inicializarVentana()){
		cout << "Error al inicializar." << endl;
	} else {
		if (!cargarImagen()){
			cout << "Error al cargar la imagen." << endl;
		} else {
			SDL_BlitSurface(imagen,NULL,capaPrincipal,NULL);
			SDL_UpdateWindowSurface(ventana);
			SDL_Delay(5000);
			}
	}

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
			int threadOk = pthread_create(&thrComu, NULL, &cicloConexion,
					cliente);
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

	cerrarVentana();
	return 0;
}
