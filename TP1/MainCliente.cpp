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
#include "Vista.h"
#include "Controlador.h"

using namespace std;

datosConexion datosCliente;
Vista * vista = new Vista();
Controlador* controlador = new Controlador();

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

void* recibirPosicionJugadores(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	while(!controlador->comprobarCierreVentana()){
		cliente -> recibir();
		//usleep(5000000);
	}
}

void* enviarEventos(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	while(!controlador->comprobarCierreVentana()){
		while(SDL_PollEvent(&evento)){
			if(controlador->presionarBoton(SDLK_RIGHT)){
				cliente->enviar("Tecla Derecha");
			}
			if(controlador->presionarBoton(SDLK_LEFT)){
				cliente->enviar("Tecla Izquierda");
			}
			if(controlador->presionarBoton(SDLK_UP)){
				cliente->enviar("Tecla Arriba");
			}
			if(controlador->soltarBoton(SDLK_RIGHT)){
				cliente->enviar("Soltar Tecla Derecha");
			}
			if(controlador->soltarBoton(SDLK_LEFT)){
				cliente->enviar("Soltar Tecla Izquierda");
			}
		}
	}
}

void* cicloConexion(void* arg) {
	//Funcion que cicla para las opciones del cliente dentro del thread de comunicacion. Devuelve 1 si la opcion es desconectar, 0 si es salir.
	Cliente* cliente = (Cliente*) arg;
	pthread_t threadVerificarConexion;
	pthread_t threadRecibirPosicionJugadores;
	pthread_t threadEnviarEventos;
	ComunicacionCliente comunicacion;
	comunicacion.cliente =cliente;
	comunicacion.termino = false;

	bool termino = false;
	bool datosIncorrectos = false;
	while ((cliente->getClientesDisponibles().empty())&&(!vista->ventanaCerrada())) {
		datosCliente = vista->cargarTerceraPantalla(datosIncorrectos);
		if((datosCliente.nombre != " ")&&(datosCliente.contrasenia != " ")){
			cliente->conectar(datosCliente.nombre, datosCliente.contrasenia);
		}
		datosIncorrectos = true;
	}

	if (!vista->ventanaCerrada()) {
		//se crea esta hilo para poder verificar la conexion con el servidor
		//pthread_create(&threadVerificarConexion, NULL,&verificarConexion,&comunicacion);
		//pthread_detach(threadVerificarConexion);
	 	 //void** escuchando;
	 	 //pthread_join(threadVerificarConexion,(void**)&escuchando);
	 	 //termino = *((bool*) (&escuchando));
		pthread_create(&threadEnviarEventos, NULL, &enviarEventos, cliente);
		//pthread_join(threadEnviarEventos, NULL);

		pthread_create(&threadRecibirPosicionJugadores, NULL, &recibirPosicionJugadores, cliente);
		//pthread_join(threadRecibirPosicionJugadores, NULL);
		pthread_detach(threadEnviarEventos);
		pthread_detach(threadRecibirPosicionJugadores);

		vista->cargarEscenario();

		/*if (!vista->ventanaCerrada()) {
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
		} else {
			return (void*) 1;
		}
	} else {
		return (void*) 1;*/
	}
}

int main() {
	bool esValido = false;
	bool socketOk = false;
	pthread_t thrComu;

	if (!vista->inicializar()){
		cout << "El programa no pudo ejecutarse." << endl;
	} else {
		vista->cargarArchivos();
		vista->cargarPrimeraPantalla();

		bool datosIncorrectos = false;
		while ((!socketOk)&&(!vista->ventanaCerrada())) {
			datosCliente = vista->cargarSegundaPantalla(datosIncorrectos);
			socketOk = chequearSocket(datosCliente.ip, atoi(datosCliente.puerto.c_str())); //FALTA IMPLEMENTAR METODO DE CHEQUEAR IP/PUERTO. ESTA MAS ABAJO LA FUNCION.
			datosIncorrectos = true;
		}

		if(!vista->ventanaCerrada()){
			vista->transicionDePantalla();
			cout << "Socket OK" << endl;
			Cliente* cliente = new Cliente(datosCliente.ip, atoi(datosCliente.puerto.c_str()));
			cout << "Bienvenido al sistema de mensajería" << endl;

			//creo el thread de comunicacion que intenta conectar.
			int threadOk = pthread_create(&thrComu, NULL, &cicloConexion,cliente);
			if (threadOk != 0) {
				cout << "Error al inicializar la conexión." << endl;
			} else {
				cliente->setThreadComunicacion(thrComu);
				void** resultado;
				pthread_join(cliente->getThreadComunicacion(),(void**) &resultado); //espero que termine el thread de comunicacion que fue invocado..

				//if (accion == 1) { //si es 1, es desconectar y vuelve a ingresar al loop que ofrece conectar y desconectar
					cout << "Desconectado del servidor.." << endl;
					cliente->vaciarClientesDisponibles();
				//}
			}

			cliente->salir(); //cierra el socket y realiza trabajos de limpieza de memoria
			cout << "Saliendo del programa..." << endl;
		}
	}
	return 0;
}
