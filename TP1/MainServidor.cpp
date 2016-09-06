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


using namespace std;
pthread_mutex_t mutex;
struct parametrosThreadCliente{
	//estructura que sirve para guardar los parametros que se le pasan a la funcion del thread.
	Servidor* serv;
	int socketCli;
};

void* cicloEscuchaCliente(void* arg)
{
	//esta funcion es la que se va a encargar de hacer send y recv de los enviar/recibir/desconectar
	//es decir, esta funcion es la que va a estar constantemente haciendo send y recv del socket del cliente y detectando lo que quiere hacer.
	struct parametrosThreadCliente* parametros = (parametrosThreadCliente*)arg;
	Servidor* servidor = &parametros->serv;
	int socketCliente = &parametros->socketCli;
	while (1)
	{
		//en este loop se van a gestionar los send y receive del cliente. aca se va a distinguir que es lo que quiere hacer y actuar segun lo que quiera el cliente.

	}
}

void* cicloEscucharConexionesNuevasThreadProceso(void* arg)
{
	int puerto;
	string archivoUsers;
	Servidor* servidor;
	cout << "Ingrese el puerto de escucha de la conexion: " << endl;
	cin >> puerto;
	cout << "Ingrese el nombre del archivo de usuarios: " << endl;
	cin >> archivoUsers;
	char* archivo = strdup(archivoUsers.c_str());
	do{
		servidor = new Servidor(archivo, puerto);
		servidor->comenzarEscucha();
	} while (!servidor->escuchando);


	pthread_t thread_id[MAX_CANT_CLIENTES]; //la cantidad maxima de clientes es 6, voy a crear, como mucho 6 threads para manejar dichas conexiones.
	for(int i=0 ; i<MAX_CANT_CLIENTES ; i++)
	{
	    thread_id[i] = NULL; //inicializo todos en null
	}
	while(1)
	{
		//se va a generar un thread para que cada cliente tenga su comunicacion con el servidor.
		int socketCliente;
		socketCliente = servidor->aceptarConexion();
		//genero un nuevo thread dinamicamente para este cliente
		if (servidor->getCantConexiones() <= MAX_CANT_CLIENTES)
		{
			cout << "te conecte" << endl;
			//si todavia hay lugar en el servidor, creo el thread que va a escuchar los pedidos de este cliente
			parametrosThreadCliente parametrosCliente;
			parametrosCliente.socketCli = socketCliente;
			parametrosCliente.serv = servidor;
			pthread_create(&thread_id[servidor->getCantConexiones()],NULL,&cicloEscuchaCliente,&parametrosThreadCliente); //optimizar ya que si un cliente se desconecta podria causar un problema
		}
	}
}
int inicializarServidor() {
	pthread_t thrProceso;
	int threadOk = pthread_create(&thrProceso, NULL, &cicloEscucharConexionesNuevasThreadProceso, NULL);
	return threadOk;
}

void* escucharEnvio(void* arg){
	Servidor* servidor = (Servidor*) arg;
	pthread_mutex_lock(&mutex);
	cout << "probandoEnvio" << endl;
	//send(servidor->getSocket(), "Bienvenidos", 10, 0);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* escucharRecibir(void* arg){
	Servidor* servidor = (Servidor*) arg;
	pthread_mutex_lock(&mutex);
	cout << "probandoRecibir" << endl;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main() {
	Servidor* servidor;
	pthread_t threadEnvio;
	pthread_t threadRecibir;
	int variable = 1;
/*
	do { //MIENTRAS NO ESTA ESCUCHANDO, TRATA DE INICIALIZAR EL SERVIDOR Y COMENZAR LA ESCUCHA.
		servidor = inicializarServidor();


	} while (!servidor->escuchando);
*/
	int ok =inicializarServidor();
	if (ok != 0){
		cout << "Error inicializando el thread de proceso" << endl;
	}
	else{
		cout << "thread ok" << endl;
	}
	while(1)
	{
		sleep(5); //esto esta a modo de prueba para que no corte los otros threads.
	}




		/*---- Listen on the socket, with 6 max connection requests queued ----*/
		//servidor->comenzarEscucha();
		//servidor->aceptarConexiones();
/*
	while (servidor->escuchando)
	{
		switch(variable){
				case 1:{	//Enviar.
					//servidor->crearMensaje();
					break;
				}
				case 2:{	//Recibir. Crea un thread que se auto destruye una vez finalizado.
					pthread_create(&threadRecibir,NULL,&escucharRecibir,NULL);
					pthread_detach(threadRecibir);
					break;
				}
				case 3:{	//Autenticar.
					servidor->aceptarConexiones();
					break;
				}
				case 4:{	//Desconectar
					servidor->finalizarEscucha();
					break;
				}
				default:
					break;
				}
	}
*/
	//servidor->recibirMensaje();
	/*do{
	switch(variable){
		case 1:{	//Enviar
			pthread_create(&threadEnvio,NULL,&escucharEnvio,&servidor);
			pthread_join(threadEnvio,NULL);
			break;
		}
		case 2:{	//Recibir
			pthread_create(&threadRecibir,NULL,&escucharRecibir,NULL);
			pthread_join(threadRecibir,NULL);
			break;
		}
		case 3:{	//Autenticar
			servidor->autenticar("","");
			break;
		}
		case 4:{	//Desconectar
			servidor->finalizarEscucha();
			break;
		}
		default:
			break;
		}
	} while(servidor->escuchando);*/
	return 0;
}
