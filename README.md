# Trabajo-Threads-y-Semaforos
### Hell's Burgers
#### Introduccion 


_El lenguaje de programación C fue el principal obstáculo que se me presento para realizar el trabajo debido a que lo desconocía. Por esto mismo, este trabajo requirió una gran búsqueda de información, sumado a la ayuda de los profesores. Si bien al realizar el pseudocódigo, se comprendían las instrucciones, las trabas que debían tener los equipos al realizar las preparaciones  por el uso compartido del equipamiento, el código en c resultó muy dificultoso. Cada vez que realizaba una parte del código, una traba se me presentaba, luego de la presentación del template de la profesora Noelia, pude reacomodar las ideas que tenía e implementarlo correctamente_

#### Implementación, dificultades y resoluciones propuestas

__Con respecto a la lectura del archivo:__

En un  principio, comence utilizando la función getLine() para leer archivos. Sin embargo, luego de lo explicado en clase sobre la lectura y el uso de delimitadores, retome la busqueda de informacion, y utilice  fgets() en su lugar, junto con el strtok. 
Además, no comprendía como leer linea por linea el archivo, luego la profesora me asesoro sobre el uso del for, al probarlo, me recorria correctamente el archivo al poner la cantidad de lineas que poseia. Para implementar  el strtok tuve que mirar varios videos y sitios web para comprender su funcionamiento.
Con lo explicado hasta ahora, la solución que surgió fue recorrer con un for el archivo.txt , leer linea por linea el mismo,  a la vez,  con el strtok, siguiendo  la logica de que el primer string de la linea   era una accion,   y el resto los ingredientes, separe la accion de los ingredientes con  ":" y   el resto  con ",".
A la vez,  en cada linea,  luego  de separar  el primer string  (la accion)  , la    guardaba   en la estructura  pasos.accion  , y al resto ,  los ingredientes,  con otro for,  los fui guardando  en el array   de ingredientes de la   estructura paso.   Al guardar estos   valores tuve un   inconveniente: Como maximo del segundo for,    utilice el numero cuatro, ( Ya que es la cantidad de ingredientes que se  utilizaban como maximo en alguna de las lineas)         , sin embargo,  todos los pasos no tenian la misma cantidad de ingredientes ,    y cuando almacenaba en el array menos de 4 ingredientes de una linea,     me almacenaba   un NULL   en su lugar, por lo que tuve que poner la condicion if(sub!=NULL) para almacenar recien ahi los ingredientes. 


__Con respecto a  los semaforos:__

No tuve conflictos para  la inicializacion  de los semáforos, ya que lo implemente  luego de ver los ejemplos.   Para cada hilo implemente 8 semaforos individuales,  y 3 semaforos globales, para simular  la accion de los mutex, para las acciones que requieren compartir equipamiento.
En las funciones en las que hay secciones críticas ,  es decir  , aquellos elementos que no pueden  ser usados al mismo tiempo (salero,  cocina, horno) , con el wait y el   post  , bloqueo y desbloqueo aquellos elementos (A través de sus semáforos ),   para que no   se puedan realizar al unisono por los equipos . Por lo que los otros equipos se quedan esperando su liberación.

__Funciones:__

También,  en la funcion ejecutarreceta() , cada hilo crea 8 hilos nuevos, que se van a  dirigir a cada paso de la receta ( Cada funcion representa un paso) .Para las demás funciones  seguí  la estructura brindada por Noelia .  En  la  funcion imprimir tuve que realizar  modificaciones . Dentro  de la función  imprimirAccion()  me surgió un problema a la hora de guardar  las cadenas en el archivo , ocurría una violación de segmento, debido a que utilizaba una sola cadena para almacenar todo el contenido.   
Después de buscar información sobre cuál podría ser el problema, lo arregle, colocando una cadena  para cada for  y  una para  separar  datos  (line1,line2,line3). En esta función, con la funcion sprintf() coloco el string del numero del equipo en el char equipo. Luego, con strcat concateno varios strings a la cadena y con sputs escribo todo el contenido de la cadena en el archivo de la competencia.  Para los ingredientes, me surgió un problema. Al revisar el archivo de salida , en cada pasada me imprimia  cada ingrediente, pero iba repitiendo lo que ya había impreso anteriormente .  
Esto pude solucionarlo limpiando en cada pasada del for con  strcpy(line3,"") la cadena.
Otro cambio sobre las funciones fue en  armarHamburguesa() . Como agregado a esta funcion tuve que crear una variable compartida  de tipo int para saber cuando un equipo gano, debido a que quise hacer una funcion llamada ganador() y un semaforo ganador , en la que sólo entraría el ganador, pero  al entrar y bloquear el semaforo, el resto  de los hilos se quedaba esperando a que se habilite el semaforo  y el programa se quedaba corriendo . Esta variable esta inicializada en cero, en donde declaro los semaforos globales, y cada vez que un equipo entra a la funcion armarHamburguesa(), se incrementa en uno. 
Dentro de esta función , comparo si esa variable ya incrementada es igual a 1, si esto sucede, se abre el archivo que documenta  la competencia  y escribe en él "¡¡¡¡ATENCION A TODOSSSS!!!! El equipo x termino su hamburguesa,¡Es el ganador!".  Si la variable es igual a dos, escribe "El equipo x termino su hamburguesa, ¡Salio segundo!", y similar con el equipo que sale tercero. Luego cierra el archivo para guardar los cambios .

__Salida de la competencia:__

Un problema surgido en la salida de la competencia lo note al ejecutar varias veces el programa , en la primera ejecución creaba el archivo que le solicitaba y en él se encontraba una salida normal, tal como se esperaba.  Luego, al ejecutarlo consecutivamente, sin borrar el txt de salida, observe que la ejecución anterior  seguía escrita dentro del archivo, y sin sobrescribirse se anidaba  la segunda ejecución. Intente con FILE* competencia=fopen("new.txt","w+"); que me sobrescribia el archivo, pero en cada llamada a la funcion imprimir se sobrescribia, por lo que solo me escribia la funcion armarHamburguesa() y sus ingredientes, por lo que lo descarte.  Como segunda opcion posible , investigué y encontré una función que removía un archivo, por lo que en ejecutarreceta() antes de leer el archivo de la receta, con remove("new.txt").Esto funciono, pero luego lo modifique, colocandolo en el main, al encontrarme con un problema nuevo: Seguido de la eliminacion del archivo, mi idea fue agregar al comiendo del archivo de salida, que habia comenzado la competencia, esto de igual forma lo implemente en ejecutarreceta(), pero luego de varias ejecuciones, en algunas pasadas, los equipos entraban antes a ejecutar la receta, por lo que primero me imprimía una acción de un equipo y luego que era el comienzo de la competencia. Intente cambiando de lugar el archivo, antes de leer la receta, pero de igual forma me sucedió lo mismo en varias pasadas.
Finalmente, como solución final ,copie esas lineas en el main, tanto la de borrar el archivo por si ya se ejecuto, y la de inicio de competencia. En esta última,  concateno con strcat una cadena que indica que comienza la competencia, y luego con fputs lo escribe sobre el archivo al comienzo.

__Pseudocódigo__
Leyendo el ejercicio del trabajo, sabemos que las acciones a realizar son: cortar,mezclar,salar,armar medallones,  cocinar las hamburguesas,  hornear los panes, y cortar los extras. A raiz de esto, se crea una funcion para cada accion, y imprimirAcciones para ir subiendo en el archivo las acciones. 
Como cortar extras y hornear pan son acciones que no dependen de otras funciones, las deje al azar, para que entren los equipos cuando tengan tiempo, y ambas mandan una señal a  armar hamburguesa, junto con la funcion cocinar hamburguesas, ya que una vez que esten cocinadas las hamburguesas, horneado el pan y  cortados los extras,se pueda preparar la hamburguesa.


! [](/Pseudocodigo.png)

