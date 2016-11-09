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
Handshake* handshakeDeserializado;
queue<string> colaMensajes;

struct ComunicacionCliente{
			Cliente* cliente;
			bool termino;
		};


int stringToInt(string atributo) {
	istringstream atributoStream(atributo);
	int atributoInt;
	atributoStream >> atributoInt;

	return atributoInt;
}

bool chequearSocket(string ip, int puerto) {
	//string ipServer = "192.168.1.11";

	//string ipServer = "10.1.77.13";
	string ipServer = "192.168.1.12";
	int puertoDeEscucha = 7891;

	return (ip == ipServer && puerto == puertoDeEscucha);
}
void* verificarConexion(void * arg){
	ComunicacionCliente* comunicacion = (ComunicacionCliente*)arg;
	Cliente* cliente = comunicacion->cliente;
	cliente->corroborarConexion();
	vista->controlador->setCerrarVentana();
    //comunicacion->termino = cliente->corroborarConexion();
}

void procesarUltimosMensajes(string mensajes, Cliente* cliente, UpdateJugador* update, bool primeraVez)
{   string mensajeVacio = "#noHayMensajes@";
	vector<SetDeSpritesDto*> setsSprites = handshakeDeserializado->getSprites();
	if(mensajes != mensajeVacio && mensajes != ""){
		primeraVez = false;
		mensajes[mensajes.length() - 1] = '#';
		string s = mensajes;
		string delimitador = "|";
		string delimitadorFinal = "#";
		string delimitadorCapas = ",";
		string texto;
		string capas;
		size_t posCapas;
		size_t pos = s.find(delimitador);
		texto = s.substr(0, pos);
		while (texto != "") {
			update->setRemitente(texto);
			s.erase(0, pos + delimitador.length());
			pos = s.find(delimitador);
			texto = s.substr(0,pos);
			if (texto == "0"){
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitador);
				texto = s.substr(0,pos);
				update->setDestinatario(texto);
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitador);
				texto = s.substr(0,pos);
				update->setX(texto);
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitador);
				texto = s.substr(0,pos);
				update->setY(texto);
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitador);
				texto = s.substr(0,pos);
				string spriteAEjecutar;
				spriteAEjecutar = texto;
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitadorFinal);
				texto = s.substr(0,pos);
				update->setCondicion(texto);
				for (int i = 0; i < setsSprites.size(); i++){
					vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
					for (int i = 0; i < listaSprites.size(); i++)
					{
						if ((string(spriteAEjecutar)) == listaSprites.at(i)->getID())
						{
							update->setSprite(listaSprites.at(i));
						}
					}
				}
			}
			else{
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitador);
				texto = s.substr(0,pos);
				int x = stringToInt(texto);
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitador);
				texto = s.substr(0,pos);
				int y = stringToInt(texto);
				s.erase(0, pos + delimitador.length());
				pos = s.find(delimitadorFinal);
				texto = s.substr(0,pos);
				vector<pair<int,int>> abscisasCapas;
				while ((posCapas = texto.find(delimitador)) != string::npos)
				{
					pair<int,int> abscisas;
					posCapas = texto.find(delimitadorFinal);
					capas = texto.substr(0,posCapas);
					abscisas.first = stringToInt(capas);
					texto.erase(0,posCapas+delimitadorCapas.length());
					posCapas = texto.find(delimitador);
					capas = texto.substr(0,posCapas);
					abscisas.second = stringToInt(capas);
					abscisasCapas.push_back(abscisas);
					texto.erase(0,posCapas+delimitador.length());
				}
				vista->actualizarCamara(x,y,abscisasCapas,stringToInt(handshakeDeserializado->getAncho()));
			}
			vista->actualizarPosJugador(update,stringToInt(handshakeDeserializado->getAncho()),stringToInt(handshakeDeserializado->getImagenes().at(0)->getAncho()));
			//vista->actualizarJugador(update,stringToInt(handshakeDeserializado->getAncho()),stringToInt(handshakeDeserializado->getImagenes().at(0)->getAncho()));
			s.erase(0, pos + delimitador.length());
			pos = s.find(delimitador);
			texto = s.substr(0,pos);
		}
	}
	//else{
	vista->actualizarPantalla(stringToInt(handshakeDeserializado->getAncho()), stringToInt(handshakeDeserializado->getImagenes().at(0)->getAncho()));
		//vista->actualizarJugador(update,stringToInt(handshakeDeserializado->getAncho()), stringToInt(handshakeDeserializado->getImagenes().at(0)->getAncho()));
	//}
}

void* recibirPosicionJugadores(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	string datosRecibidos = "";
	UpdateJugador* update = new UpdateJugador();
	bool primeraVez = true;
	 //The frames per second timer
	LTimer capTimer;
	usleep(50000);
	while(!vista->controlador->comprobarCierreVentana()){
		//usleep(3000);
		 //Start cap timer
		capTimer.start();
		datosRecibidos = cliente->recibir();
		procesarUltimosMensajes(datosRecibidos, cliente, update, &primeraVez);

		//si se procesa antes, espero lo que tengo que resta.
		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICKS_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}
	}
}
/* METODO QUE CREE PARA PROBAR ALGO (FRANCO)
void* enviarYRecibir(void* arg)
{
	Cliente* cliente = (Cliente*) arg;
	//The frames per second cap timer
	LTimer capTimer;
	while(!vista->controlador->comprobarCierreVentana()){
		capTimer.start();
		if (SDL_PollEvent(&evento))
		{
			if (evento.type == SDL_QUIT){
				vista->controlador->setCerrarVentana();
			}
			if(vista->controlador->presionarBoton(SDLK_RIGHT)){
				cliente->enviar("Tecla Derecha","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_LEFT)){
				cliente->enviar("Tecla Izquierda","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_UP)){
				cliente->enviar("Tecla Arriba","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_r)){
				cliente->enviar("R","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_RIGHT)){
				cliente->enviar("Soltar Tecla Derecha","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_LEFT)){
				cliente->enviar("Soltar Tecla Izquierda","Todos");
			}
			SDL_FlushEvent(SDL_MOUSEMOTION);
			SDL_FlushEvent(SDL_KEYDOWN);
		}
		string datosRecibidos = "";
		UpdateJugador* update = new UpdateJugador();
		bool primeraVez = true;
		datosRecibidos = cliente->recibir();
		procesarUltimosMensajes(datosRecibidos, cliente, update, &primeraVez);

		//si se procesa antes, espero lo que tengo que resta.
		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICKS_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}
	}
}
*/
void* enviarEventos(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	bool controlArriba = false;
	bool presionadaArriba = false;
	bool presionadaDerecha = false;
	bool presionadaIzquierda = false;
	 //The frames per second timer
	LTimer fpsTimer;
	//The frames per second cap timer
	LTimer capTimer;
	//Start counting frames per second
	int countedFrames = 0;
	fpsTimer.start();
	while(!vista->controlador->comprobarCierreVentana()){
		while(SDL_PollEvent(&evento)){
			if (evento.type == SDL_QUIT){
				vista->controlador->setCerrarVentana();
			}
			else if(vista->controlador->presionarBoton(SDLK_r)){
				cliente->enviar("R","Todos");
			}
			else if (vista->controlador->presionarBoton(SDLK_RIGHT) && !presionadaDerecha){
				presionadaDerecha = true;
				cliente->enviar("Tecla Derecha","Todos");
			}
			else if (vista->controlador->presionarBoton(SDLK_LEFT) && !presionadaIzquierda)
			{
				presionadaIzquierda = true;
				cliente->enviar("Tecla Izquierda", "Todos");
			}
			else if (vista->controlador->presionarBoton(SDLK_UP) && !presionadaArriba && !vista->salto){
				vista->salto = true;
				presionadaArriba = true;
				cliente->enviar("Tecla Arriba", "Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_RIGHT)){
				presionadaDerecha = false;
				cliente->enviar("Soltar Tecla Derecha","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_LEFT)){
				presionadaIzquierda = false;
				cliente->enviar("Soltar Tecla Izquierda","Todos");
			}
			else if(!vista->salto){
				presionadaArriba = false;
			}
		/*const Uint8 *keys = SDL_GetKeyboardState(NULL);
		usleep(3000);
		while(SDL_PollEvent(&evento)){
			capTimer.start();
			if (evento.type == SDL_QUIT){
				vista->controlador->setCerrarVentana();
			}
			if(vista->controlador->presionarBoton(SDLK_RIGHT)){
				cliente->enviar("Tecla Derecha","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_LEFT)){
				cliente->enviar("Tecla Izquierda","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_UP)){
				cliente->enviar("Tecla Arriba","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_r)){
				cliente->enviar("R","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_RIGHT)){
				cliente->enviar("Soltar Tecla Derecha","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_LEFT)){
				cliente->enviar("Soltar Tecla Izquierda","Todos");
			}
			*/
			SDL_FlushEvent(SDL_MOUSEMOTION);
			SDL_FlushEvent(SDL_KEYDOWN);//si se procesa antes, espero lo que tengo que resta.
			int frameTicks = capTimer.getTicks();
			if( frameTicks < 25 )
			{
				//Wait remaining time
				SDL_Delay( 25 - frameTicks );
			}
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
		pthread_create(&threadVerificarConexion, NULL,&verificarConexion,&comunicacion);
		pthread_detach(threadVerificarConexion);
		bool inicio;
		do
		{
			usleep(500000);
			inicio = cliente->checkearInicioJuego(vista);
		}while (!inicio);
		handshakeDeserializado = cliente->getHandshake();
		pthread_create(&threadEnviarEventos, NULL, &enviarEventos, cliente);
		pthread_detach(threadEnviarEventos);
		pthread_create(&threadRecibirPosicionJugadores, NULL, &recibirPosicionJugadores,cliente);
		pthread_detach(threadRecibirPosicionJugadores);
		vector<ImagenDto*> imagenes = handshakeDeserializado->getImagenes();
		vista->cargarEscenario(imagenes, stringToInt(handshakeDeserializado->getAncho()), stringToInt(handshakeDeserializado->getAlto()));
		while(!vista->controlador->comprobarCierreVentana()){
			//recibirPosicionJugadores((void*)&cliente);
			usleep(100);
		}
		cliente->desconectar();
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
