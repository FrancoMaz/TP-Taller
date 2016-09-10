#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>

using namespace std;

Cliente::Cliente(string ip, int puerto) {
//El cliente se crea con la direccion IP y el puerto en el cual se encuentra disponible el servidor
	this->direccionIP = strdup(ip.c_str());
	this->puertoServidor = puerto;
	this->socketCliente = socket(PF_INET, SOCK_STREAM, 0);
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_port = htons(puerto);
	direccionServidor.sin_addr.s_addr = inet_addr(this->direccionIP);
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
		/*char* mensaje = "CLIENTE: Te pregunto";
		char buffer[BUFFER_MAX_SIZE];
		char datosRecibidos[BUFFER_MAX_SIZE];
		strcpy(buffer, mensaje);
		send(socketCliente, buffer, strlen(mensaje) + 1, 0);
		recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
		cout << "Recibi: " << datosRecibidos << endl;
		break;*/
		string lectura;
		string destinatario;
		string mensajeAEnviar;
		this -> mostrarClientesDisponibles();
		cout << "Escriba el nombre del destinatario del mensaje " << endl;
		cout <<	"(si quiere mandarle mensaje a todos los usuarios de la lista escriba Todos): " << endl;
		cin >> destinatario;
		cin.ignore();
		cout << "Escriba su mensaje: " << endl;
		getline(cin,mensajeAEnviar);
		this->enviar(mensajeAEnviar, destinatario);
		break;

	}
	case 2: {
		this->recibir();
		break;
	}
	case 3: {
		double frecuenciaDeEnvios;
		double cantidadMaximaDeEnvios;
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

void Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles
	char buffer[BUFFER_MAX_SIZE];
	char datosRecibidos[BUFFER_MAX_SIZE];
	this->addr_size = sizeof direccionServidor;
	char* nombreYPass = strdup((nombre + ',' + contrasenia).c_str()); // convierte el string de const char* a char*
	cout << nombreYPass << endl;
	cout << "Intentando conectarse con el servidor. . ." << endl;
	if (connect(socketCliente, (struct sockaddr *) &direccionServidor,addr_size) == 0) {

		strcpy(buffer, nombreYPass);
		send(socketCliente, buffer, strlen(nombreYPass) + 1, 0);

		cout << "Conectandose al puerto: " << this->puertoServidor << endl;
		this->nombre = nombre;
		//this->clientesDisponibles.push_front("hola"); //pongo cualquier cosa para comprobar el ciclo ok.
		recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
		string datos = datosRecibidos;
		string desconectarse = "Desconectar";
		if (strcmp(datos.c_str(), desconectarse.c_str()) == 0) {
			this->desconectar();
		} else {
			cout << "Datos recibidos: " << datos << endl; //IMPRIMO LA RESPUESTA DEL SERVER, SI SE PUDO AUTENTICAR O NO. --> HAY QUE CAMBIAR ESTO PORQUE EN REALIDAD SE DEVUELVE LA LISTA DE USUARIOS.
			splitUsuarios(datosRecibidos);
		}
	} else {
		cout << "Error conectandose al puerto" << endl;
	}
	//Faltaria que el servidor devuelve la lista con los usuarios disponibles y que confirme la autenticacion del cliente
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
	cout << "Desconectando al cliente" << endl;
	close(socketCliente);
}

void Cliente::enviar(string mensaje, string destinatario) {
	//Se envia un mensaje a un usuario o a todos (este ultimo caso sucede cuando el destinatario es el string "Todos").
	string metodo = "Enviar";
	char* mensajeCadena = strdup(mensaje.c_str());
	for (int i = 0; i < strlen(mensajeCadena); i += BUFFER_MAX_SIZE) {
		Mensaje *mensajeAEnviar = new Mensaje(this->nombre, destinatario, mensaje);
		char buffer[BUFFER_MAX_SIZE];
		string largoMensaje;
		stringstream conversion;
		conversion << strlen(mensajeCadena);
		largoMensaje = conversion.str();
		char* stringDatosMensaje = strdup((largoMensaje + '|' + metodo + '|' + mensajeAEnviar->getStringDatos()).c_str());
		cout << stringDatosMensaje << endl;
		strcpy(buffer, stringDatosMensaje);
		send(this->socketCliente, buffer, strlen(stringDatosMensaje) + 1, 0);
	}
}

void Cliente::recibir() {
	//Se reciben todos los mensajes en la secuencia en la que fueron enviados
	char colaMensajes[BUFFER_MAX_SIZE];
	string metodo = "Recibir|";
	char* metodoYNombre = strdup((metodo + this -> nombre).c_str());
	send(this->socketCliente, metodoYNombre, strlen(metodoYNombre) + 1, 0);
	recv(this->socketCliente, colaMensajes, strlen(colaMensajes), 0);
	this -> mostrarUltimosMensajes(colaMensajes);
}

void Cliente::mostrarUltimosMensajes(string colaMensajes)
{
	string mensajesASplitear = colaMensajes.substr(0, colaMensajes.length() - 1);
	char str[BUFFER_MAX_SIZE];
	strcpy(str, colaMensajes.c_str());
	char* mensaje = strtok(str, "||");
	cout << "Ultimos mensajes recibidos: " << endl;
	while (mensaje != NULL) {
		cout << mensaje << endl;
		mensaje = strtok(NULL, "||");
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
	//Por ahora puse que el tamanio de los mensajes este entre 0 y 30, pero mas adelante eso se tiene que modificar
	tamanioMensaje = (int) (rand() % 30);
	//Se elige aleatoriamente el destinatario de la secuencia de mensajes
	numeroDeClienteAEnviar = (int) (rand()% this->clientesDisponibles.size());
	list<string>::iterator iterador = this->clientesDisponibles.begin();
	advance(iterador, numeroDeClienteAEnviar);
	clienteAleatorioAEnviar = *iterador;
	//tiempoPorMensaje indica cada cuanto tiempo se manda un mensaje (en segundos)
	double tiempoPorMensaje = (frecuenciaDeEnvios/cantidadMaximaDeEnvios);
	//Indico el tiempo de inicio
	clock_t tiempoInicio = clock();
	for (int i = 0; i < cantidadMaximaDeEnvios; i++) {
		char cadena[tamanioMensaje];
		//No tiene que ocurrir nada hasta que el tiempo transcurrido sea igual al tiempo estipulado de envio para cada mensaje
		do {
		} while (((double)(clock()-tiempoInicio)/CLOCKS_PER_SEC) < tiempoPorMensaje);
		//Una vez que se transcurren tiempoPorMensaje segundos se empieza a leer caracteres del archivo
			for (int j=0; j < tamanioMensaje; j++) {
				char c = fgetc(archivo);
				//Si no hay salto de linea o fin de archivo se guarda el caracter leido
				if (c != '\n' && c != EOF) {
					cadena[j] = c;}
				//Si viene un salto de linea se guarda un espacio (si no hacia esto se imprimia un caracter random)
				if (c == '\n') {
					cadena[j] = ' ';
				}
				//Si viene un fin de archivo se guarda un espacio y se vuelven a leer caracteres del archivo desde el comienzo
				if (c == EOF) {
					cadena[j] = ' ';
					fclose(archivo);
					archivo = fopen("LoremIpsum.txt","r"); }
				}
			//Una vez que se tiene un mensaje completo se envia al cliente elegido aleatoriamente de la lista
			//DESPUES HAY QUE DESCOMENTAR LA LINEA DE ABAJO. Falta terminar el metodo enviar, por lo que para probar la funcionalidad la deje comentada

			this->enviar(cadena, clienteAleatorioAEnviar);
			//Al enviar un mensaje el tiempo de referencia es el actual
			tiempoInicio = clock();
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

void Cliente::setClientesDisponibles(string nombre, string contrasenia) {
	//this->clientesDisponibles = this->conectar(nombre, contrasenia);
}

void Cliente::mostrarClientesDisponibles(){
	cout << "Los usuarios disponibles son: " << endl;
	for (list<string>::iterator i = this->clientesDisponibles.begin(); i != this->clientesDisponibles.end(); i++) {
		cout << (*i) << endl;
	}
}
