/*
 * mainServidor.cpp
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
#include "Servidor.h"
#include <pthread.h>
#include <SDL2/SDL.h>

using namespace std;

//Dimensiones de la pantalla
const int ANCHO_VENTANA = 800;
const int ALTO_VENTANA = 600;
SDL_Window* ventana = NULL;
SDL_Surface* capaPrincipal = NULL;
SDL_Surface* imagen = NULL;

struct parametrosThreadCliente {
	//estructura que sirve para guardar los parametros que se le pasan a la funcion del thread.
	Servidor* serv;
	int socketCli;
	string nombre;
};

struct parametrosThreadEncolarMensaje {
	Mensaje* mensajeNoProcesado;
	Servidor* servidor;
};


struct parametrosThreadEnviarMensajeProcesado {
	Servidor* servidor;
	char* usuario;
	int socketCliente;
};

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

bool stringTerminaCon(std::string const &fullString,
		std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0
				== fullString.compare(fullString.length() - ending.length(),
						ending.length(), ending));
	} else {
		return false;
	}
}


bool fileExists(string fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}


void* encolar(void* arg) {
	parametrosThreadEncolarMensaje parametrosEncolarMensaje = *(parametrosThreadEncolarMensaje*) arg;
	Servidor* servidor = parametrosEncolarMensaje.servidor;
	cout<<"servidor en encolar: "<<parametrosEncolarMensaje.servidor<<endl;
	Mensaje* mensaje = parametrosEncolarMensaje.mensajeNoProcesado;

	servidor->mensaje = "Encolando mensaje: " + mensaje->getTexto() + ". De: "
			+ mensaje->getRemitente() + ". Para: " + mensaje->getDestinatario()
			+ ". \n";
	servidor->guardarLog(servidor->mensaje, DEBUG);

	cout << servidor->mensaje << endl;
	if (mensaje->getDestinatario().compare("Todos") != 0) {
		cout<<"entra aca porque el mensaje no es para todos"<<endl;
		pthread_mutex_lock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		servidor->crearMensaje(*mensaje);
		servidor->mensaje = "Encolando mensaje: " + mensaje->getTexto()
				+ ". De: " + mensaje->getRemitente() + ". Para: "
				+ mensaje->getDestinatario() + ". \n";
		servidor->guardarLog(servidor->mensaje, DEBUG);
		pthread_mutex_unlock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
	} else {
		cout<<"como el mensaje es para todos entra aca"<<endl;
		//genera la lista de destinatarios del remitente en cuestion
		list<string> destinatarios = servidor->agregarDestinatarios(mensaje->getRemitente());
		pthread_mutex_lock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		for (list<string>::iterator datoActual = destinatarios.begin();datoActual != destinatarios.end(); datoActual++) {
			 string usuario;
			 usuario = *datoActual;
			 cout<<"procesa el remitente: "<<usuario<<endl;

			 if (usuario != mensaje->getRemitente()){

				Mensaje* msj = new Mensaje(mensaje->getRemitente(),usuario,mensaje->getTexto());
				servidor->mensaje = "Encolando mensaje: " + msj->getTexto() + ". De: " + msj->getRemitente() + ". Para: " + msj->getDestinatario() + ". \n";
				parametrosEncolarMensaje.servidor->crearMensaje(*msj);
				servidor->guardarLog(servidor->mensaje, DEBUG);
				msj->~Mensaje();

			}
		}
		pthread_mutex_unlock(&parametrosEncolarMensaje.servidor->mutexColaNoProcesados);
		mensaje->~Mensaje();
	}
	return NULL;
}


void encolarMensaje(char* remitente, char* destinatario, char* mensaje, Servidor* servidor) {
	pthread_t threadEncolarMensaje;
	parametrosThreadEncolarMensaje parametrosEncolarMensaje;
	parametrosEncolarMensaje.mensajeNoProcesado = new Mensaje(remitente, destinatario, mensaje);
	parametrosEncolarMensaje.servidor = servidor;
	cout<<"servidor en encolarMensaje: "<<servidor<<endl;
	pthread_create(&threadEncolarMensaje, NULL, &encolar,&parametrosEncolarMensaje);
	pthread_detach(threadEncolarMensaje); //lo marco
}

void* procesar(void* arg) {
    cout<<"llega al procesar"<<endl;
	parametrosThreadEnviarMensajeProcesado* parametros = (parametrosThreadEnviarMensajeProcesado*) arg;
	cout<< "servidor :"<< parametros->servidor<<endl;
	Servidor* servidor = parametros->servidor;
	cout<<"hace bien el servidor"<<endl;
	char* usuario = parametros->usuario;
	cout<<"hace bien el usuario"<<endl;
	int socket = parametros->socketCliente;
	cout<<"hace bien todos los parametros"<<endl;
	cout<<"socket Cliente: "<<parametros->socketCliente<<endl;

	pthread_mutex_lock(&servidor->mutexListaProcesados);
	cout << "le llega el mensaje de recibir al servidor, del cliente: " << usuario << endl;
	string mensajesProcesados = servidor->traerMensajesProcesados(usuario);
	pthread_mutex_unlock(&servidor->mutexListaProcesados);

	cout << "trae bien los mensajes procesados" << endl;
	char buffer[1024];
	strcpy(buffer, mensajesProcesados.c_str()); //aca muere, el problema es este strcpy y el string y char*

	int largo = strlen(mensajesProcesados.c_str());
	cout << "Cliente que solicita sus mensajes: " << usuario << "  "<< mensajesProcesados << endl;
	//cout<<"socket Cliente: "<<socket<<endl;
	largo -= send(socket, buffer, largo + 1, 0);
	cout<<"largo del send: "<<largo<<endl;
	while (largo > 0) {
		largo -= send(socket, buffer, largo + 1, 0);
	}
	return NULL;
}
void enviarMensajesProcesadosA(char* usuario, Servidor* servidor, int socket) {

	pthread_t threadEnviarMensajesProcesados;
	parametrosThreadEnviarMensajeProcesado parametrosMensajesProcesados;
	cout<<"entra al enviarMensajesProcesados"<<endl;
	parametrosMensajesProcesados.usuario = usuario;
	parametrosMensajesProcesados.servidor = servidor;
	cout<<"servidor en enviarMensajesProcesadosA: "<<parametrosMensajesProcesados.servidor<<endl;
	parametrosMensajesProcesados.socketCliente = socket;
	cout<<"socket en enviarMensaje: "<<socket<<endl;
	cout<<"parametroMensajesProcesados: "<<parametrosMensajesProcesados.socketCliente<<endl;
	pthread_create(&threadEnviarMensajesProcesados, NULL, &procesar,&parametrosMensajesProcesados);
	cout<<"sale del hilo de mensajes procesados"<<endl;
	pthread_join(threadEnviarMensajesProcesados,NULL);
	//pthread_detach(threadEnviarMensajesProcesados); //lo marco
}


void* cicloProcesarMensajes(void* arg)
{
	Servidor* servidor = (Servidor*)arg;
	while(servidor->escuchando)
	{
		servidor->procesarMensajes();
	}
}

void* cicloEscuchaCliente(void* arg) {
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*) arg;
	Servidor* servidor = parametros->serv;
	string nombre = parametros->nombre;
	int socketCliente = parametros->socketCli;
	servidor->guardarLog("Escuchando al cliente: " + nombre + string(".\n"),INFO);
	char bufferRecibido[BUFFER_MAX_SIZE];
	bool conectado = true;
	while (conectado and servidor->escuchando) {
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.
		string datosRecibidos;

		int largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE,0); //recibo por primera vez
		if (largoRequest > 0) {
			datosRecibidos.append(bufferRecibido, largoRequest);
			cout << largoRequest << endl;
			while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "#")) {
				//mientras haya cosas que leer, sigo recibiendo.
				largoRequest = recv(socketCliente, bufferRecibido, BUFFER_MAX_SIZE, 0);
				cout << largoRequest << endl;
				datosRecibidos.append(bufferRecibido, largoRequest);
			}
			if (largoRequest <= 0)
			{
				string mensaje = "Ocurrió un problema en el socket para el cliente: " + nombre + string(".\n");
				conectado = false;
				servidor->guardarLog(mensaje, DEBUG);
				servidor->restarCantidadClientesConectados();
			}
			else{
				//en el formato siempre recibimos primero el metodo que es un entero.
				//1 representa enviar un mensaje, 2 representa recibir mis mensajes, 3 desconectar.
				char* datos = strdup(datosRecibidos.c_str());
				char* metodo = strtok(datos, "|");
				int accion = atoi(metodo); //convierto a entero el metodo recibido por string
				switch (accion) {
				case 1: { //1 es enviar
					char* remitente = strtok(NULL,"|");
					char* destinatario = strtok(NULL,"|");
					char* mensaje = strtok(NULL,"#");
					encolarMensaje(remitente, destinatario, mensaje,servidor);
					break;
				}
				case 2: { //2 es recibir
					char* usuarioQueSolicita = strtok(NULL, "#");
					cout<<"socket Cliente antes de enviar mensaje:"<<socketCliente<<endl;
					enviarMensajesProcesadosA(usuarioQueSolicita, servidor, socketCliente);
					break;
				}


				}
			}
		}
		else {
			string mensaje = "Ocurrió un problema en el socket para el cliente: " + nombre + string(".\n");
			conectado = false;
			servidor->guardarLog(mensaje, DEBUG);
			servidor->restarCantidadClientesConectados();
		}
	}
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg) {
	Servidor* servidor = (Servidor*)arg;
	pthread_t threadProceso;
	pthread_create(&threadProceso,NULL,&cicloProcesarMensajes,(void*)servidor);
	servidor->setThreadProceso(threadProceso);

	pthread_t thread_id[MAX_CANT_CLIENTES]; //la cantidad maxima de clientes es 6, voy a crear, como mucho 6 threads para manejar dichas conexiones.
	for (int i = 0; i < MAX_CANT_CLIENTES; i++) {
		thread_id[i] = NULL; //inicializo todos en null
	}
	while (servidor->escuchando) {
		//se va a generar un thread para que cada cliente tenga su comunicacion con el servidor.
		pair<int,string> cliente = servidor->aceptarConexion();
		int socketCliente = cliente.first;
		string nombreCliente = cliente.second;
		if (socketCliente < 0)
		{
			cout << "Error al conectar al cliente" << endl;
			servidor->guardarLog("No se pudo conectar al cliente.\n", INFO);
			servidor->guardarLog("ERROR: Problema con la conexion del socket.\n", DEBUG);
		}
		else{
			//genero un nuevo thread dinamicamente para este cliente
			if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES) {
				//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
				parametrosThreadCliente parametrosCliente;
				parametrosCliente.socketCli = socketCliente;
				parametrosCliente.serv = servidor;
				parametrosCliente.nombre = nombreCliente;
				cout<<"llega casi al final de cicloEscucharConexionesNuevasThreadProceso"<<endl;
				pthread_create(&thread_id[servidor->getCantConexiones()], NULL,&cicloEscuchaCliente, &parametrosCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
				pthread_detach(thread_id[servidor->getCantConexiones()] ); //lo marco como detach
			}
		}
	}
}

Servidor* inicializarServidor() {
	pthread_t thrConexiones;
	int puerto;
	string archivoUsers;
	int modoLogger;
	bool puertoValido = false;
	bool modoLoggerValido = false;
	do {
		cout << "Elija el modo en el que quiere loggear los eventos: " << endl;
		cout << "1) INFO" << endl;
		cout << "2) DEBUG" << endl;
		cin >> modoLogger;
		if (cin.good() && (modoLogger == 1 || modoLogger == 2)) {
			modoLoggerValido = true;
		} else {
			cin.clear();
			cin.ignore();
			cout << "Error: la opcion ingresada no es valida" << endl;
		}
	} while (!modoLoggerValido);
	Logger* logger = new Logger(modoLogger);
	stringstream modo;
	modo << modoLogger;
	logger->escribir("El nivel de log seleccionado es: " + modo.str() + string(". \n"),INFO);
	do {
		cout << "Ingrese el puerto de escucha de la conexion: " << endl;
		cin >> puerto;
		if (cin.good())
		{
			puertoValido = true;
			stringstream puertoStr;
			puertoStr << puerto;
			logger->escribir("Puerto ingresado: " + puertoStr.str() + string(". \n"),INFO);
		}
		else
		{
			cin.clear();
			cin.ignore();
			cout << "Error: el dato ingresado debe ser un numero" << endl;
			logger->escribir("ERROR: Puerto inválido. \n",INFO);
		}
	} while (!puertoValido);
	bool existeArchivo;
	do{
		cout << "Ingrese el nombre del archivo de usuarios: " << endl;
		cin >> archivoUsers;
		logger->escribir("Archivo ingresado: " + archivoUsers + string(". \n"),INFO);
		existeArchivo = fileExists(archivoUsers);
		if (!existeArchivo){
			logger->escribir("ERROR: El archivo no existe. \n",INFO);
			cout << "Error: El archivo no existe." << endl;
		}
	} while (!existeArchivo);
	logger->escribir("Archivo encontrado.\n",DEBUG);
	char* archivo = strdup(archivoUsers.c_str());
	Servidor* servidor = new Servidor(archivo, puerto, logger);

	do {
		servidor->comenzarEscucha();
	} while (!servidor->escuchando);

	int threadOk = pthread_create(&thrConexiones, NULL,
		&cicloEscucharConexionesNuevasThreadProceso, servidor);
	if (threadOk != 0){
		cout << "Error inicializando el thread de escucha de conexiones" << endl;
		servidor->guardarLog("ERROR: No se pudo crear el thread de escucha de conexiones.\n", DEBUG);
	}
	else{
		servidor->guardarLog("Creación de thread de escucha de conexiones OK. \n", DEBUG);
	}
	pthread_detach(thrConexiones);
	return servidor;
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

	Servidor* servidor = inicializarServidor();
	cout << "Escriba q y presione enter para salir.. \n";
	string salir = "";
	while (servidor->escuchando) {
		cin >> salir;
		if (salir == "q"){
			string mensaje = "Se ha cerrado el servidor";
			servidor->guardarLog(mensaje,INFO);
			servidor->finalizarEscucha();
		}
	}



	cerrarVentana();
	return 0;
}
