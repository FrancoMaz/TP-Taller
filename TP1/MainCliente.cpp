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
#include "Handshake.h"

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

int getCantidadDeFotogramas(string spriteAEjecutar, Handshake* handshake)
{
	list<SpriteDto*> listaSprites = handshake->getSprites1()->getSprites();
	cout << "path: " << handshake->getImagen1()->getPath() << endl;
	int cantFotogramas = 0;
	for (list<SpriteDto*>::iterator spriteActual = listaSprites.begin();
			spriteActual != listaSprites.end();spriteActual++)
	{
		SpriteDto* sprite;
		sprite = *spriteActual;
		cout << "id: " << sprite->getId() << endl;
		if ((spriteAEjecutar + ".png") == string(sprite->getId()))
		{
			cantFotogramas = atoi(sprite->getCantidadDeFotogramas());
		}
	}
	return cantFotogramas;
}

void procesarUltimosMensajes(string mensajes, Cliente* cliente)
{   string mensajeVacio = "#noHayMensajes@";
	//cout << "Ultimos mensajes recibidos: " << endl;
	if(strcmp(mensajes.c_str(), mensajeVacio.c_str()) != 0){
		//cout<<"No hay mensajes nuevos"<<endl;}
	//else{
		//cout << mensajes << endl;
		mensajes[mensajes.length() - 1] = '#';
		char str[mensajes.length()];
		strcpy(str, mensajes.c_str());
		char* texto = strtok(str, "|");
		// hacer que no imprima arroba
		while (texto != NULL) {
			char* remitente = texto;
			texto = strtok(NULL,"|");
			char* destinatario = texto;
			texto = strtok(NULL,"|");
			char* x = texto;
			texto = strtok(NULL,"|");
			char* y = texto;
			texto = strtok(NULL,"|");
			char* spriteAEjecutar = texto;
			texto = strtok(NULL,"#");
			char* condicion = texto;
			texto = strtok(NULL,"|");
			string handshakeRecibido = cliente->getHandshakeRecibido();
			Handshake* handshakeDeserializado = cliente->deserializarHandshake(handshakeRecibido, false);
			list<SpriteDto*> listaSprites = handshakeDeserializado->getSprites1()->getSprites();
			int cantidadDeFotogramas = 0;
			for (list<SpriteDto*>::iterator spriteActual = listaSprites.begin();
					spriteActual != listaSprites.end();spriteActual++)
			{
				SpriteDto* sprite;
				sprite = *spriteActual;
				if ((string(spriteAEjecutar) + ".png") == string(sprite->getId()))
				{
					cantidadDeFotogramas = atoi(sprite->getCantidadDeFotogramas());
				}
			}
			//int cantidadDeFotogramas = getCantidadDeFotogramas(string(spriteAEjecutar), handshakeDeserializado);
			cout<<"Mensaje de "<<remitente<<":"<<endl;
			cout << "Posicion x: " << x << endl;
			cout << "Posicion y: " << y << endl;
			cout << "Sprite a ejecutar: " << spriteAEjecutar << endl;
			cout << "Condicion: " << condicion << endl;
			cout << "CantidadDeFotogramas: " << cantidadDeFotogramas << endl;

			vista->actualizarJugador(string(remitente),atoi((string(x).c_str())),atoi(string(y).c_str()), string(spriteAEjecutar), string(condicion), cantidadDeFotogramas);
		}
	}
}

void* recibirPosicionJugadores(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	string datosRecibidos;
	while(!controlador->comprobarCierreVentana()){
		datosRecibidos = cliente->recibir();
		procesarUltimosMensajes(datosRecibidos, cliente);
		usleep(1/1000);
	}
}

void* enviarEventos(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	while(!controlador->comprobarCierreVentana()){
		while(SDL_PollEvent(&evento)){
			usleep(100000);
			if(controlador->presionarBoton(SDLK_RIGHT)){
				cliente->enviar("Tecla Derecha","Todos");
			}
			else if(controlador->presionarBoton(SDLK_LEFT)){
				cliente->enviar("Tecla Izquierda","Todos");
			}
			else if(controlador->presionarBoton(SDLK_UP)){
				cliente->enviar("Tecla Arriba","Todos");
			}
			else if(controlador->soltarBoton(SDLK_RIGHT)){
				cliente->enviar("Soltar Tecla Derecha","Todos");
			}
			else if(controlador->soltarBoton(SDLK_LEFT)){
				cliente->enviar("Soltar Tecla Izquierda","Todos");
			}
			SDL_FlushEvent(SDL_MOUSEMOTION);
			SDL_FlushEvent(SDL_KEYDOWN);
		}
	}
}

void* cicloConexion(void* arg) {
	//Funcion que cicla para las opciones del cliente dentro del thread de comunicacion. Devuelve 1 si la opcion es desconectar, 0 si es salir.
	Cliente* cliente = (Cliente*) arg;
	pthread_t threadVerificarConexion;
	pthread_t threadEnviarEventos;
	pthread_t threadRecibirPosicionJugadores;
	ComunicacionCliente comunicacion;
	comunicacion.cliente =cliente;
	comunicacion.termino = false;

	bool termino = false;
	bool datosIncorrectos = false;
	bool conexion = false;
	while ((!conexion)&&(!vista->ventanaCerrada())) {
		datosCliente = vista->cargarTerceraPantalla(datosIncorrectos);
		if((datosCliente.nombre != " ")&&(datosCliente.contrasenia != " ")){
			conexion = cliente->conectar(datosCliente.nombre, datosCliente.contrasenia);
		}
		//datosIncorrectos = true;
	}

	if (!vista->ventanaCerrada()) {
		//se crea esta hilo para poder verificar la conexion con el servidor
		//pthread_create(&threadVerificarConexion, NULL,&verificarConexion,&comunicacion);
		//pthread_detach(threadVerificarConexion);
		bool inicio;
		do
		{
			usleep(2000000);
			inicio = cliente->checkearInicioJuego(vista);
		}while (!inicio);
		pthread_create(&threadEnviarEventos, NULL, &enviarEventos, cliente);
		pthread_detach(threadEnviarEventos);
		pthread_create(&threadRecibirPosicionJugadores, NULL, &recibirPosicionJugadores,cliente);
		pthread_detach(threadRecibirPosicionJugadores);
		vista->cargarEscenario();

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
