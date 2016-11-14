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
		for (int j = 0; j < sprites.size(); j++){
			if(!verificarExistencia(sprites.at(j)->getPath())){this->cargarImagenPorDefecto(sprites.at(j));}
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


void Cliente::corroborarConexion() {
 	int ok = 1;
	while(ok>0){
		usleep(5000000);
		char buffer[BUFFER_MAX_SIZE];
		char* escuchando = "3|";
		strcpy(buffer, escuchando);
		clock_t tiempoInicio = clock();
		pthread_mutex_lock(&mutexSocket);

		ok = send(this->socketCliente, escuchando, strlen(escuchando), 0);
		if (ok > 0)
		{
			ok = recv(this->socketCliente, buffer, BUFFER_MAX_SIZE, 0);
		}
		pthread_mutex_unlock(&mutexSocket);
 	}
	cout << "Se cerro la conexion con el servidor" << endl;
	this->terminoComunicacion = true;
	/*if (this->opcionMenu != 4) {
		this->terminoComunicacion = true;
		int opcion = 1;
		while (opcion != 5) {
			cout << endl;
			cout << "Se cerro la conexion con el servidor" << endl;
			cin >> opcion; }
		if (opcion == 5) {
			this->salir();
		}
	}*/
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
	this->desconectar();
}

void Cliente::enviar(string mensaje, string destinatario) {
	//Se envia un mensaje a un usuario o a todos (este ultimo caso sucede cuando el destinatario es el string "Todos").
	if(!(this->terminoComunicacion)){
		char* mensajeCadena = strdup(mensaje.c_str());
		Mensaje *mensajeAEnviar = new Mensaje(this->nombre, destinatario, mensaje);
		char* stringDatosMensaje = strdup(("1|" + mensajeAEnviar->getStringDatos()).c_str()); //1 significa enviar.
		int largo = strlen(stringDatosMensaje);
		int largoRequest = 0;
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

}

string Cliente::recibir() {
		//Se reciben todos los mensajes en la secuencia en la que fueron enviados
		char colaMensajes[BUFFER_MAX_SIZE];
		memset(colaMensajes, '\0', BUFFER_MAX_SIZE);
		string metodo = "2|" + this->nombre + "#";
		char* recibir = strdup(metodo.c_str()); //2 es recibir
		pthread_mutex_lock(&mutexSocket);
		int ok = send(this->socketCliente, recibir, strlen(recibir), 0);
		string datosRecibidos = "";
		int largoRequest;
		//do {
			largoRequest = recv(this->socketCliente, colaMensajes, BUFFER_MAX_SIZE, 0);
		//} while (largoRequest == 0 and !this->terminoComunicacion);

		datosRecibidos += string(colaMensajes);
		memset(colaMensajes, '\0', strlen(colaMensajes));
		if (largoRequest > 0) {
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
		else {
			return "0";
		}
}

bool Cliente::stringTerminaCon(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
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
	//pthread_mutex_lock(&mutexSocket);
	do{
		largoEnviado += send(socketCliente,buffer,strlen(req),0);
	} while (largoEnviado < largoTotal);
	//pthread_mutex_unlock(&mutexSocket);
}

string Cliente::recibirResponse(){
	char colaMensajes[BUFFER_MAX_SIZE];
	memset(colaMensajes, '\0', BUFFER_MAX_SIZE);
	int largoRecibido = 0;
	string datosRecibidos = "";
	//pthread_mutex_lock(&mutexSocket);
	do
	{
		largoRecibido += recv(socketCliente,colaMensajes,BUFFER_MAX_SIZE,0);
		datosRecibidos += string(colaMensajes);
		memset(colaMensajes, '\0', strlen(colaMensajes));
	}while (largoRecibido >= BUFFER_MAX_SIZE and !stringTerminaCon(datosRecibidos, "@")); //mientras el largoRequest sea del tamaño del max size, sigo pidiendo
	//pthread_mutex_unlock(&mutexSocket);
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
	string s = response;
	string delimitador = "|";
	string delimitadorFinal = "#";
	string delimitadorCapas = ",";
	string texto;
	string strAux;
	size_t pos = s.find(delimitador);
	size_t posAux;
	texto = s.substr(0,pos);

	string nombreJugador;
	string x,xJugador;
	string y,yJugador;
	string sprite,spriteJugador;
	string camaraX;
	string camaraY;

	string comenzo = texto;
	if (comenzo != "0"){
		return false;
	}
	else
	{
		s.erase(0, pos + delimitador.length());
		string del = "camara|";
		pos = s.find(del);
		texto = s.substr(0,pos);
		while ((posAux = texto.find(delimitadorFinal)) != string::npos)
		{
			posAux = texto.find(delimitador);
			nombreJugador = texto.substr(0,posAux);
			texto.erase(0,posAux+delimitador.length());
			posAux = texto.find(delimitador);
			x = texto.substr(0,posAux);
			texto.erase(0,posAux+delimitador.length());
			posAux = texto.find(delimitador);
			y = texto.substr(0,posAux);
			texto.erase(0,posAux+delimitador.length());
			posAux = texto.find(delimitadorFinal);
			sprite = texto.substr(0,posAux);
			texto.erase(0,posAux+delimitadorFinal.length());
			if(this->nombre == nombreJugador){
				xJugador = x;
				yJugador = y;
				spriteJugador = sprite;
			}
			else{
				vista->cargarVistaInicialJugador(nombreJugador,atoi(x.c_str()),atoi(y.c_str()),buscarSprite(sprite));
			}
		}
		vista->cargarVistaInicialJugador(this->nombre,atoi(xJugador.c_str()),atoi(yJugador.c_str()),buscarSprite(spriteJugador));
		s.erase(0,pos+del.length());
		pos = s.find(delimitador);
		camaraX = s.substr(0,pos);
		s.erase(0,pos+delimitador.length());
		pos = s.find(delimitador);
		camaraY = s.substr(0,pos);
		s.erase(0,pos+delimitador.length());
		pos = s.find(delimitadorFinal);
		texto = s.substr(0,pos);
		vector<pair<int,int>> abscisasCapas;
		while ((posAux = texto.find(delimitador)) != string::npos)
		{
			pair<int,int> abscisas;
			posAux = texto.find(delimitadorCapas);
			abscisas.first = atoi(texto.substr(0,posAux).c_str());
			texto.erase(0,posAux+delimitadorCapas.length());
			posAux = texto.find(delimitador);
			abscisas.second = atoi(texto.substr(0,posAux).c_str());
			abscisasCapas.push_back(abscisas);
			texto.erase(0,posAux+delimitador.length());
		}
		s.erase(0,pos+delimitadorFinal.length());
		vista->inicializarCamara(atoi(camaraX.c_str()),atoi(camaraY.c_str()),atoi(handshake->getAncho().c_str()), atoi(handshake->getAlto().c_str()), abscisasCapas, handshake->getImagenes());
	}
	return true;
}

string Cliente::getHandshakeRecibido()
{
	return this->handshakeRecibido;
}
