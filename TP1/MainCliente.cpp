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
#include "UpdateJugador.h"
#include "LTimer.h"
using namespace std;

datosConexion datosCliente;
Vista * vista = new Vista();
Controlador* controlador = new Controlador();
Handshake* handshakeDeserializado;

struct ComunicacionCliente{
			Cliente* cliente;
			bool termino;
		};

bool chequearSocket(string ip, int puerto) {
	string ipServer = "192.168.1.11";
	//string ipServer = "127.0.0.1";
	int puertoDeEscucha = 7891;

	return (ip == ipServer && puerto == puertoDeEscucha);
}
void* verificarConexion(void * arg){
	ComunicacionCliente* comunicacion = (ComunicacionCliente*)arg;
	Cliente* cliente = comunicacion->cliente;
	cliente->corroborarConexion();
    //comunicacion->termino = cliente->corroborarConexion();
}

void procesarUltimosMensajes(string mensajes, Cliente* cliente, UpdateJugador* update)
{   string mensajeVacio = "#noHayMensajes@";
	vector<SetDeSpritesDto*> setsSprites = handshakeDeserializado->getSprites();
	if(strcmp(mensajes.c_str(), mensajeVacio.c_str()) != 0 && mensajes != ""){

		mensajes[mensajes.length() - 1] = '#';
		char str [mensajes.length()];
		strcpy(str, mensajes.c_str());
		char* texto = strtok(str, "|");
		while (texto != NULL) {
			update->setRemitente(string(texto));
			texto = strtok(NULL, "|");
			if (string(texto) == "0"){
				texto = strtok(NULL, "|");
				update->setDestinatario(string(texto));
				texto = strtok(NULL,"|");
				update->setX(string(texto));
				texto = strtok(NULL,"|");
				update->setY(string(texto));
				texto = strtok(NULL,"|");
				char* spriteAEjecutar;
				spriteAEjecutar = texto;
				texto = strtok(NULL,"#");
				update->setCondicion(string(texto));
				for (int i = 0; i < setsSprites.size(); i++){
					vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
					for (int i = 0; i < listaSprites.size(); i++)
					{
						if ((string(spriteAEjecutar)) == listaSprites.at(i)->getId())
						{
							update->setSprite(listaSprites.at(i));
						}
					}
				}
			}
			else{
				texto = strtok(NULL, "|");
				int x = atoi(texto);
				texto = strtok(NULL,"#");
				int y = atoi(texto);
				vista->actualizarCamara(x,y);
			}
			vista->actualizarJugador(update);
			texto = strtok(NULL, "|");
		}
	}
	else{
		vista->actualizarJugador(update);
	}
}

void* recibirPosicionJugadores(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	string datosRecibidos = "";
	UpdateJugador* update = new UpdateJugador();
	 //The frames per second timer
	LTimer capTimer;
	while(!controlador->comprobarCierreVentana()){
		//usleep(50000);
		 //Start cap timer
		capTimer.start();
		datosRecibidos = cliente->recibir();
		procesarUltimosMensajes(datosRecibidos, cliente, update);

		//si se procesa antes, espero lo que tengo que resta.
		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICKS_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}
	}
}

void* enviarEventos(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	bool controlArriba = false;
	 //The frames per second timer
	LTimer fpsTimer;
	//The frames per second cap timer
	LTimer capTimer;
	//Start counting frames per second
	int countedFrames = 0;
	fpsTimer.start();
	while(!controlador->comprobarCierreVentana()){
		while(SDL_PollEvent(&evento)){
			//usleep(50000);
			capTimer.start();
			if(controlador->presionarBoton(SDLK_RIGHT)){
				cliente->enviar("Tecla Derecha","Todos");
			}
			else if(controlador->presionarBoton(SDLK_LEFT)){
				cliente->enviar("Tecla Izquierda","Todos");
			}

			else if(controlador->soltarBoton(SDLK_RIGHT)){
				cliente->enviar("Soltar Tecla Derecha","Todos");
			}
			else if(controlador->soltarBoton(SDLK_LEFT)){
				cliente->enviar("Soltar Tecla Izquierda","Todos");
			}
			else if(controlador->presionarBoton(SDLK_UP)){
				cliente->enviar("Tecla Arriba","Todos");
			}
			SDL_FlushEvent(SDL_MOUSEMOTION);
			SDL_FlushEvent(SDL_KEYDOWN);//si se procesa antes, espero lo que tengo que resta.
			int frameTicks = capTimer.getTicks();
			if( frameTicks < SCREEN_TICKS_PER_FRAME )
			{
				//Wait remaining time
				SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
			}
		}
	}
}

int stringToInt(string atributo) {
	istringstream atributoStream(atributo);
	int atributoInt;
	atributoStream >> atributoInt;

	return atributoInt;
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
		handshakeDeserializado = cliente->getHandshake();
		pthread_create(&threadEnviarEventos, NULL, &enviarEventos, cliente);
		pthread_detach(threadEnviarEventos);
		pthread_create(&threadRecibirPosicionJugadores, NULL, &recibirPosicionJugadores,cliente);
		pthread_detach(threadRecibirPosicionJugadores);
		vector<ImagenDto*> imagenes = handshakeDeserializado->getImagenes();

		vista->cargarEscenario(stringToInt(imagenes.at(0)->getAncho()), stringToInt(imagenes.at(0)->getAlto()), stringToInt(handshakeDeserializado->getAncho()), stringToInt(handshakeDeserializado->getAlto()));

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
