/*
 * Servidor.cpp
 *
 *  Created on: 28 ago. 2016
 *      Author: franco
 */

#include "Servidor.h"
#include <string.h>
#include <stdlib.h>
#include "LTimer.h"
using namespace std;

Servidor::Servidor(char* nombreArchivoDeUsuarios, int puerto, Logger* logger) {
	this->puerto = puerto;
	this->nombreArchivo = nombreArchivoDeUsuarios;
	this->logger = logger;
	this->welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	this->serverAddr.sin_family = AF_INET;
	/* Set port number, using htons function to use proper byte order */
	this->serverAddr.sin_port = htons(puerto);
	/* Set IP address to localhost */
	//this->serverAddr.sin_addr.s_addr = inet_addr("192.168.1.10");
	this->serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//this->serverAddr.sin_addr.s_addr = inet_addr("10.1.77.13");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	/*---- Bind the address struct to the socket ----*/
	bind(this->welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	this->datosUsuarios = new list<Datos>();
	this->jugadores = new vector<Jugador*>();
	this->listaMensajesProcesados = new list<MensajesProcesados>();
	vectorEquipos = {"rojo", "verde", "amarillo", "azul"};
	stringstream ss;
	ss << puerto;
	mensaje = "Se creó correctamente el servidor en el puerto: " + ss.str() + ", ip: 192.168.1.10" + "\n";
	this->guardarLog(mensaje, DEBUG);
	this->guardarDatosDeUsuarios();
	this->guardarDatosDeConfiguracion();
	this->guardarModoDeJuego();
	posicionXInicial = 23;
	posicionVector = 0;
	camara.x = 0;
	camara.y = 0;
	for (int i = 0; i <= 3; i++)
	{
		pair<int,int> abscisas;
		abscisas.first = 0;
		abscisas.second = 0;
		abscisasCapas.push_back(abscisas);
	}
	this->nivelActual = 0;
	this->gameComplete = false;
	this->inicializarDatosNiveles();
	this->contadorJugadores = 0;
}

Servidor::~Servidor() {
}

void Servidor::guardarModoDeJuego(){
	bool modoJuego = false;
	do {
		cout << "Elija el modo de juego: " << endl;
		cout << "1) Individual Multijugador" << endl;
		cout << "2) Colaborativo Multijugador" << endl;
		cout << "3) Grupal Multijugador" << endl;
		cin >> this->modoJuegoElegido.first;
		if (cin.good() && (this->modoJuegoElegido.first == 1 || this->modoJuegoElegido.first == 2 || this->modoJuegoElegido.first == 3)) {
			modoJuego = true;
		} else {
			cin.clear();
			cin.ignore();
			cout << "Error: la opcion ingresada no es valida" << endl;
		}
	} while (!modoJuego);
	modoJuego = false;
	int modoPrueba;
	do{
		cout << "Elija modo normal o prueba:" << endl;
		cout << "1) Normal" << endl;
		cout << "2) Prueba" << endl;
		cin >> modoPrueba;
		if (cin.good() && (modoPrueba == 1 || modoPrueba == 2)){
			modoJuego = true;
			if (modoPrueba == 1){
				this->modoJuegoElegido.second = false;
			}
			else{
				this->modoJuegoElegido.second = true;
			}
		}
		else{
			cin.clear();
			cin.ignore();
			cout << "Error: la opcion ingresada no es valida";
		}
	} while(!modoJuego);
}

void Servidor::inicializarDatosNiveles()
{
	for (int i = 0; i < CANTIDADNIVELES; i++)
	{
		this->vectorNiveles.push_back(new Escenario("Recursos/nivel" + to_string(i+1) + ".xml"));
	}
}
void Servidor::guardarDatosDeConfiguracion() {
	string path;
	cin.ignore();
	cout << "Ingrese el path del archivo de configuracion" << endl;
	getline(cin,path);
	//cin >> path;

	if (this->existeArchivo(path))
	{
	   this->parser = new XmlParser(path);
	}
	else {
			if(path.empty()){
				cout << "No se ingreso ninguna ruta. Se cargara el xml por defecto" << endl;
			}
			else {
				cout << "No existe el archivo ingresado. Se cargara el xml por defecto" << endl;
			}
			this->parser = new XmlParser("Recursos/configuration.xml");
	}
	vector<ImagenDto*> escenario = this->parser->getEscenario();
	vector<SetDeSpritesDto*> setDeSprites = this->parser->getSprites();
	pair<string, string> ventana = this->parser->getTamanioVentana();
	const char* cantidadMaximaJugadores = this->parser->getCantidadMaximaDeJugadores();

	ImagenDto* imagenTemporal;
	for (int i = 0; i < escenario.size(); i++) {
		for (int j = 0; j < escenario.size() - 1; j++) {
			if (escenario.at(j)->getZIndex() > escenario.at(j+1)->getZIndex()) {
				imagenTemporal = escenario.at(j);
				escenario.at(j) = escenario.at(j+1);
				escenario.at(j+1) = imagenTemporal;
			}
		}
	}

	this->handshake = new Handshake(escenario, setDeSprites, ventana.first, ventana.second, cantidadMaximaJugadores);
}

bool Servidor::existeArchivo(string fileName) {
	ifstream infile(fileName.c_str());
	return infile.good();
}

void Servidor::enviarHandshake(int socket, char* cliente){
	string handshake = this->parser->serializarEscenario();
	handshake += this->parser->serializarSetDeSprites(this->vectorEquipos);
	handshake += this->parser->serializarVentana();
	handshake += "#@";
	int largo = strlen(handshake.c_str());
	std::ostringstream oss;
	oss << largo;
	string largoString = oss.str();
	char buffer[BUFFER_MAX_SIZE];
	int inicio = 0;
	int ok;
	if (largo > BUFFER_MAX_SIZE) {
		while (BUFFER_MAX_SIZE < (largo - inicio)) {
			string mensajeSpliteado = handshake.substr(inicio, BUFFER_MAX_SIZE);
			strcpy(buffer, mensajeSpliteado.c_str());
			ok = send(socket, buffer, strlen(buffer), 0);
			inicio += BUFFER_MAX_SIZE;
		}
		string mensajeSpliteado = handshake.substr(inicio, largo);
		strcpy(buffer, mensajeSpliteado.c_str());
	} else {
		strcpy(buffer, handshake.c_str());
	}
	//pthread_mutex_lock(&servidor->mutexSocket);
	ok = send(socket, buffer, strlen(buffer), 0);
}

void Servidor::guardarDatosDeUsuarios() {
	string linea, csvItem;
	int nroItem;
	ifstream myfile(this->nombreArchivo);
	if (myfile.is_open()) {
		while (getline(myfile, linea)) {
			Datos datosCapturados;
			MensajesProcesados mensajesProcesados;
			nroItem = 0;
			istringstream lineaActual(linea);
				while (getline(lineaActual, csvItem, ',')) {
				if (nroItem == 0) {
					datosCapturados.nombre = csvItem;
				}
				if (nroItem == 1) {
					datosCapturados.contrasenia = csvItem;
				}
				nroItem++;
			}
			queue<Mensaje>* colaMensajes = new queue<Mensaje>;
			mensajesProcesados.mensajes = colaMensajes;
			datosUsuarios->push_back(datosCapturados);
		}

		myfile.close();
	}

}

bool Servidor::contieneJugador(string nombre)
{
	bool estaContenido = false;
	for (int i=0; i <jugadores->size(); i++)
	{
		if (jugadores->at(i)->getNombre() == nombre)
		{
			estaContenido = true;
			break;
		}
	}
	return estaContenido;
}

void Servidor::autenticar(string nombre, string contrasenia, list<string>& usuarios) {

	bool autenticacionOK = false;
	this->usuarios.clear();
	for (list<Datos>::iterator datoActual = datosUsuarios->begin();
			datoActual != datosUsuarios->end(); datoActual++) {

		Datos usuario;
		usuario = *datoActual;
		if ((strcmp(usuario.nombre.c_str(), nombre.c_str()) == 0)
				&& (strcmp(usuario.contrasenia.c_str(), contrasenia.c_str())
						== 0)) {
			autenticacionOK = true;
			if (jugadores->empty() || !this->contieneJugador(usuario.nombre))
			{
				Jugador* jugador = new Jugador(usuario.nombre, this->vectorEquipos.at(posicionVector), posicionXInicial, this->vectorNiveles.at(this->nivelActual)->plataformas);
				jugador->setConectado();
				if (this->modoJuegoElegido.first == 3){
					if (this->posicionVector % 2 == 0){
						this->equipoAlfa.push_back(jugador->getNombre());
					}
					else{
						this->equipoBeta.push_back(jugador->getNombre());
					}
				}
				pthread_mutex_lock(&mutexVectorJugadores);
				jugadores->push_back(jugador);
				pthread_mutex_unlock(&mutexVectorJugadores);
				MensajesProcesados mensajesProcesados;
				mensajesProcesados.destinatario = usuario.nombre;
				queue<Mensaje>* colaMensajes = new queue<Mensaje>;
				mensajesProcesados.mensajes = colaMensajes;
				listaMensajesProcesados->push_back(mensajesProcesados);
			}
			else
			{
				Jugador* jugador = this->obtenerJugador(usuario.nombre);
				jugador->setConectado();
				jugador->setSprite("Jugador_" + jugador->getEquipo());
			}
		} else {
			usuarios.push_back(usuario.nombre);
		}
	}
	if (autenticacionOK) {
		cout << "Autenticación OK" << endl;
	} else {
		usuarios.clear();
		cout << "Error de autenticación: usuario y/o clave incorrectos" << endl;
		string msj = "Error de autenticación: usuario y/o clave incorrectos \n";
		this->guardarLog(msj, INFO);
	}
	this->posicionVector += 1;
	this->posicionXInicial += 84;
	if (this->posicionVector > this->vectorEquipos.size())
	{
		this->posicionVector = 0;
	}
}


void Servidor::guardarLog(string mensaje, const int nivelDeLog) {
	pthread_mutex_lock(&mutexLog);
	this->logger->escribir(mensaje, nivelDeLog);
	pthread_mutex_unlock(&mutexLog);
}


void Servidor::crearMensaje(Mensaje mensaje) {
	this->colaMensajesNoProcesados.push(mensaje);
}

list<Servidor::MensajesProcesados>* Servidor::getListaMensajesProcesados()
{
	return listaMensajesProcesados;
}

Jugador* Servidor::obtenerJugador(string nombre){
	for (int i = 0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		if (jugador->getNombre() == nombre)
		{
			return jugador;
			break;
		}
	}
}

pair<int,int> Servidor::obtenerPosicionesExtremos()
{
	int posicionX = jugadores->at(0)->getPosicion().first;
	int posicionMasAdelante = camara.x;
	int posicionMasAtras = camara.x + atoi(handshake->getAncho().c_str());
	for (int i = 0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		if (!jugador->getEstaMuerto())
		{
			if (jugador->getPosicion().first <= posicionMasAtras && jugador->getConectado())
			{
				posicionMasAtras = jugadores->at(i)->getPosicion().first;
			}
			else if (jugador->getPosicion().first >= posicionMasAdelante && jugador->getConectado())
			{
				posicionMasAdelante = jugadores->at(i)->getPosicion().first;
			}
		}
	}
	pair<int,int> posiciones;
	posiciones.first = posicionMasAtras;
	posiciones.second = posicionMasAdelante;
	return posiciones;
}

int Servidor::getAnchoSprite(string sprite)
{
	int ancho = 0;
	vector<SetDeSpritesDto*> setsSprites = handshake->getSprites();
	for (int i = 0; i < setsSprites.size(); i++){
		vector<SpriteDto*> listaSprites = setsSprites.at(i)->getSprites();
		for (int i = 0; i < listaSprites.size(); i++)
		{
			if (sprite == listaSprites.at(i)->getID())
			{
				ancho = atoi(listaSprites.at(i)->getAncho().c_str());
			}
		}
	}
	return ancho;
}

string Servidor::serializarCapas()
{
	string capas = "";
	for (int i = 0; i < abscisasCapas.size(); i++)
	{
		capas += this->vectorNiveles.at(nivelActual)->capas.at(i);
		capas += ",";
		capas += to_string(abscisasCapas.at(i).first);
		capas += ",";
		capas += to_string(abscisasCapas.at(i).second);
		capas += "|";
	}
	return capas;
}

void Servidor::procesarMensajes() {
	Mensaje* mensajeCamara;
	string mensajeCamaraString;
	//usleep(5000);
	if (!colaMensajesNoProcesados.empty()) {
		pthread_t threadSalto;
		pthread_mutex_lock(&mutexColaNoProcesados);
		Mensaje mensajeAProcesar = colaMensajesNoProcesados.front();
		colaMensajesNoProcesados.pop();
		pthread_mutex_unlock(&mutexColaNoProcesados);
		if (mensajeAProcesar.getTexto() == "R"){
			string mensajeJugadorPosActualizada = "";
			camara.x = 0;
			camara.y = 0;
			for (int i = 0; i < abscisasCapas.size(); i++)
			{
				abscisasCapas.at(i).first = 0;
				abscisasCapas.at(i).second = 0;
			}
			int xInicial = 20;
			for (int i=0; i < jugadores->size(); i++)
			{
				Jugador* jugador = jugadores->at(i);
				mensajeAProcesar.setRemitente(jugador->getNombre());
				jugador->setPosicion(xInicial);
				xInicial += 84;
				jugador->setSprite("Jugador",true,"Normal");
				mensajeJugadorPosActualizada = jugador->getStringJugador();
				encolarMensajeProcesadoParaCadaCliente(mensajeAProcesar,mensajeJugadorPosActualizada);
			}
			mensajeCamaraString = "1|" + to_string(camara.x) + "|" + to_string(camara.y) + "|" + this->serializarCapas() + "#";
			mensajeCamara = new Mensaje(mensajeAProcesar.getRemitente(),"Todos",mensajeCamaraString);
			encolarMensajeProcesadoParaCadaCliente(*mensajeCamara,mensajeCamaraString);
		}
		else
		{
			pthread_mutex_lock(&mutexVectorJugadores);
			Jugador* jugador = this->obtenerJugador(mensajeAProcesar.getRemitente());
			if (jugador->getConectado()){
				jugador->setMov(mensajeAProcesar.deserializar(mensajeAProcesar.getTexto()), mensajeAProcesar.sePresionoTecla());
			}
			pthread_mutex_unlock(&mutexVectorJugadores);
		}
	}
}

void Servidor::resetearPosiciones()
{
	camara.x = 0;
	camara.y = 0;
	for (int i = 0; i < abscisasCapas.size(); i++)
	{
		abscisasCapas.at(i).first = 0;
		abscisasCapas.at(i).second = 0;
	}
	int xInicial = 20;
	for (int i=0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		jugador->setPosicion(xInicial);
		xInicial += 84;
		jugador->setSprite("Jugador",true,"Normal");
		jugador->revivir();
	}
}


void Servidor::encolarMensajeProcesadoParaCadaCliente(Mensaje mensajeAProcesar, string mensajeJugadorPosActualizada){
	Mensaje* mensajePosicionActualizada;
	for (list<MensajesProcesados>::iterator usuarioActual = listaMensajesProcesados->begin();usuarioActual != listaMensajesProcesados->end();usuarioActual++)
	{
		MensajesProcesados listaMensajes;
		listaMensajes = *usuarioActual;
		if (listaMensajes.destinatario == mensajeAProcesar.getDestinatario() or mensajeAProcesar.getDestinatario() == "Todos")
		{
			pthread_mutex_lock(&mutexVectorJugadores);
			Jugador* jugador = this->obtenerJugador(listaMensajes.destinatario);
			if (jugador->getConectado())
			{
				mensajePosicionActualizada = new Mensaje(mensajeAProcesar.getRemitente(),listaMensajes.destinatario,mensajeJugadorPosActualizada);
				pthread_mutex_lock(&mutexListaProcesados);
				listaMensajes.mensajes->push(*mensajePosicionActualizada);
				pthread_mutex_unlock(&mutexListaProcesados);
				mensajePosicionActualizada->~Mensaje();
			}
			pthread_mutex_unlock(&mutexVectorJugadores);
		}
	}
}

void Servidor::comenzarEscucha() {
//Metodo que pone al servidor a escuchar si alguien requiere algo.
	this->escuchando = (listen(this->welcomeSocket, MAX_CANT_CLIENTES) == 0);
	cout << "Escuchando conexiones entrantes.." << endl;
}

pair<int,string> Servidor::aceptarConexion() {
//esta funcion acepta las conexiones para cada cliente que lo solicita si es autenticado y devuelve el socket de dicho cliente.
	string nombre, pass;
	char buffer[BUFFER_MAX_SIZE];
	char datosRecibidos[BUFFER_MAX_SIZE];
	pair<int,string> cli;

	this->addr_size = sizeof serverStorage;
	int socketCliente = accept(welcomeSocket,(struct sockaddr *) &this->serverStorage, &this->addr_size);
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
	//pthread_mutex_lock(&mutexSocket);
	int ok = recv(socketCliente, datosRecibidos, BUFFER_MAX_SIZE, 0);
	if (ok < 0){
		this->guardarLog("No se pudieron recibir los datos para la conexion.",INFO);
		this->guardarLog("ERROR: Problema con el recv del socket.",DEBUG);
		cli.first = ok;
		cli.second = "";
		return cli;
	}
	cout << "Datos recibidos: " << datosRecibidos << endl;

	splitDatos(datosRecibidos, &nombre, &pass);
	this->autenticar(nombre, pass, usuarios);
	char* resultadoDeLaAutenticacion;
    int okAutenticar = 1;
	if (usuarios.empty()) {
		okAutenticar = -1;
		strcpy(buffer, "Desconectar");
		ok = send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
		cli.first = -1;
		cli.second = "";
		return cli;
	}
	else {
		this->cantClientesConectados += 1;
		strcpy(buffer, this->serializarLista(usuarios).c_str());
		string bufferS = buffer;
		for (list<string>::iterator i = usuarios.begin(); i != usuarios.end();i++) {
			mensaje += (*i);
			mensaje += " ";
		}
		ok = send(socketCliente, buffer, BUFFER_MAX_SIZE, 0);
	}
	//pthread_mutex_unlock(&mutexSocket);
	if (ok < 0){
		this->guardarLog("No se pudieron enviar los datos de la lista a traves de la conexion del cliente " + nombre + ".",INFO);
		this->guardarLog("ERROR: Problema con el send del socket.",DEBUG);
		cli.first = -1;
		cli.second = "";
		return cli;
	}
	//this->cantClientesConectados += 1;
	cli.first = socketCliente;
	cli.second = nombre;
	return cli;
}

void Servidor::finalizarEscucha() {
	escuchando = false;
	close(welcomeSocket);
}

queue<Mensaje> Servidor::getColaMensajesNoProcesados() {
	return this->colaMensajesNoProcesados;
}

void Servidor::splitDatos(char* datos, string* nombre, string* pass) {
	int pos = 0;
	bool nombreCompleto = false;
	bool termino = false;
	while (pos < strlen(datos)) {
		if (datos[pos] != ',' && !nombreCompleto) {
			*nombre = *nombre + datos[pos];

			pos++;
		} else {
			pos++;
			nombreCompleto = true;
			*pass = *pass + datos[pos];
		}
	}
}

void Servidor::setThreadProceso(pthread_t thrProceso) {
	this->threadProcesador = thrProceso;
}

int Servidor::getCantConexiones() {
	return this->cantClientesConectados;
}

string Servidor::serializarLista(list<string> datos) {
	string buffer = "";
	for (list<string>::iterator i = datos.begin(); i != datos.end(); i++) {
		buffer = buffer + *i + ",";
	}
	return buffer;
}

list<string> Servidor::agregarDestinatarios(string remitente) {
	list<string> destinatarios;
	for (list<Servidor::Datos>::iterator datoActual = datosUsuarios->begin();
			datoActual != datosUsuarios->end(); datoActual++) {
		Datos usuario;
		usuario = *datoActual;
		if (usuario.nombre != remitente) {
			destinatarios.push_back(usuario.nombre);
		}
	}
	return destinatarios;
}
string Servidor::traerMensajesProcesados(string nombreCliente) {

	queue<Mensaje>* colaDeMensajes;

	for (list<Servidor::MensajesProcesados>::iterator datoActual = listaMensajesProcesados->begin();
			datoActual != listaMensajesProcesados->end(); datoActual++) {

		MensajesProcesados mensaje;
		mensaje = *datoActual;
		if (mensaje.destinatario == nombreCliente) {
			colaDeMensajes = mensaje.mensajes;
		}
	}

	string mensajesConcatenados = concatenarMensajes(colaDeMensajes);

	return mensajesConcatenados;

}
string Servidor::concatenarMensajes(queue<Mensaje>* colaDeMensajes) {

	Mensaje mensaje;
	string mensajesConcatenados = "";
	//pthread_mutex_lock(&mutexListaProcesados);
	if (colaDeMensajes->empty()) {
		string noHayMensajes = "#noHayMensajes#";
		mensajesConcatenados.append(noHayMensajes);
	}
	while (!colaDeMensajes->empty()) {
		mensaje = colaDeMensajes->front();
		colaDeMensajes->pop();
		mensajesConcatenados += mensaje.getRemitente();
		mensajesConcatenados += "|";
		mensajesConcatenados += mensaje.getTexto();
	}
	//pthread_mutex_unlock(&mutexListaProcesados);
	string lala = mensajesConcatenados.substr(0, mensajesConcatenados.length() -1);
	lala += "@";

	return lala;
}

void Servidor::restarCantidadClientesConectados(){
	this->cantClientesConectados -= 1;
}

vector<Jugador*>* Servidor::getJugadores()
{
	return jugadores;
}

void Servidor::setJugadorConectado(string nombre)
{
	Jugador* jugador = obtenerJugador(nombre);
	jugador->setConectado();
}

void Servidor::setJugadorDesconectado(string nombre)
{
	Jugador* jugador = this->obtenerJugador(nombre);
	jugador->setDesconectado();
}

vector<Jugador*>* Servidor::getJugadoresConectados()
{
	vector<Jugador*>* jugadoresConectados = new vector<Jugador*>();
	for (int i = 0; i < jugadores->size(); i++)
	{
		if (jugadores->at(i)->getConectado())
		{
			jugadoresConectados->push_back(jugadores->at(i));
		}
	}
	return jugadoresConectados;
}

string Servidor::getEstadoInicialSerializado()
{
	string estadoInicial = "";
	vector<Jugador*>* jugadoresConectados = getJugadoresConectados();
	for (int i = 0; i < jugadoresConectados->size(); i++)
	{
		estadoInicial += jugadoresConectados->at(i)->serializarInicio() + "$";
	}
	estadoInicial = estadoInicial + "camara|" + to_string(camara.x) + "|" + to_string(camara.y) + "|" + this->serializarCapas() + "#";
	return estadoInicial;
}

void Servidor::iniciarCamara(){
	camara = {camara.x,camara.y,atoi(handshake->getAncho().c_str()),atoi(handshake->getAlto().c_str())};
}

int Servidor::getCantJugadoresConectadosMax()
{
	return atoi(this->handshake->getCantidadMaximaJugadores().c_str());
}

void Servidor::verificarDesconexion(string nombre)
{
	for (int i = 0; i < jugadores->size(); i++)
	{
		Jugador* jugador = jugadores->at(i);
		if (jugador->getNombre() == nombre)
		{
			jugador->setDesconectado();
			jugador->setSprite("Jugador_desconectado");
			Mensaje* mensaje;
			string mensajeDesconectado = "";
			mensajeDesconectado = jugador->getStringJugador();
			mensaje = new Mensaje(jugador->getNombre(),"Todos",mensajeDesconectado);
			encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeDesconectado);
			while (this->escuchando && !jugador->getConectado())
			{	usleep(100);
				if (jugador->getPosicion().first < camara.x)
				{
					jugador->setPosicion(camara.x);
					mensajeDesconectado = jugador->getStringJugador();
					mensaje = new Mensaje(jugador->getNombre(),"Todos",mensajeDesconectado);
					encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeDesconectado);
				}
			}
			jugador->setSprite("Jugador_" + jugador->getEquipo());
			mensajeDesconectado = jugador->getStringJugador();
			mensaje = new Mensaje(jugador->getNombre(),"Todos",mensajeDesconectado);
			encolarMensajeProcesadoParaCadaCliente(*mensaje,mensajeDesconectado);
		}
	}
}

Escenario* Servidor::getNivelActual()
{
	return (this->vectorNiveles.at(nivelActual));
}

void Servidor::calcularPuntajes()
{
	if (contadorJugadores % this->jugadores->size() == 0)
	{
		string mensajeJugador = "";
		string mensajePuntajesString = "8|" + to_string(modoJuegoElegido.first) + "|";
		for (int i = 0; i < jugadores->size(); i++){
			Jugador* jugador = jugadores->at(i);
			jugador->resetMov();
			jugador->puntajeTotal += jugador->puntaje;
			mensajePuntajesString += jugador->getNombre() + "," + to_string(jugador->puntaje) + "," + to_string(jugador->puntajeTotal);
			if (modoJuegoElegido.first == 3){
				for (int i = 0; i < equipoAlfa.size(); i++){
					if (equipoAlfa.at(i) == jugador->getNombre()){
						mensajePuntajesString += ",alfa";
						break;
					}
				}
				for (int i = 0; i < equipoBeta.size(); i++){
					if (equipoBeta.at(i) == jugador->getNombre()){
						mensajePuntajesString += ",beta";
					}
				}
			}
			if (i < jugadores->size() - 1){
				mensajePuntajesString += ";";
			}
		}
		mensajePuntajesString += "#";
		Mensaje* mensajePuntajes = new Mensaje("Servidor","Todos",mensajePuntajesString);
		encolarMensajeProcesadoParaCadaCliente(*mensajePuntajes,mensajePuntajesString);
		mensajePuntajes->~Mensaje();
		this->getNivelActual()->vaciarVectores();
		this->getNivelActual()->avanzoDeNivel = true;
	}
	contadorJugadores ++;
	usleep(5000000); //tiempo para mostrar la pantalla de puntajes antes de pasar al proximo nivel
}

void Servidor::avanzarDeNivel()
{
	this->nivelActual++;
	if (this->nivelActual > CANTIDADNIVELES)
	{
		this->gameComplete = true;
	}
	if (!this->gameComplete)
	{
		this->resetearPosiciones();
		for (int i = 0; i < jugadores->size(); i++){
			jugadores->at(i)->setearPlataformas(this->getNivelActual()->plataformas);
		}
		string mensajeAvanzarNivel = "9|0|0|" + serializarCapas() + "#";
		Mensaje* mensajeNivel = new Mensaje("Servidor","Todos",mensajeAvanzarNivel);
		encolarMensajeProcesadoParaCadaCliente(*mensajeNivel,mensajeAvanzarNivel);
		mensajeNivel->~Mensaje();
	}
}

bool Servidor::verificarColisionConJugadores(Proyectil* proyectil) {
	pthread_mutex_lock(&mutexVectorJugadores);
	for (int i = 0; i < this->jugadores->size(); i++) {
		if (!this->jugadores->at(i)->getEstaMuerto())
		{
			if (this->getNivelActual()->colisionaronObjetos(proyectil->getBoxCollider(),this->jugadores->at(i)->boxCollider)) {
				if (this->modoJuegoElegido.second == true)
				{
					//si el modo de juego es prueba, verifico colision pero no hago daño.
					pthread_mutex_unlock(&mutexVectorJugadores);
					return true;
				}
				cout << "Se impacto al jugador" << endl;
				this->jugadores->at(i)->daniarseCon(proyectil->getDanio());
				proyectil->jugadorQueRecibioDisparo = this->jugadores->at(i)->getNombre();
				proyectil->vidaDelJugadorImpactado = this->jugadores->at(i)->getVida();
				pthread_mutex_unlock(&mutexVectorJugadores);
				return true;
			}
		}
	}
	pthread_mutex_unlock(&mutexVectorJugadores);
	return false;
}

bool Servidor::verificarColision(SDL_Rect camara, Proyectil* proyectil, bool estaDisparando) {
	if (this->verificarColisionConJugadores(proyectil)) {
		proyectil->colisionPersonaje = true;
		return true;
	} else {
		if (!proyectil->cortoAlcance) {
			return(proyectil->getBoxCollider().x > camara.x + camara.w || (proyectil->getBoxCollider().x + proyectil->getBoxCollider().w) < camara.x || (proyectil->getBoxCollider().y + proyectil->getBoxCollider().h) < camara.y || proyectil->getBoxCollider().y > camara.y + camara.h);
		} else {
			return (!estaDisparando);
		}
	}
}
