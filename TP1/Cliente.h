/*
 * Cliente.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_CLIENTE_H_
#define TP1_CLIENTE_H_

#include <string>
#include <string.h>
#include <pthread.h>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iterator>
#include "Mensaje.h"
#include <queue>
#include "Vista.h"
#include "ImagenDto.h"
#include "SpriteDto.h"
#include "SetDeSpritesDto.h"
#include "Handshake.h"
#define BUFFER_MAX_SIZE 200
 const int SCREEN_FPS = 20;
 const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
using namespace std;

class Cliente {
private:
	int opcionMenu;
	char* direccionIP;
	int puertoServidor;
	string nombre;
	int socketCliente;
	struct sockaddr_in direccionServidor;
	socklen_t addr_size;
	pthread_t threadComunicacion;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	list<string> clientesDisponibles;
	pthread_mutex_t mutexSocket;
	bool terminoComunicacion;
	struct structHandshake{
			ImagenDto* imagen1;
			ImagenDto* imagen2;
			SetDeSpritesDto* setSprite1;
			SetDeSpritesDto* setSprite2;
			SetDeSpritesDto* setSprite3;
			char* alto;
			char* ancho;
	};
	Handshake* handshake;
	string handshakeRecibido;

public:
	Cliente();
	Cliente(string ip, int puerto);
	virtual ~Cliente();
	ImagenDto* deserializarImagen(char* campo);
	SetDeSpritesDto* deserializarSprite(char* campo);
	Handshake* deserializarHandshake(string handshake, bool primeraVez);
	void recorrerSprites(vector<SpriteDto*> sprites, list<string> *archivos);
	void recibirHandshake();
	void mostrarMenuYProcesarOpcion();
	void elegirOpcionDelMenu(int opcion);
	bool conectar(string nombre, string contrasenia);
	void desconectar();
	void salir();
	void enviar(string mensaje, string destinatario);
	string recibir();
	void loremIpsum(double frecuenciaDeEnvios,double cantidadMaximaDeEnvios);
	string getNombre();
	int getOpcionMenu();
	void setOpcionMenu(int opcion);
	list<string> getClientesDisponibles();
	void vaciarClientesDisponibles();
	pthread_t getThreadComunicacion();
	void setThreadComunicacion(pthread_t thrComu);
	void splitUsuarios(string datosRecibidos);
	void mostrarClientesDisponibles();
	void mostrarUltimosMensajes(string colaMensajes);
	string devolverNombre(int numeroDestinatario);
	void enviarMensajeATodos(string mensaje);
	void inicializarSocket();
	void corroborarConexion();
	bool getTermino();
	bool stringTerminaCon(std::string const &fullString, std::string const &ending);

	//Metodo que envia el request al servidor.
	void enviarRequest(string request);

	//Metodo que envia el request al servidor.
	string recibirResponse();

	//Comprueba si se inicio el juego
	bool checkearInicioJuego(Vista* vista);
	bool verificarBiblioteca(Handshake* handshake);
	bool verificarExistencia(string archivo);
	int getCantidadDeFotogramas(string sprite);
	string getHandshakeRecibido();
	Handshake* getHandshake();
	SpriteDto* buscarSprite(string ID);
};

#endif /* TP1_CLIENTE_H_ */

