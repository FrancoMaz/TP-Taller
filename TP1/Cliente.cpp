#include <stdlib.h>
#include "Cliente.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>


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

//----------------------------METODOS PARA EL HANDSHAKE-------------------------------------
ImagenDto* Cliente::deserializarImagen(string campo){

	string nombre, ancho, alto, zIndex;
	string s = campo;
	string delimitador = ",";
	size_t pos = s.find(delimitador);
	nombre = s.substr(0,pos);
	s.erase(0,pos + delimitador.length());
	pos = s.find(delimitador);
	ancho = s.substr(0,pos);
	s.erase(0,pos + delimitador.length());
	pos = s.find(delimitador);
	alto = s.substr(0,pos);
	s.erase(0,pos + delimitador.length());
	zIndex = s;
	ImagenDto* imagenReconstruida = new ImagenDto(nombre,ancho,alto,zIndex);
	return imagenReconstruida;
}

SetDeSpritesDto* Cliente::deserializarSprite(string campo){


	string id, cantFotogramas, ancho, alto, imagen, zIndex;
	string carpeta;
	vector<SpriteDto*> spritesAccion;
	string s = campo;
	string delimitador = ";";
	string delimitadorSprite = ",";
	size_t pos = s.find(delimitador);
	size_t posSprite;
	string campoSprite;
	carpeta = s.substr(0,pos);
	s.erase(0,pos + delimitador.length());
	while ((pos = s.find(delimitador)) != string::npos)
	{
		campoSprite = s.substr(0,pos);
		posSprite = campoSprite.find(delimitadorSprite);
		id = campoSprite.substr(0,posSprite);
		campoSprite.erase(0,posSprite + delimitadorSprite.length());
		posSprite = campoSprite.find(delimitadorSprite);
		cantFotogramas = campoSprite.substr(0,posSprite);
		campoSprite.erase(0,posSprite + delimitadorSprite.length());
		posSprite = campoSprite.find(delimitadorSprite);
		ancho = campoSprite.substr(0,posSprite);
		campoSprite.erase(0,posSprite + delimitadorSprite.length());
		posSprite = campoSprite.find(delimitadorSprite);
		alto = campoSprite.substr(0,posSprite);
		campoSprite.erase(0,posSprite + delimitadorSprite.length());
		posSprite = campoSprite.find(delimitadorSprite);
		imagen = campoSprite.substr(0,posSprite);
		campoSprite.erase(0,posSprite + delimitadorSprite.length());
		zIndex = campoSprite;
		spritesAccion.push_back(new SpriteDto(id,cantFotogramas,ancho,alto,imagen,zIndex));
		s.erase(0,pos + delimitador.length());
	}

	SetDeSpritesDto* setSpriteReconstruido = new SetDeSpritesDto(carpeta,spritesAccion);

	return setSpriteReconstruido;
}

Handshake* Cliente::deserializarHandshake(string handshake, bool primeraVez){

	Handshake* handshakeAux;
	string s = handshake;
	string delimitador = "[";
	string campo;
	size_t pos = s.find(delimitador);
	campo = s.substr(0, pos);
    vector<ImagenDto*> imagenes;
    vector<SetDeSpritesDto*> setsSprites;
    string ancho;
    string alto;
    string delimitadorCampo = "";
	if (campo == "Escenario"){
		string campoImagen;
		s.erase(0, pos + delimitador.length());
		delimitador = "]-";
		pos = s.find(delimitador);
		campo = s.substr(0,pos);
		delimitadorCampo = "|";
		while ((pos = campo.find(delimitadorCampo)) != string::npos)
		{
			campoImagen = campo.substr(0,pos);
			imagenes.push_back(deserializarImagen(campoImagen));
			campo.erase(0, pos + delimitadorCampo.length());
		}
		pos = s.find(delimitador);
		s.erase(0,pos + delimitador.length());
		delimitador = "[";
		pos = s.find(delimitador);
		campo = s.substr(0,pos);
	}
	if(campo == "SetSprites"){
		string campoSprite;
		s.erase(0, pos + delimitador.length());
		delimitador = "]";
		pos = s.find(delimitador);
		campo = s.substr(0,pos);
		delimitadorCampo = "|-";
		while ((pos = campo.find(delimitadorCampo)) != string::npos)
		{
			campoSprite = campo.substr(0,pos);
			setsSprites.push_back(deserializarSprite(campoSprite));
			campo.erase(0, pos + delimitadorCampo.length());
		}
		pos = s.find(delimitador);
		s.erase(0,pos + delimitador.length());
		delimitador = "[";
		pos = s.find(delimitador);
		campo = s.substr(0,pos);
	}
	if(campo == "Ventana"){
		s.erase(0, pos + delimitador.length());
		delimitador = ",";
		pos = s.find(delimitador);
		campo = s.substr(0,pos);
		ancho = campo;
		s.erase(0, pos + delimitador.length());
		delimitador = "]";
		pos = s.find(delimitador);
		campo = s.substr(0,pos);
		alto = campo;
	}
	handshakeAux = new Handshake(imagenes, setsSprites, ancho, alto);
	this->verificarBiblioteca(handshakeAux);
   return handshakeAux;
}

void Cliente::recorrerSprites(vector<SpriteDto*> sprites, vector<void*> &archivos){
    for (int i = 0; i < sprites.size(); i++){
		archivos.push_back((void*)sprites.at(i));
		}
}

void Cliente::cargarImagenPorDefecto(ImagenDto* imagen)
{
	imagen->setPath("ImagenNoEncontrada");
	imagen->setAncho(to_string(226));
	imagen->setAlto(to_string(55));
}

void Cliente::cargarImagenPorDefecto(SpriteDto* sprite)
{
	sprite->setPath("ImagenNoEncontrada");
	sprite->setAncho(to_string(226));
	sprite->setAlto(to_string(55));
	sprite->setCantFotogramas(to_string(0));
}

bool Cliente::verificarExistencia(string archivo){

	string recursos = "Recursos/";
	string extension = ".png";
	string rutaArchivo = recursos + archivo + extension;
	ifstream infile(rutaArchivo.c_str());
	return infile.good();
}

void Cliente::verificarBiblioteca(Handshake* handshakeAux) {
	vector<ImagenDto*> imagenes = handshakeAux->getImagenes();
	vector<SetDeSpritesDto*> setsSprites = handshakeAux->getSprites();
	for (int i = 0; i < imagenes.size(); i++){
		if(!verificarExistencia(imagenes.at(i)->getPath())){this->cargarImagenPorDefecto(imagenes.at(i));}
	}
	for (int i = 0; i < setsSprites.size(); i++){
		vector<SpriteDto*> sprites = setsSprites.at(i)->getSprites();
		for (int j = 0; i < sprites.size(); i++){
			if(!verificarExistencia(sprites.at(j)->getID())){this->cargarImagenPorDefecto(sprites.at(i));}
		}
	}

}

void Cliente::recibirHandshake(){

       char colaMensaje[BUFFER_MAX_SIZE];
       memset(colaMensaje, '\0', BUFFER_MAX_SIZE);
       string datosRecibidos = "";
       int largoRequest;
       do {
               largoRequest = recv(this->socketCliente, colaMensaje, BUFFER_MAX_SIZE, 0);
       } while (largoRequest == 0);

       datosRecibidos += string(colaMensaje);
       memset(colaMensaje, '\0', strlen(colaMensaje));
       while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")){ //mientras el largoRequest sea del tamaño del max size, sigo pidiendo
               int largo;
                       //mientras haya cosas que leer, sigo recibiendo.
               do {//sigue aca mientras no recibe nada, cuando recibe algo sale de este do while
                       largo = recv(socketCliente, colaMensaje, BUFFER_MAX_SIZE, 0);
                       largoRequest += largo;
               } while (largoRequest < BUFFER_MAX_SIZE);
               if (largoRequest > 0){datosRecibidos += string(colaMensaje); }
               memset(colaMensaje, '\0', strlen(colaMensaje));
       }
       //cout<<"handshake recibido: "<<datosRecibidos<<endl;
       this->handshakeRecibido = datosRecibidos;
       this-> handshake = this->deserializarHandshake(datosRecibidos, true);
}

Handshake* Cliente::getHandshake()
{
	return this->handshake;
}
//-------------------------------ACA TERMINAN LOS METODOS PARA EL HANDSHAKE-------------------------------

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
				if (!(this->terminoComunicacion)) {
					cout << "Escriba su mensaje: " << endl;
					getline(cin,mensajeAEnviar);
					if (numeroDestinatario != (this->clientesDisponibles.size()) + 1){
						string nombreDestinatario = this->devolverNombre(numeroDestinatario - 1);
						this->enviar(mensajeAEnviar, nombreDestinatario);
					} else {this -> enviar(mensajeAEnviar, "Todos");}
				}
			}
			break;
		}
		case 2: {
			if(!(this->terminoComunicacion)){this->recibir();}//verifica si hay conexion a la hora de recibir mensajes
			break;
		}
		case 3: {
			double frecuenciaDeEnvios;
			double cantidadMaximaDeEnvios;
			if(!(this->terminoComunicacion)){//verifica si hay conexion a la hora de mandar un mensaje
				cout << "Escriba la frecuencia de envios: " << endl;
				cin >> frecuenciaDeEnvios;
				if (!(this->terminoComunicacion)) {
					cout << "Escriba la cantidad maxima de envios: " << endl;
					cin >> cantidadMaximaDeEnvios;
					if (!this->terminoComunicacion) {
						this->loremIpsum(frecuenciaDeEnvios, cantidadMaximaDeEnvios);
						}
					}
				}
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
		usleep(5000000);
		//do {
			//} while ((double)((clock()-tiempoInicio)/CLOCKS_PER_SEC) < 5);
		pthread_mutex_lock(&mutexSocket);
		ok = send(this->socketCliente, escuchando, strlen(escuchando), 0);
		if (ok > 0)
		{
			ok = recv(this->socketCliente, buffer, BUFFER_MAX_SIZE, 0);
		}
		pthread_mutex_unlock(&mutexSocket);
 	}
	if (this->opcionMenu != 4) {
		this->terminoComunicacion = true;
		int opcion = 1;
		while (opcion != 5) {
			cout << endl;
			cout << "Se cerro la conexion con el servidor. Presione 5 para salir" << endl;
			cin >> opcion; }
		if (opcion == 5) {
			this->salir();
		}
	}
 }

bool Cliente::conectar(string nombre, string contrasenia) {
	//Se establece la conexion con el servidor mediante autenticacion. El servidor devuelve la lista con todos los usuarios disponibles
	this->inicializarSocket();
	char buffer[BUFFER_MAX_SIZE];
	char datosRecibidos[BUFFER_MAX_SIZE];
	this->addr_size = sizeof direccionServidor;
	char* nombreYPass = strdup((nombre + ',' + contrasenia).c_str()); // convierte el string de const char* a char*
	cout << "Intentando conectarse con el servidor. . ." << endl;
	//pthread_mutex_lock(&mutexSocket);
	if (connect(socketCliente, (struct sockaddr *) &direccionServidor,addr_size) == 0) {

		strcpy(buffer, nombreYPass);
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		if (setsockopt (socketCliente, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		{
			cout << "No se pudo setear el timeout del recv del socket" << endl;
		}
		if (setsockopt (socketCliente, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		{
			cout << "No se pudo setear el timeout del send del socket" << endl;
		}
		send(socketCliente, buffer, strlen(nombreYPass) + 1, 0);
		this->nombre = nombre;
		//this->clientesDisponibles.push_front("hola"); //pongo cualquier cosa para comprobar el ciclo ok.
		recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
		//pthread_mutex_unlock(&mutexSocket);
		string datos = datosRecibidos;
		string desconectarse = "Desconectar";
		if (strcmp(datos.c_str(), desconectarse.c_str()) == 0) {
			cout << "Usuario/clave incorrectos, inténtelo de nuevo" << endl;
			close(socketCliente);
			//pthread_mutex_unlock(&mutexSocket);
		} else {
			this->nombre = nombre;
			string opcion = "6|" + nombre;
			cout << "Conectandose al puerto: " << this->puertoServidor << endl;
			cout<<opcion<<endl;
			memset(buffer, '\0', strlen(buffer));
			strcpy(buffer,opcion.c_str());
			//pthread_mutex_lock(&mutexSocket);
			send(socketCliente,buffer,opcion.length(),0);
			splitUsuarios(datosRecibidos);
			this->recibirHandshake();
			//pthread_mutex_unlock(&mutexSocket);
			return true;
		}
		//pthread_mutex_unlock(&mutexSocket);
	} else {
		cout << "Error conectandose al puerto" << endl;
	}
	//free(nombreYPass);
	return false;
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
	char buffer[BUFFER_MAX_SIZE];
	char* desconectar = "4|";
	strcpy(buffer, desconectar);
	pthread_mutex_lock(&mutexSocket);
	send(socketCliente,buffer,strlen(desconectar),0);
	pthread_mutex_unlock(&mutexSocket);
	close(socketCliente);
}

void Cliente::salir() {
	//Se termina la ejecucion del programa
	this->opcionMenu = 5;
	//cout << "Desconectando al cliente" << endl;
	this->desconectar();
}

void Cliente::enviar(string mensaje, string destinatario) {
	//Se envia un mensaje a un usuario o a todos (este ultimo caso sucede cuando el destinatario es el string "Todos").
	if(!(this->terminoComunicacion)){
		char* mensajeCadena = strdup(mensaje.c_str());
		Mensaje *mensajeAEnviar = new Mensaje(this->nombre, destinatario, mensaje);
		char* stringDatosMensaje = strdup(("1|" + mensajeAEnviar->getStringDatos()).c_str()); //1 significa enviar.
		int largo = strlen(stringDatosMensaje);
		int largoRequest;
		//cout<<"Mensaje enviado: "<<mensaje<<endl;
		pthread_mutex_lock(&mutexSocket);
		while (largo > 0)
		{
			largoRequest = send(this->socketCliente, stringDatosMensaje, largo, 0);
			largo -= largoRequest;
		}
		pthread_mutex_unlock(&mutexSocket);
		free(mensajeCadena);
		mensajeAEnviar->~Mensaje();
	}

	//free(stringDatosMensaje);
}

void Cliente::enviarMensajeATodos(string mensaje) {
	for (list<string>::iterator i = this->clientesDisponibles.begin(); i != this->clientesDisponibles.end(); i++) {
			this -> enviar(mensaje,*i);
	}
}

string Cliente::recibir() {
	//Se reciben todos los mensajes en la secuencia en la que fueron enviados
	char colaMensajes[BUFFER_MAX_SIZE];
	memset(colaMensajes, '\0', BUFFER_MAX_SIZE);
	string metodo = "2|" + this->nombre + "#";
	char* recibir = strdup(metodo.c_str()); //2 es recibir
	pthread_mutex_lock(&mutexSocket);
	send(this->socketCliente, recibir, strlen(recibir), 0);
	string datosRecibidos = "";
	int largoRequest;
	do {
		largoRequest = recv(this->socketCliente, colaMensajes, BUFFER_MAX_SIZE, 0);
	} while (largoRequest == 0);

	datosRecibidos += string(colaMensajes);
	memset(colaMensajes, '\0', strlen(colaMensajes));
	while (largoRequest >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")) //mientras el largoRequest sea del tamaño del max size, sigo pidiendo
	{
		int largo;
		//mientras haya cosas que leer, sigo recibiendo.
		do {
			//sigue aca mientras no recibe nada, cuando recibe algo sale de este do while
			largo = recv(socketCliente, colaMensajes, BUFFER_MAX_SIZE, 0);
			largoRequest += largo;
		}
		while (largoRequest < BUFFER_MAX_SIZE);
		if (largoRequest > 0){
			datosRecibidos += string(colaMensajes);
		}
		memset(colaMensajes, '\0', strlen(colaMensajes));
	}
	pthread_mutex_unlock(&mutexSocket);
	return datosRecibidos;
}

bool Cliente::stringTerminaCon(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}

void Cliente::mostrarUltimosMensajes(string colaMensajes)
{   string mensajeVacio = "#noHayMensajes@";
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
	double tiempoPorMensaje = (1000000/frecuenciaDeEnvios);
	cout<<"tiempo por mensaje: "<<tiempoPorMensaje<<endl;
	//Indico el tiempo de inicio
	clock_t tiempoInicio = clock();
	for (int i = 0; i < cantidadMaximaDeEnvios; i++) {
		if (!this->terminoComunicacion) {
			string cadena;
			//No tiene que ocurrir nada hasta que el tiempo transcurrido sea igual al tiempo estipulado de envio para cada mensaje
			usleep(tiempoPorMensaje);
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
				//tiempoInicio = clock();
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

void Cliente::enviarRequest(string request){
	char buffer[BUFFER_MAX_SIZE];
	const char* req = request.c_str();
	strcpy(buffer, request.c_str());
	int largoEnviado = 0;
	int largoTotal = strlen(req);
	pthread_mutex_lock(&mutexSocket);
	do{
		largoEnviado += send(socketCliente,buffer,strlen(req),0);
	} while (largoEnviado < largoTotal);
	pthread_mutex_unlock(&mutexSocket);
}

string Cliente::recibirResponse(){
	char colaMensajes[BUFFER_MAX_SIZE];
	memset(colaMensajes, '\0', BUFFER_MAX_SIZE);
	int largoRecibido = 0;
	string datosRecibidos = "";
	pthread_mutex_lock(&mutexSocket);
	do
	{
		largoRecibido += recv(socketCliente,colaMensajes,BUFFER_MAX_SIZE,0);
		datosRecibidos += string(colaMensajes);
		memset(colaMensajes, '\0', strlen(colaMensajes));
	}while (largoRecibido >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")); //mientras el largoRequest sea del tamaño del max size, sigo pidiendo
	pthread_mutex_unlock(&mutexSocket);
	return datosRecibidos;
}

SpriteDto* Cliente::buscarSprite(string ID){

	vector<SetDeSpritesDto*> setsSprites = this->handshake->getSprites();
	for (int i = 0; i < setsSprites.size(); i++){
		vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
		for (int i = 0; i < listaSprites.size(); i++)
		{
			if ( ID == listaSprites.at(i)->getID())
			{
				return listaSprites.at(i);
			}
		}
					}
 return NULL;
}

bool Cliente::checkearInicioJuego(Vista* vista)
{
	//Devuelve falso si no se inicio el juego o true si se inicio. A su vez carga la vista inicial de los jugadores en caso de que se haya iniciado.
	this->enviarRequest("5|"); //5 es el case de request se inicio el juego.
	string response = this->recibirResponse();
	response = response.substr(0,response.length() - 1);
	char strResponse[response.length()];
	strcpy(strResponse, response.c_str());
	char* strComenzo = strtok(strResponse,"|#");

	char* nombreJugador;
	char* x;
	char* y;
	char* sprite;
	char* camaraX;
	char* camaraY;

	int comenzo = atoi(strComenzo);
	if (comenzo != 0){
		return false;
	}
	else{
		strComenzo = strtok(NULL,"|#");
		while (strComenzo != NULL){
			nombreJugador = strComenzo;
			strComenzo = strtok(NULL,"|#");
			x = strComenzo;
			strComenzo = strtok(NULL,"|#");
			y = strComenzo;
			strComenzo = strtok(NULL,"|#");
			sprite = strComenzo;
			strComenzo = strtok(NULL,"|#");
			vista->cargarVistaInicialJugador(nombreJugador,atoi(x),atoi(y),buscarSprite(sprite));
			if (strcmp(strComenzo, "camara") == 0)
			{
				strComenzo = strtok(NULL,"|#");
				camaraX = strComenzo;
				strComenzo = strtok(NULL,"|#");
				camaraY = strComenzo;
				strComenzo = strtok(NULL,"|#");
				vista->inicializarCamara(atoi(camaraX),atoi(camaraY),atoi(handshake->getAncho().c_str()), atoi(handshake->getAlto().c_str()));
			}
		}
		return true;
	}
	return false;
}

string Cliente::getHandshakeRecibido()
{
	return this->handshakeRecibido;
}
