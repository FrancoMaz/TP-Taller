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

#define PI 3.14159265

datosConexion datosCliente;
Vista * vista = new Vista();
Handshake* handshakeDeserializado;
queue<string> colaMensajes;

struct ComunicacionCliente{
			Cliente* cliente;
			bool termino;
		};

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	//splitea un string segun un delim y lo guarda en elems
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> splitToVec(const std::string &s, char delim) {
	//devuelve un vector de elementos que representan al string spliteado segun delim
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


int stringToInt(string atributo) {
	istringstream atributoStream(atributo);
	int atributoInt;
	atributoStream >> atributoInt;

	return atributoInt;
}

bool chequearSocket(string ip, int puerto) {
	//string ipServer = "192.168.1.11";

	string ipServer = "127.0.0.1";
	//string ipServer = "192.168.1.12";
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


double calcularAngulo(int numero, string sentido)
{
	double angulo;
	switch (numero)
	{
		case 0:
		{
			angulo = 0;
			break;
		}
		case 1:
		{
			if (sentido == "Normal"){
				angulo = -90;
			} else {
				angulo = 90;
			}
			break;
		}
		case 2:
		{
			if (sentido == "Normal"){
				angulo = -30;
			} else {
				angulo = 30;
			}
			break;
		}
		case 3:
		{
			if (sentido == "Normal"){
				angulo = 45;
			} else {
				angulo = -45;
			}
			break;
		}
		case 4:
		{
			if (sentido == "Normal"){
				angulo = 90;
			} else {
				angulo = -90;
			}
			break;
		}
	}
	return angulo;
}

void procesarUltimosMensajes(string mensajes, Cliente* cliente, UpdateJugador* update, bool primeraVez) {
	string mensajeVacio = "#noHayMensajes@";
	vector<SetDeSpritesDto*> setsSprites = handshakeDeserializado->getSprites();
	if(mensajes != mensajeVacio && mensajes != "") {
		primeraVez = false;
		mensajes[mensajes.length() - 1] = '#';
		string s = mensajes;
		char delimitador = '|';
		char delimitadorFinal = '#';
		char delimitadorCapas = ',';
		string texto;
		string capas;
		size_t posCapas;

		vector<string> msjVec = splitToVec(s,delimitadorFinal);
		vector<string> msjContenido;
		for (int i = 0; i < msjVec.size(); i++){
			string msjActual = msjVec.at(i);
			msjContenido = splitToVec(msjActual,delimitador);
			update->setRemitente(msjContenido.at(0));
			int caso = stringToInt(msjContenido.at(1));
			switch (caso){
				case 0:{
					update->setDestinatario(msjContenido.at(2));
					update->setX(msjContenido.at(3));
					update->setY(msjContenido.at(4));
					update->setCondicion(msjContenido.at(6));
					string spriteAEjecutar = msjContenido.at(5);
					for (int i = 0; i < setsSprites.size(); i++){
						vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
						for (int i = 0; i < listaSprites.size(); i++) {
							if (spriteAEjecutar == listaSprites.at(i)->getID()) {
								update->setSpriteActual(listaSprites.at(i));
							}
						}
					}
					vista->actualizarPosJugador(update,stringToInt(handshakeDeserializado->getAncho()),stringToInt(handshakeDeserializado->getImagenes().at(0)->getAncho()));
					break;
				}
				case 1:{
					int x = stringToInt(msjContenido.at(2));
					int y = stringToInt(msjContenido.at(3));
					vector<pair<int,int>> abscisasCapas;
					for (int i = 4; i < msjContenido.size(); i++){
						pair<int,int> abscisas;
						vector<string> capa = splitToVec(msjContenido.at(i), delimitadorCapas);
						abscisas.first = stringToInt(capa.at(0));
						abscisas.second = stringToInt(capa.at(1));
						abscisasCapas.push_back(abscisas);
					}
					vista->actualizarCamara(x,y,abscisasCapas,stringToInt(handshakeDeserializado->getAncho()));
					break;
				}
				case 2:{
					string nuevaBala = msjContenido.at(2);
					int xBala = stringToInt(msjContenido.at(3));
					int yBala = stringToInt(msjContenido.at(4));
					string spriteBala = msjContenido.at(5);
					int idBala = stringToInt(msjContenido.at(6));
					string sentido = msjContenido.at(7);
					double angulo = calcularAngulo(stringToInt(msjContenido.at(8)), sentido);

					int cantFotogramas;
					for (int i = 0; i < setsSprites.size(); i++) {
						vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
						for (int i = 0; i < listaSprites.size(); i++) {
							if (spriteBala == listaSprites.at(i)->getID()) {
								cantFotogramas = stringToInt(listaSprites.at(i)->getCantidadDeFotogramas());
							}
						}
					}
					vista->actualizarProyectil(nuevaBala,xBala,yBala,spriteBala,idBala,cantFotogramas,sentido,angulo);
					break;
				}
				case 3:{
					string borrarItem = msjContenido.at(2);
					string spriteItem = msjContenido.at(3);
					int xItem = stringToInt(msjContenido.at(4));
					int yItem = stringToInt(msjContenido.at(5));
					vista->agregarVistaItem(borrarItem,spriteItem,xItem,yItem);
					break;
				}
				case 4:{
					string nuevoEnemigo = msjContenido.at(2);
					int xEnemigo = stringToInt(msjContenido.at(3));
					int yEnemigo = stringToInt(msjContenido.at(4));
					string spriteEnemigo = msjContenido.at(5);
					int idEnemigo = stringToInt(msjContenido.at(6));
					int cantFotogramas;
					for (int i = 0; i < setsSprites.size(); i++) {
						vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
						for (int i = 0; i < listaSprites.size(); i++) {
							if (spriteEnemigo == listaSprites.at(i)->getID()) {
								cantFotogramas = stringToInt(listaSprites.at(i)->getCantidadDeFotogramas());
							}
						}
					}
					vista->actualizarEnemigo(nuevoEnemigo, xEnemigo, yEnemigo, spriteEnemigo, idEnemigo, cantFotogramas);
					break;
				}
				case 5:{
					string nuevoBoss = msjContenido.at(2);
					int xBoss = stringToInt(msjContenido.at(3));
					int yBoss = stringToInt(msjContenido.at(4));
					string spriteBoss = msjContenido.at(5);
					string sentido = msjContenido.at(6);
					int cantFotogramas;
					for (int i = 0; i < setsSprites.size(); i++) {
						vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
						for (int i = 0; i < listaSprites.size(); i++) {
							if (spriteBoss == listaSprites.at(i)->getID()) {
								cantFotogramas = stringToInt(listaSprites.at(i)->getCantidadDeFotogramas());
							}
						}
					}
					vista->actualizarBoss(nuevoBoss, xBoss, yBoss, spriteBoss, sentido, cantFotogramas);
					break;
				}
			}
		}
		vista->actualizarPantalla(stringToInt(handshakeDeserializado->getAncho()), stringToInt(handshakeDeserializado->getImagenes().at(0)->getAncho()));
	}
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
		usleep(1000);
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

void* enviarEventos(void* arg) {
	Cliente* cliente = (Cliente*) arg;
	bool controlArriba = false;
	bool presionadaArriba = false;
	bool presionadaDerecha = false;
	bool presionadaIzquierda = false;
	bool presionadaAbajo = false;
	bool presionadaX = false;
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
			else if (vista->controlador->presionarBoton(SDLK_z) && !vista->salto){
				vista->salto = true;
				cliente->enviar("Z", "Todos");
			}
			else if (vista->controlador->presionarBoton(SDLK_UP) && !presionadaArriba)
			{
				presionadaArriba = true;
				cliente->enviar("Tecla Arriba","Todos");
			}
			else if (vista->controlador->presionarBoton(SDLK_DOWN) && !presionadaAbajo)
			{
				presionadaAbajo = true;
				cliente->enviar("Tecla Abajo","Todos");
			}
			else if(vista->controlador->presionarBoton(SDLK_x) && !presionadaX){
				presionadaX = true;
				cliente->enviar("X","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_RIGHT)){
				presionadaDerecha = false;
				cliente->enviar("Soltar Tecla Derecha","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_LEFT)){
				presionadaIzquierda = false;
				cliente->enviar("Soltar Tecla Izquierda","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_DOWN)){
				presionadaAbajo = false;
				cliente->enviar("Soltar Tecla Abajo","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_UP)){
				presionadaArriba = false;
				cliente->enviar("Soltar Tecla Arriba","Todos");
			}
			else if(vista->controlador->soltarBoton(SDLK_x)){
				presionadaX = false;
				cliente->enviar("Soltar X","Todos");
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
			vista->cargarPantallaEsperandoJugadores();
		}while (!inicio && !vista->ventanaCerrada());
		if (!vista->ventanaCerrada()){
			handshakeDeserializado = cliente->getHandshake();
			pthread_create(&threadEnviarEventos, NULL, &enviarEventos, cliente);
			pthread_detach(threadEnviarEventos);
			pthread_create(&threadRecibirPosicionJugadores, NULL, &recibirPosicionJugadores,cliente);
			pthread_detach(threadRecibirPosicionJugadores);
			vector<ImagenDto*> imagenes = handshakeDeserializado->getImagenes();
			vista->cargarEscenario(imagenes, stringToInt(handshakeDeserializado->getAncho()), stringToInt(handshakeDeserializado->getAlto()));
			while(!vista->controlador->comprobarCierreVentana()){
				//recibirPosicionJugadores((void*)&cliente);
				usleep(1000000);
			}
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
