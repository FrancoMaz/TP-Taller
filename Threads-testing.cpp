#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

using namespace std;

//-------------------------PUNTO 1--------------------------------

/* Si nosotros queremos crear un thread que inicie una función, debemos
 crear dicha función que sea si o si de tipo void* (puntero void) y tenga
 al menos un argumento que sea de tipo void*.
 */

//Por ejemplo, creamos la siguiente función

/*
void* imprimirTexto (void*){
	for(int i=0; i < 30000; i++){
		cout << "Hola " << i << endl;
	}
	return NULL;
}
*/
/*
void* multiplicarPorDos (void* arg){
	int res = *((int*)arg) * 2;
	//string saludo = "Hola " + *((string*) arg);
	//pthread_exit((void*) res);
	return (void*) res;
}

int main (){
	pthread_t thread_id;

	int valor = 9;
	int resultado;

	pthread_create(&thread_id, NULL, &multiplicarPorDos, &valor);
	pthread_join (thread_id, (void**) &resultado);

	cout << "Resulted: "<< resultado << endl;

	return 0;
}

*/


//-------------------------PUNTO 2--------------------------------

/*
struct imprimirCaracterVars{
	char caracter;
	int contador;
};

void* imprimirCaracter(void* parametros){
	struct imprimirCaracterVars *p = (struct imprimirCaracterVars*) parametros;

	for(int i=0; i < p->contador; ++i){
		cout << p->caracter << endl;
	}
	return NULL;
}

int main(){
	pthread_t thread1_id;
	pthread_t thread2_id;
	struct imprimirCaracterVars thread1_args;
	struct imprimirCaracterVars thread2_args;
	thread1_args.caracter = 'x';
	thread1_args.contador = 300;
	pthread_create(&thread1_id, NULL, &imprimirCaracter, &thread1_args);
	thread2_args.caracter = 'o';
	thread2_args.contador = 300;
	pthread_create(&thread2_id, NULL, &imprimirCaracter, &thread2_args);
	pthread_join(thread1_id, NULL);
	pthread_join(thread2_id, NULL);
	return 0;
}
*/


//-------------------------PUNTO 3--------------------------------

/*
void* thread_function (void* thread_arg){
	cout << "ok" << endl;
	return NULL;
}

int main (){
	pthread_attr_t attr;		//en attr se guardan los atributos para thread
	pthread_t thread;
	pthread_attr_init(&attr);	//iniciamos el objeto attr
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	//El thread se libera automáticamente al finalizarse
	pthread_create(&thread, &attr, &thread_function, NULL);
	pthread_attr_destroy(&attr);	//para liberar la variable y pueda ser reutilizada

	sleep(5);
	cout << "ok2" << endl;
	return 0;
}

*/


//-------------------------PUNTO 4--------------------------------

pthread_mutex_t mutex;

void* llamarPorTelefono(void* arg){
	int numPersona = *(int*)arg;
	pthread_mutex_lock(&mutex);
	cout << "Persona " << numPersona << ": Hola que ase" << endl;
	cout << "Persona " << numPersona << ": bla bla bla" << endl;
	cout << "Persona " << numPersona << ": bla bla" << endl;
	cout << "Persona " << numPersona << ": nos vimos" << endl;
	numPersona++;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(){
	pthread_t persona1;
	int numPer1 = 1;
	pthread_t persona2;
	int numPer2 = 2;
	pthread_t persona3;
	int numPer3 = 3;
	pthread_create(&persona1, NULL, &llamarPorTelefono, &numPer1);
	pthread_create(&persona2, NULL, &llamarPorTelefono, &numPer2);
	pthread_create(&persona3, NULL, &llamarPorTelefono, &numPer3);

	pthread_join(persona1, NULL);
	pthread_join(persona2, NULL);
	pthread_join(persona3, NULL);
	return 0;
}
