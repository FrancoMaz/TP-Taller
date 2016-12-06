/*
 * Servidor.h
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#ifndef TP1_SERVIDOR_H_
#define TP1_SERVIDOR_H_

#include <string>
#include <iostream>
#include <list>
#include <queue>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include "Cliente.h"
#include "Logger.h"
#include "Mensaje.h"
#include "Jugador.h"
#include "XmlParser.h"
#include "Handshake.h"
#include <vector>
#include "Capa.h"
#include "Escenario.h"
#define MAX_CANT_CLIENTES 6
#define BUFFER_MAX_SIZE 1000
#define CANTIDADNIVELES 3
const int INFO = 1;
const int DEBUG = 2;

using namespace std;
class Servidor {

private:
	int puerto;
	int welcomeSocket;
	int socketServer;
	int cantClientesConectados = 0;
	char* nombreArchivo;
	queue<Mensaje> colaMensajesNoProcesados;
	Logger* logger;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	pthread_t threadProcesador;
	pthread_t threadChecker;
	list<string> usuarios;
	struct Datos {
		string nombre;
		string contrasenia;
	};
	struct MensajesProcesados {
		string destinatario;
		Jugador* jugador;
		queue<Mensaje>* mensajes;
	};
	list<Datos>* datosUsuarios;
	list<MensajesProcesados>* listaMensajesProcesados;
	struct ParametrosServidor {
		Mensaje mensajeAProcesar;
		Servidor* servidor;
	};
	struct ParametrosActPosicion{
		Servidor* servidor;
		Jugador* jugador;
	};
	XmlParser* parser;
	vector<string> vectorEquipos;
	int posicionVector;
	int posicionXInicial;
	int nivelActual;
	bool gameComplete;
	int contadorJugadores;

public:
	Servidor();
	Servidor(char* nombreArchivoDeUsuarios, int puerto, Logger* logger);
	virtual ~Servidor();
	void guardarModoDeJuego();
	void guardarDatosDeUsuarios();
	bool existeArchivo(string fileName);
	bool escuchando = false; //representa si el servidor esta disponible para escuchar pedidos
	void autenticar(string nombre, string contrasenia, list<string>& usuarios);
	list<Cliente> obtenerClientes();
	void guardarLog(string mensaje, const int nivelDeLog);
	list<Mensaje> obtenerMensajes(Cliente cliente);
	void crearMensaje(Mensaje mensaje);
	void comenzarEscucha();
	void finalizarEscucha();
	queue<Mensaje> getColaMensajesNoProcesados();
	void splitDatos(char* datos, string* nombre, string* pass);
	void recibirMensaje();
	void setThreadProceso(pthread_t thrProceso);
	pair<int,string> aceptarConexion();
	int getCantConexiones();
	string serializarLista(list<string> datos);
	pthread_mutex_t mutexColaNoProcesados = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexListaProcesados = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexColasProcesadas = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexEnviarMensajes = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexVectorJugadores = PTHREAD_MUTEX_INITIALIZER;;
	pthread_mutex_t mutexLog = PTHREAD_MUTEX_INITIALIZER;
	string mensaje;
	list<string> agregarDestinatarios(string remitente);
	void procesarMensaje(Mensaje mensaje);
	string traerMensajesProcesados(string nombreCliente);
	string concatenarMensajes(queue<Mensaje>* colaDeMensajes);
	void procesarMensajes();
	void restarCantidadClientesConectados();
	static void* actualizarPosiciones(void *arg);
	static void* actualizarPosicionesJugador(void* arg);
	list<MensajesProcesados>* getListaMensajesProcesados();
	void actualizarPosicionesSalto(Mensaje mensajeAProcesar);
	vector<Jugador*>* getJugadores();
	void setJugadorConectado(string nombre);
	void setJugadorDesconectado(string nombre);
	Jugador* obtenerJugador(string nombre);
	vector<Jugador*>* getJugadoresConectados();
	void encolarMensajeProcesadoParaCadaCliente(Mensaje mensajeAProcesar, string mensajeJugadorPosActualizada);
	void iniciarCamara();
	//obtiene el estado inicial de los jugadores serializado para enviarlo via socket
	string getEstadoInicialSerializado();
	void guardarDatosDeConfiguracion();
	void enviarHandshake(int socket,char* cliente);
	pthread_mutex_t mutexSocket = PTHREAD_MUTEX_INITIALIZER;;
	int cantJugadoresConectadosMax = 2;
	SDL_Rect camara;
	Handshake* handshake;
	pair<int,int> obtenerPosicionesExtremos();
	int getCantJugadoresConectadosMax();
	void verificarDesconexion(string nombre);
	bool contieneJugador(string nombre);
	int getAnchoSprite(string sprite);
	vector<Jugador*>* jugadores;
	vector<pair<int,int>> abscisasCapas;
	string serializarCapas();
	void iniciarThreadMovimientoJugador(string nombre);
	void inicializarDatosNiveles();
	vector<Escenario*> vectorNiveles;
	Escenario* getNivelActual();
	void avanzarDeNivel();
	bool verificarColision(SDL_Rect camara, Proyectil* proyectil, bool estaDisparando);
	bool verificarColisionConJugadores(Proyectil* proyectil);
	pair<int,bool> modoJuegoElegido; //el primero es el modo de juego elegido, el segundo es si es modo prueba o no
	vector<string> equipoAlfa;
	vector<string> equipoBeta;
	void resetearPosiciones();
	void calcularPuntajes();
};

#endif /* TP1_SERVIDOR_H_ */
