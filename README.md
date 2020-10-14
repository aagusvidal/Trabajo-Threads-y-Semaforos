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

No tuve conflictos para  la inicializacion  de los semáforos, ya que lo implemente  luego de ver los ejemplos.   Para cada hilo implemente 7 semáforos  individuales,  uno por cada acción (Sin contar a la función cortar) , y también  3 semáforos  globales,  simulando  la  función  de los mutex, para las acciones que requiere compartir equipamiento . 
En las funciones en las que hay secciones críticas ,  es decir  , aquellos elementos que no pueden  ser usados al mismo tiempo (salero,  cocina, horno) , con el wait y el   post  , bloqueo y desbloqueo aquellos elementos (A través de sus semáforos ),   para que no   se puedan realizar al unisono por los equipos . Por lo que los otros equipos se quedan esperando su liberación.

__Funciones:__

También,  en la funcion ejecutarreceta() , cada hilo crea 8 hilos nuevos, que se van a  dirigir a cada paso de la receta ( Cada funcion representa un paso) .  


