#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>
#define LIMITE 50

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Defino los semaforos que van a compartir los hilos y la variable entera compartida que me ayudara a saber si algun equipo gano//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sem_t saleroUnico;
sem_t planchaUnica;
sem_t hornoUnico;
int rankingganador;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Creo la estructura de los semaforos y dentro de ella coloco todos los semaforos que utilizaran los hilos//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct semaforos {
	    	sem_t sem_mezclar;
		sem_t sem_salarMezcla;
		sem_t sem_armarMedallones;
		sem_t sem_cocinarHamburguesa;
		sem_t sem_hornearPan;
		sem_t sem_cortarExtras;
		sem_t sem_armarHamburguesa;

};

////////////////////////////////////////////////////////////////////////////////////////////////
//Creo una estructura paso que posee una accion de la receta y un "array" con los ingredientes//
////////////////////////////////////////////////////////////////////////////////////////////////
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
};

/////////////////////////////////////
//Creo los parametros de los hilos //
/////////////////////////////////////
struct parametro {
 	int equipo_param;
  	struct semaforos semaforos_param;
 	struct paso pasos_param[8];
};

///////////////////////////////////////////////////
//Imprimira cada accion realizada por los equipos//
///////////////////////////////////////////////////
void* imprimirAccion(void *data, char *accionIn) {
	FILE* competencia=fopen("new.txt","a");
	char equipo[3];
	char line1[100];
	char line2[100];
	char line3[100];
	struct parametro *mydata = data;
	//Calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//Indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//Pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)		
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
			//Guardo en equipo el string con el numero del equipo ejecutando
			sprintf(equipo,"%d",mydata->equipo_param);
			//Guardo en line1 distintas cadenas
			strcat(line1,"\nAccion del equipo ");
			strcat(line1,equipo);
			strcat(line1,":");
			strcat(line1,mydata->pasos_param[i].accion);
			strcat(line1,"\n");
			//Subo al archivo competencia los strings que se encuentran en line1
			fputs(line1,competencia);

		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		//Guardo en line2 distintas cadenas
		strcat(line2,"Lista de ingredientes donde realiza la accion el equipo ");
		strcat(line2,equipo);
		strcat(line2,":\n");
        	//Subo al archivo competencia los strings que se encuentran en line2
		fputs(line2,competencia);
		for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							//Concateno en line3 un vacio, para limpiar la variable
							strcpy(line3,"");
							//Guardo en line3 distintas cadenas
							strcat(line3,">");
							strcat(line3,mydata->pasos_param[i].ingredientes[h]);
							strcat(line3,"\n");
							//Subo al archivo competencia los strings que se encuentran en line3
							fputs(line3,competencia);
				}
			}
		}
	}
	fclose(competencia);
}

///////////////////////////////////////////
//Cortara el ajo, la cebolla y el perejil//
///////////////////////////////////////////
void* cortar(void *data) {
        //Creo el nombre de la accion de la funcion
        char *accion = "cortar";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
        //Doy la señal a la siguiente accion (cortar me habilita mezclar)
        sem_post(&mydata->semaforos_param.sem_mezclar);
		pthread_exit(NULL);
}

/////////////////////////////////////////////////
//Mezclara los ingredientes con la carne picada//
/////////////////////////////////////////////////
void* mezclar(void *data) {
        //Creo el nombre de la accion de la funcion
        char *accion = "mezclar";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_mezclar);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
        //Doy la señal a la siguiente accion (mezclar me habilita a salar la mezcla)
		sem_post(&mydata->semaforos_param.sem_salarMezcla);
		pthread_exit(NULL);
}

////////////////////////////////////////
//Salara la mezcla de las hamburguesas//
////////////////////////////////////////
void* salar(void *data) {
		//Bloqueo el semaforo salero unico para que otro equipo no pueda usarlo
		sem_wait(&saleroUnico);
        //Creo el nombre de la accion de la funcion
        char *accion = "salar";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_salarMezcla);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
        //Desbloqueo el semaforo salero unico para que otro equipo pueda usarlo
        sem_post(&saleroUnico);
		//Doy la señal a la siguiente accion (salar me habilita a armar los medallones)
		sem_post(&mydata->semaforos_param.sem_armarMedallones);
		pthread_exit(NULL);
}

///////////////////////////////////////////
//Armara los medallones de la hamburguesa//
///////////////////////////////////////////
void* armarMedallones(void *data) {
		//Creo el nombre de la accion de la funcion
        char *accion = "armar";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_armarMedallones);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
        //Doy la señal a la siguiente accion (armar medallones me habilita que pueda cocinar las hamburguesas)
		sem_post(&mydata->semaforos_param.sem_cocinarHamburguesa);
    	pthread_exit(NULL);
}

////////////////////////////////////////////
//Cocinara las hamburguesas de cada equipo//
////////////////////////////////////////////
void* cocinarHamburguesa(void *data) {
		//Bloqueo el semaforo plancha unica para que otro equipo no pueda usarla
		sem_wait(&planchaUnica);
        //Creo el nombre de la accion de la funcion
        char *accion = "cocinar";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_cocinarHamburguesa);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
		//Desbloqueo el semaforo plancha unica para que otro equipo pueda usarla
		sem_post(&planchaUnica);
		//Doy la señal a la siguiente accion (cocinar me habilita a hornear el pan)
		sem_post(&mydata->semaforos_param.sem_hornearPan);
    	pthread_exit(NULL);
}

//////////////////////////////
//Horneara el pan del equipo//
//////////////////////////////
void* hornearPan(void *data) {
		//Bloqueo el semaforo horno unico para que otro equipo no pueda usarlo
		sem_wait(&hornoUnico);
        //Creo el nombre de la accion de la funcion
        char *accion = "hornear";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_hornearPan);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
		//Desbloqueo el semaforo horno unico para que otro equipo pueda usarlo
		sem_post(&hornoUnico);
		//Doy la señal a la siguiente accion (hornear me habilita a cortar la lechuga y el tomate)
        sem_post(&mydata->semaforos_param.sem_cortarExtras);
    	pthread_exit(NULL);
}

//////////////////////////////////////////////////////
//Cortara la lechuga y el tomate para la hamburguesa//
//////////////////////////////////////////////////////
void* cortarExtras(void *data) {
        //Creo el nombre de la accion de la funcion
        char *accion = "cortarExtras";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_cortarExtras);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
        //Doy la senal a la siguiente accion (cortarextras me habilita a armar la hamburguesa completa)
	sem_post(&mydata->semaforos_param.sem_armarHamburguesa);
    	pthread_exit(NULL);
}

////////////////////////////////////////////////////
//Armara la hamburguesa con todos los ingredientes//
////////////////////////////////////////////////////
void* armarHamburguesa(void *data) {
		//Creo el nombre de la accion de la funcion
        char *accion = "armarHamburguesa";
        //Creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
		sem_wait(&mydata->semaforos_param.sem_armarHamburguesa);
        //Llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //Uso sleep para simular que que pasa tiempo
		rankingganador++;

		//Me subira al archivo cual es el equipo ganador
		if(rankingganador==1){
			FILE* competencia=fopen("new.txt","a");
			char lineaganador[100];
			char equipo[3];
			sprintf(equipo,"%d",mydata->equipo_param);
			strcat(lineaganador,"\n¡¡¡¡ATENCION A TODOSSSS!!!!");
			strcat(lineaganador,"\nEl equipo ");
			strcat(lineaganador,equipo);
			strcat(lineaganador," termino su hamburguesa¡Es el ganador!\n");
			fputs(lineaganador,competencia);
			fclose(competencia);
		}
		//Me subira al archivo cual es el equipo que salio segundo
		else if(rankingganador==2) {
			FILE* competencia=fopen("new.txt","a");
			char segundopuesto[100];
			char equipo[3];
			sprintf(equipo,"%d",mydata->equipo_param);
			strcat(segundopuesto,"\nEl equipo ");
			strcat(segundopuesto,equipo);
			strcat(segundopuesto," termino su hamburguesa,¡Salio segundo!\n");
			fputs(segundopuesto,competencia);
			fclose(competencia);
			}
		//Me subira al archivo cual es el equipo que salio tercero
		else{
			FILE* competencia=fopen("new.txt","a");
			char tercerpuesto[100];
			char equipo[3];
			sprintf(equipo,"%d",mydata->equipo_param);
			strcat(tercerpuesto,"\nEl equipo ");
			strcat(tercerpuesto,equipo);
			strcat(tercerpuesto," termino su hamburguesa,¡Salio tercero!\n");
			fputs(tercerpuesto,competencia);
			fclose(competencia);
		}
		usleep( 2000000 );
	pthread_exit(NULL);
}

//////////////////////////////////////////////////////////
//Leera el archivo que contiene la receta y la ejecutara//
//////////////////////////////////////////////////////////
void* ejecutarReceta(void *i){
	//Defino los semaforos
	sem_t sem_mezclar;
	sem_t sem_cortarExtras;
	sem_t sem_hornearPan;
	sem_t sem_armarMedallones;
	sem_t sem_salarMezcla;
	sem_t sem_cocinarHamburguesa;
	sem_t sem_armarHamburguesa;

	//Identifico con un puntero los hilos que apuntaran a cada funcion
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;

	int p = *((int *) i);


	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	pthread_data->equipo_param = p;

	//Abro el archivo que contiene la receta
	FILE* receta=fopen("listado.txt","r");
	//Defino los delimitadores que separaran las palabras
	char delimitador[]=",:";
	//Cantidad de instrucciones de la receta
	int largo=8;
	//Guardara cada substring de la receta
	char *sub;
	char line[100];
	//Recorro la cantidad de lineas del archivo hasta el final
	for (int i=0;i<largo;i++){
		//Leo la linea
		fgets(line,100,receta);
		//Con el strtok leo el string y si encuentro un delimitador se queda con el string anterior
		sub=strtok(line,delimitador);
		//Como el primer string siempre es la accion, guardo en sub la accion de cada linea, y utilizo la estructura pasos en donde la voy a almacenar 
		strcpy(pthread_data->pasos_param[i].accion,sub);
		while(sub!=NULL){
				//Recorro el resto de la linea, sacando la accion,como maximo hay cuatro ingredientes, utilizo el cuatro
				for(int j=0;j<4;j++){
				sub=strtok(NULL,delimitador);
				//Como algunas lineas tienen menos de cuatro ingredientes utilizo esto para que no me guarde valores null
				if(sub!=NULL){
					//Almaceno los ingredientes de cada fila en el string de ingredientes
					strcpy(pthread_data->pasos_param[i].ingredientes[j],sub);
				}
			}
		}
	}
	//Cierro el archivo receta
	fclose(receta);

	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salarMezcla = sem_salarMezcla;
	pthread_data->semaforos_param.sem_armarMedallones = sem_armarMedallones;
	pthread_data->semaforos_param.sem_cocinarHamburguesa = sem_cocinarHamburguesa;
	pthread_data->semaforos_param.sem_cortarExtras = sem_cortarExtras;
	pthread_data->semaforos_param.sem_hornearPan= sem_hornearPan;
	pthread_data->semaforos_param.sem_armarHamburguesa = sem_armarHamburguesa;


	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salarMezcla),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarMedallones),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinarHamburguesa),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cortarExtras),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_hornearPan),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarHamburguesa),0,0);

	int rc;

	//Creo los hilos que se dirigiran a cada paso de la receta
	rc = pthread_create(&p1,NULL,cortar,pthread_data);
	rc = pthread_create(&p2,NULL,mezclar,pthread_data);
	rc = pthread_create(&p3,NULL,salar,pthread_data);
	rc = pthread_create(&p4,NULL,armarMedallones,pthread_data);
	rc = pthread_create(&p5,NULL,cocinarHamburguesa,pthread_data);
	rc = pthread_create(&p6,NULL,hornearPan,pthread_data);
	rc = pthread_create(&p7,NULL,cortarExtras,pthread_data);
	rc = pthread_create(&p8,NULL,armarHamburguesa,pthread_data);



	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);

	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salarMezcla);
	sem_destroy(&sem_armarMedallones);
	sem_destroy(&sem_cocinarHamburguesa);
	sem_destroy(&sem_cortarExtras);
	sem_destroy(&sem_hornearPan);
	sem_destroy(&sem_armarHamburguesa);

	pthread_exit(NULL);
}

int main ()
{
	//Abro el archivo que contendra la sucedido en la competencia y lo elimino, para borrar rastros en caso de que el archivo contenga algun dato.
	remove("new.txt");

	//Abro el archivo de la competencia y cargo que empieza la competencia 
	FILE* newcompetencia=fopen("new.txt","a");
	char line[100];
	strcat(line,"\nEmpieza la competencia Hell's Burgers...");
	strcat(line,"\n");
	fputs(line,newcompetencia);
	fclose(newcompetencia);


	//Inicializo los semaforos compartidos
	sem_init(&saleroUnico,0,1);
	sem_init(&planchaUnica,0,1);
	sem_init(&hornoUnico,0,1);

    	int rc;

    	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));

    	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;


    	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;


	rc = pthread_create(&equipo1,NULL,ejecutarReceta,equipoNombre1);
	rc = pthread_create(&equipo2,NULL,ejecutarReceta,equipoNombre2);
	rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);

    	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);

    	pthread_exit(NULL);
	//Destruyo los semaforos
	sem_destroy(&saleroUnico);
	sem_destroy(&planchaUnica);
	sem_destroy(&hornoUnico);
}

