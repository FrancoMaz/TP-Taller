#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>

using namespace std;

Cliente::Cliente(char* ip, int puerto) {
//El cliente se crea con la direccion IP y el puerto en el cual se encuentra disponible el servidor
	this->direccionIP = ip;
	this->puertoServidor = puerto;
	this->socketCliente = socket(PF_INET, SOCK_STREAM, 0);
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_port = htons(puerto);
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
	memset(direccionServidor.sin_zero, '\0', sizeof direccionServidor.sin_zero);
	this->opcionMenu = 0;
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

void Cliente::mostrarMenuYProcesarOpcion() {
	do {
		cout << "1) Enviar Mensaje" << endl;
		cout << "2) Recibir Mensajes" << endl;
		cout << "3) Lorem Ipsum" << endl;
		cout << "4) Desconectar" << endl;
		cout << "5) Salir" << endl;
		cout << "Elija la accion que desea realizar: " << endl;
		cin >> opcionMenu;
	} while (opcionMenu < 1 && opcionMenu > 5);
	this->elegirOpcionDelMenu(opcionMenu);
}

void Cliente::elegirOpcionDelMenu(int opcion) {
	switch (opcion) {
	case 1: {
		char* mensaje = "CLIENTE: Te pregunto";
		char buffer [BUFFER_MAX_SIZE];
		char datosRecibidos[BUFFER_MAX_SIZE];
		strcpy(buffer,mensaje);
		send(socketCliente, buffer, strlen(mensaje) + 1, 0);
		recv(socketCliente,datosRecibidos,BUFFER_MAX_SIZE,0);
		cout << "Recibi: " << datosRecibidos << endl;
		break;
		/*string destinatario;
		string mensajeAEnviar;
		cout << "Escriba el nombre del destinatario del mensaje: " << endl;
		cin >> destinatario;
		cout << "Escriba su mensaje: " << endl;
		cin >> mensajeAEnviar;
		this->enviar(mensajeAEnviar, destinatario);*/
		break;

	}
	case 2: {
		this->recibir();
		break;
	}
	case 3: {
		int frecuenciaDeEnvios;
		int cantidadMaximaDeEnvios;
		cout << "Escriba la frecuencia de envios: " << endl;
		cin >> frecuenciaDeEnvios;
		cout << "Escriba la cantidad maxima de envios: " << endl;
		cin >> cantidadMaximaDeEnvios;
		this->loremIpsum(frecuenciaDeEnvios, cantidadMaximaDeEnvios);
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

list<string> Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles
	list<string> clientesDisponibles;
	char buffer [BUFFER_MAX_SIZE];
	char datosRecibidos [BUFFER_MAX_SIZE];
	this->addr_size = sizeof direccionServidor;
	char* nombreYPass = strdup((nombre + ',' + contrasenia).c_str());
	cout << nombreYPass << endl;
	cout << "Intentando conectarse con el servidor. . ." << endl;
	if (connect(socketCliente, (struct sockaddr *) &direccionServidor,
			addr_size) == 0) {
		strcpy(buffer, nombreYPass);
		send(socketCliente, buffer, strlen(nombreYPass) + 1, 0);
		cout << "Conectandose al puerto: " << this->puertoServidor << endl;
		this->nombre = nombre;
		//this->clientesDisponibles.push_front("hola"); //pongo cualquier cosa para comprobar el ciclo ok.
		recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
		cout << datosRecibidos << endl; //IMPRIMO LA RESPUESTA DEL SERVER, SI SE PUDO AUTENTICAR O NO. --> HAY QUE CAMBIAR ESTO PORQUE EN REALIDAD SE DEVUELVE LA LISTA DE USUARIOS.
		//this->clientesDisponibles.push_front(datosRecibidos); //pongo cualquier cosa para que me autentique.
	} else {
		cout << "Error conectandose al puerto" << endl;
	}
	return clientesDisponibles;
	//Faltaria que el servidor devuelve la lista con los usuarios disponibles y que confirme la autenticacion del cliente
}

void Cliente::desconectar() {
	//Se cierra la conexion con el servidor
	close(socketCliente);
}

void Cliente::salir() {
	//Se termina la ejecucion del programa
	close(socketCliente);
}

void Cliente::enviar(string mensaje, string destinatario) {
	//Se envia un mensaje a un usuario o a todos (este ultimo caso sucede cuando el destinatario es el string "Todos".
	//Hay que realizar el submenu dinamico con todos los usuarios disponibles.
	//Requiere una conexion abierta.
	Mensaje *mensajeAEnviar = new Mensaje(this->nombre, destinatario, mensaje);
	char buffer[BUFFER_MAX_SIZE];
	char* stringDatosMensaje = mensajeAEnviar->getStringDatos();
	strcpy(buffer,stringDatosMensaje);
	send(this->socketCliente, buffer, strlen(stringDatosMensaje)+1, 0);
	//HAY QUE VER SI ESTE METODO FUNCIONA CORRECTAMENTE

}

queue<Mensaje> Cliente::recibir() {
	//Se reciben todos los mensajes en la secuencia en la que fueron enviados
	//Requiere una conexion abierta.
	queue<Mensaje> *colaMensajes = new queue<Mensaje>;
	recv(this->socketCliente, reinterpret_cast<char*>(&colaMensajes),
			sizeof(colaMensajes), 0); //HAY QUE VER SI ESTE METODO FUNCIONA CORRECTAMENTE
	return *colaMensajes;
}

void Cliente::loremIpsum(int frecuenciaDeEnvios, int cantidadMaximaDeEnvios) {
	//Toma el texto de un archivo y se envian mensajes en forma ciclica. El tamanio de mensajes y el destinatario son aleatorios
	//Cuando todo el texto fue transmitido, se empieza otra vez desde el inicio
	//Requiere una conexion abierta.
	time_t tiempoInicio = time(0);
	ifstream archivo("LoremIpsum.txt");
	this->clientesDisponibles.push_back("A");
	this->clientesDisponibles.push_back("B");
	this->clientesDisponibles.push_back("C");
	this->clientesDisponibles.push_back("D");
	//LAS CUATRO LINEAS DE ARRIBA LAS PUSE PARA PROBAR LA FUNCIONALIDAD. CUANTO TENGAMOS LA LISTA DE CLIENTES ES ESA LA LISTA QUE VAMOS A USAR
	for (int i = 0; i < cantidadMaximaDeEnvios; i++) {
		int tamanioMensaje;
		srand(time(NULL));
		tamanioMensaje = (int) (rand() % 30); //Por ahora puse que el tamanio de los mensajes este entre 0 y 30, pero mas adelante eso se tiene que modificar
		char cadena[tamanioMensaje];
		int numeroDeClienteAEnviar;
		string clienteAleatorioAEnviar;
		numeroDeClienteAEnviar = (int) (rand()
				% this->clientesDisponibles.size());
		list<string>::iterator iterador = this->clientesDisponibles.begin();
		advance(iterador, numeroDeClienteAEnviar);
		clienteAleatorioAEnviar = *iterador;
		do {
		} while ((time(0) - tiempoInicio) < frecuenciaDeEnvios); //este loop esta vacio porque quiero que no haga nada hasta que pase cierto tiempo. De ultima despues vemos de arreglarlo si no esta bien
		archivo.get(cadena, tamanioMensaje);
		this->enviar(cadena, clienteAleatorioAEnviar);
		tiempoInicio = time(0);
		if (archivo.eof()) {
			archivo.clear();
			archivo.seekg(0, ios::beg);
		}
		cout << cadena << endl;
	}
	archivo.close();
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

void Cliente::setClientesDisponibles(string nombre, string contrasenia) {
this->clientesDisponibles = this->conectar(nombre, contrasenia);
}
