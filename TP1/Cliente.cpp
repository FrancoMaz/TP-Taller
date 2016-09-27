#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>

using namespace std;

Cliente::Cliente(string ip, int puerto) {
//El cliente se crea con la direccion IP y el puerto en el cual se encuentra disponible el servidor
	this->direccionIP = strdup(ip.c_str());
	this->puertoServidor = puerto;
	this->opcionMenu = 0;
	this->terminoComunicacion = false;
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

void Cliente::inicializarSocket(){
	this->socketCliente = socket(PF_INET, SOCK_STREAM, 0);
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_port = htons(this->puertoServidor);
	direccionServidor.sin_addr.s_addr = inet_addr(this->direccionIP);
	memset(direccionServidor.sin_zero, '\0', sizeof direccionServidor.sin_zero);
}

void Cliente::mostrarMenuYProcesarOpcion() {
	bool esValido = false;
		cout << "1) Enviar Mensaje" << endl;
		cout << "2) Recibir Mensajes" << endl;
		cout << "3) Lorem Ipsum" << endl;
		cout << "4) Desconectar" << endl;
		cout << "5) Salir" << endl;
	do {
		cout << "Elija la accion que desea realizar: " << endl;
		cin >> opcionMenu;
		if (cin.good() && (opcionMenu >= 1 && opcionMenu <= 5)) {esValido = true;}
			else {
				cin.clear();
				cin.ignore();
				cout << "Error: la opcion ingresada no es valida" << endl;
				}
		}
		while (!esValido);
		//this->elegirOpcionDelMenu(opcionMenu);
}

void Cliente::elegirOpcionDelMenu(int opcion) {

		switch (opcion) {
		case 1: {
			bool esValido = false;
			int numeroDestinatario;
			string mensajeAEnviar;
			if(!(this->terminoComunicacion)){//verifica si hay conexion a la hora de mandar un mensaje
				this -> mostrarClientesDisponibles();
				int cantidadClientesDisponibles = this->clientesDisponibles.size();
				do {
					cout << "Escriba el numero asociado al nombre del destinatario del mensaje: ";
					cin >> numeroDestinatario;
					if (cin.good() && (numeroDestinatario >= 1 && numeroDestinatario <= (cantidadClientesDisponibles + 1)))
					{esValido = true;}
					else {
						cin.clear();
						cin.ignore();
						cout << "Error: la opcion ingresada no es valida" << endl;
					}
				} while (!esValido);
				cin.ignore();
				cout << "Escriba su mensaje: " << endl;
				getline(cin,mensajeAEnviar);
				if (numeroDestinatario != (this->clientesDisponibles.size()) + 1){
					string nombreDestinatario = this->devolverNombre(numeroDestinatario - 1);
					this->enviar(mensajeAEnviar, nombreDestinatario);
				} else {this -> enviar(mensajeAEnviar, "Todos");}
			}else{cout<<"Se cerro la conexion con el servidor. Presione 5 para salir"<<endl;}
			break;
		}
		case 2: {
			if(!(this->terminoComunicacion)){this->recibir();}//verifica si hay conexion a la hora de recibir mensajes
			else{cout<<"Se cerro la conexion con el servidor. Presione 5 para salir"<<endl;}
			break;
		}
		case 3: {
			double frecuenciaDeEnvios;
			double cantidadMaximaDeEnvios;
			if(!(this->terminoComunicacion)){//verifica si hay conexion a la hora de mandar un mensaje
				cout << "Escriba la frecuencia de envios: " << endl;
				cin >> frecuenciaDeEnvios;
				cout << "Escriba la cantidad maxima de envios: " << endl;
				cin >> cantidadMaximaDeEnvios;
				this->loremIpsum(frecuenciaDeEnvios, cantidadMaximaDeEnvios);}
			else{cout << "Se cerro la conexion con el servidor. Presione 5 para salir" << endl;}
			break;
		}
		case 4: {
			this->desconectar();
			break;
		}
		case 5: {
			this->salir();
			break;
		}
		default:
			break;
		}
}

void Cliente::corroborarConexion() {
 	int ok = 1;
	while(ok>0){
		//cout<<"el valor del ok: "<<ok<<endl;
		char buffer[BUFFER_MAX_SIZE];
		char* escuchando = "3|";
		strcpy(buffer, escuchando);
		clock_t tiempoInicio = clock();

		do {
			} while ((double)((clock()-tiempoInicio)/CLOCKS_PER_SEC) < 2);
		send(this->socketCliente, escuchando, strlen(escuchando), 0);

		/*if (okSend > 0)
		{*/
			ok = recv(this->socketCliente, buffer, BUFFER_MAX_SIZE, 0);
	/*	}
		*/
 	}
	this->terminoComunicacion = true;
	cout << "Se cerro la conexion con el servidor. Presione 5 para salir" << endl;
	this->salir();
 }

void Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles
	this->inicializarSocket();
	char buffer[BUFFER_MAX_SIZE];
	char datosRecibidos[BUFFER_MAX_SIZE];
	this->addr_size = sizeof direccionServidor;
	char* nombreYPass = strdup((nombre + ',' + contrasenia).c_str()); // convierte el string de const char* a char*
	cout << "Intentando conectarse con el servidor. . ." << endl;
	if (connect(socketCliente, (struct sockaddr *) &direccionServidor,addr_size) == 0) {

		strcpy(buffer, nombreYPass);
		send(socketCliente, buffer, strlen(nombreYPass) + 1, 0);
		this->nombre = nombre;
		//this->clientesDisponibles.push_front("hola"); //pongo cualquier cosa para comprobar el ciclo ok.
		recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
		string datos = datosRecibidos;
		string desconectarse = "Desconectar";
		if (strcmp(datos.c_str(), desconectarse.c_str()) == 0) {
			cout << "Usuario/clave incorrectos, inténtelo de nuevo" << endl;
			this->desconectar();
		} else {
			cout << "Conectandose al puerto: " << this->puertoServidor << endl;
			cout << "Datos recibidos: " << datos << endl; //IMPRIMO LA RESPUESTA DEL SERVER, SI SE PUDO AUTENTICAR O NO. --> HAY QUE CAMBIAR ESTO PORQUE EN REALIDAD SE DEVUELVE LA LISTA DE USUARIOS.
			splitUsuarios(datosRecibidos);
		}
	} else {
		cout << "Error conectandose al puerto" << endl;
	}
	//free(nombreYPass);
}

void Cliente::splitUsuarios(string datos) {
	string datosASplitear = datos.substr(0, datos.length() - 1);
	char str[BUFFER_MAX_SIZE];
	strcpy(str, datosASplitear.c_str());
	char* pch = strtok(str, ",");
	cout << "Los usuarios disponibles son: " << endl;
	while (pch != NULL) {
		this->clientesDisponibles.push_back(pch);
		cout << pch << endl;
		pch = strtok(NULL, ",");
	}
}

void Cliente::desconectar() {
	//Se cierra la conexion con el servidor
	close(socketCliente);
}

void Cliente::salir() {
	//Se termina la ejecucion del programa
	this->opcionMenu = 5;
	//cout << "Desconectando al cliente" << endl;
	close(socketCliente);
}

void Cliente::enviar(string mensaje, string destinatario) {
	//Se envia un mensaje a un usuario o a todos (este ultimo caso sucede cuando el destinatario es el string "Todos").

	if(!(this->terminoComunicacion)){
		char* mensajeCadena = strdup(mensaje.c_str());
		Mensaje *mensajeAEnviar = new Mensaje(this->nombre, destinatario, mensaje);
		char* stringDatosMensaje = strdup(("1|" + mensajeAEnviar->getStringDatos()).c_str()); //1 significa enviar.
		int largo = strlen(stringDatosMensaje);
		int largoRequest;
		while (largo > 0)
		{
			largoRequest = send(this->socketCliente, stringDatosMensaje, largo + 1, 0);
			largo -= largoRequest;
		}
		free(mensajeCadena); }
	//free(stringDatosMensaje);
}

void Cliente::enviarMensajeATodos(string mensaje) {
	for (list<string>::iterator i = this->clientesDisponibles.begin(); i != this->clientesDisponibles.end(); i++) {
			this -> enviar(mensaje,*i);
	}
}

void Cliente::recibir() {
	//Se reciben todos los mensajes en la secuencia en la que fueron enviados
	char colaMensajes[BUFFER_MAX_SIZE];
	string metodo = "2|" + this->nombre + "#";
	char* recibir = strdup(metodo.c_str()); //2 es recibir
	send(this->socketCliente, recibir, strlen(recibir), 0);
	string datosRecibidos = "";
	int largoRequest;
	do {
		largoRequest = recv(this->socketCliente, colaMensajes, BUFFER_MAX_SIZE, 0);
	} while (largoRequest == 0);

	cout << "Cola mensajes: " << colaMensajes << endl;
	datosRecibidos += string(colaMensajes);
	memset(colaMensajes, '\0', strlen(colaMensajes));
	cout << "Largo request: " << largoRequest << endl;
	while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")) //mientras el largoRequest sea del tamaño del max size, sigo pidiendo
	{
		int largo;
		//mientras haya cosas que leer, sigo recibiendo.
		do {
			//sigue aca mientras no recibe nada, cuando recibe algo sale de este do while
			largo = recv(socketCliente, colaMensajes, BUFFER_MAX_SIZE, 0);
			cout << "Largo: " << largo << endl;
			largoRequest += largo;
		}
		while (largoRequest < BUFFER_MAX_SIZE);
		cout << "Cola mensajes: " << colaMensajes << endl;
		cout << "Largo request: " << largoRequest << endl;
		if (largoRequest > 0){
			datosRecibidos += string(colaMensajes);
		}
		memset(colaMensajes, '\0', strlen(colaMensajes));
	}
	cout<<"mensaje recibido: "<<datosRecibidos<<endl;
	cout<<"largo mensaje: "<<datosRecibidos.length()<<endl;
	this -> mostrarUltimosMensajes(datosRecibidos);
}

bool Cliente::stringTerminaCon(std::string const &fullString,
		std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0
				== fullString.compare(fullString.length() - ending.length(),
						ending.length(), ending));
	} else {
		return false;
	}
}

void Cliente::mostrarUltimosMensajes(string colaMensajes)
{   string mensajeVacio = "#noHayMensajes#";
	cout << "Ultimos mensajes recibidos: " << endl;
	if(strcmp(colaMensajes.c_str(), mensajeVacio.c_str()) == 0){
		cout<<"No hay mensajes nuevos"<<endl;}
	else{
		colaMensajes[colaMensajes.length() - 1] = '#';
		char str[colaMensajes.length()];
		strcpy(str, colaMensajes.c_str());
		char* texto = strtok(str, "|");
		// hacer que no imprima arroba
		while (texto != NULL) {
			cout<<"Mensaje de "<<texto<<":"<<endl;
			texto = strtok(NULL,"#");
			cout << texto << endl;
			cout<<endl;
			texto = strtok(NULL, "|");
		}
	}
}

void Cliente::loremIpsum(double frecuenciaDeEnvios, double cantidadMaximaDeEnvios) {
	//Toma el texto de un archivo y se envian mensajes en forma ciclica. El tamanio de mensajes y el destinatario son aleatorios
	//Cuando todo el texto fue transmitido, se empieza otra vez desde el inicio
	FILE* archivo;
	int tamanioMensaje;
	int numeroDeClienteAEnviar;
	string clienteAleatorioAEnviar;
	archivo = fopen("LoremIpsum.txt","r");
	srand(time(NULL));
	//El tamanio de los mensajes este entre 0 y 200
	tamanioMensaje = (int) (rand() % 200);
	//Se elige aleatoriamente el destinatario de la secuencia de mensajes
	numeroDeClienteAEnviar = (int) (rand()% this->clientesDisponibles.size());
	list<string>::iterator iterador = this->clientesDisponibles.begin();
	advance(iterador, numeroDeClienteAEnviar);
	clienteAleatorioAEnviar = *iterador;
	//tiempoPorMensaje indica cada cuanto tiempo se manda un mensaje (en segundos)
	//Se mandan frecuenciaDeEnvios mensajes en 1 segundo, entonces un mensaje se manda en 1/frecuenciaDeEnvios segundos
	double tiempoPorMensaje = (1/frecuenciaDeEnvios);
	//Indico el tiempo de inicio
	clock_t tiempoInicio = clock();
	for (int i = 0; i < cantidadMaximaDeEnvios; i++) {
		if (!this->terminoComunicacion) {
			string cadena;
			//No tiene que ocurrir nada hasta que el tiempo transcurrido sea igual al tiempo estipulado de envio para cada mensaje
			do {
			} while (((double)(clock()-tiempoInicio)/CLOCKS_PER_SEC) < tiempoPorMensaje);
			//Una vez que se transcurren tiempoPorMensaje segundos se empieza a leer caracteres del archivo
				for (int j=0; j < tamanioMensaje; j++) {
					char c = fgetc(archivo);
					//Si no hay salto de linea o fin de archivo se guarda el caracter leido
					if (c != '\n' && c != EOF) {
						cadena = cadena + c;}
					//Si viene un salto de linea se guarda un espacio (si no hacia esto se imprimia un caracter random)
					if (c == '\n') {
						cadena = cadena + ' ';
					}
					//Si viene un fin de archivo se guarda un espacio y se vuelven a leer caracteres del archivo desde el comienzo
					if (c == EOF) {
						cadena = cadena + ' ';
						fclose(archivo);
						archivo = fopen("LoremIpsum.txt","r"); }
					}
				//Una vez que se tiene un mensaje completo se envia al cliente elegido aleatoriamente de la lista
				this->enviar(cadena, clienteAleatorioAEnviar);
				//Al enviar un mensaje el tiempo de referencia es el actual
				tiempoInicio = clock();
		}
		else {cout << "Se cerro la conexion con el servidor" << endl;}
	}
	fclose(archivo);
}

string Cliente::getNombre() {
	return this->nombre;
}

int Cliente::getOpcionMenu() {
	return this->opcionMenu;
}

pthread_t Cliente::getThreadComunicacion() {
	return this->threadComunicacion;
}

list<string> Cliente::getClientesDisponibles() {
	return this->clientesDisponibles;

}

void Cliente::setThreadComunicacion(pthread_t thrComu) {
	this->threadComunicacion = thrComu;
}

void Cliente::vaciarClientesDisponibles() {
	this->clientesDisponibles.clear();
}

void Cliente::mostrarClientesDisponibles(){
	int numeroCliente = 1;
	cout << "Los usuarios disponibles son: " << endl;
	for (list<string>::iterator i = this->clientesDisponibles.begin(); i != this->clientesDisponibles.end(); i++) {
		cout << numeroCliente << ") " << (*i) << endl;
		numeroCliente ++; }
	cout << numeroCliente << ") Enviar mensaje a todos los usuarios" << endl;
}

string Cliente::devolverNombre(int numeroDestinatario) {
	list<string>::iterator iterador = this->clientesDisponibles.begin();
	advance(iterador, numeroDestinatario);
	return (*iterador);
}
void Cliente::setOpcionMenu(int opcion){
	this->opcionMenu = opcion;
}

bool Cliente::getTermino()
{
	return this->terminoComunicacion;
}
