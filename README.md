# Trabajo-Threads-y-Semaforos
### Hell's Burgers
#### Introduccion 


_El lenguaje de programación C fue el principal obstáculo que se me presento para realizar el trabajo debido a que lo desconocía. Por esto mismo, este trabajo requirió una gran búsqueda de información, sumado a la ayuda de los profesores. Si bien al realizar el pseudocódigo, se comprendían las instrucciones, las trabas que debían tener los equipos al realizar las preparaciones  por el uso compartido del equipamiento, el código en c resultó muy dificultoso. Cada vez que realizaba una parte del código, una traba se me presentaba, luego de la presentación del template de la profesora Noelia, pude reacomodar las ideas que tenía e implementarlo correctamente_

#### Implementación, dificultades y resoluciones propuestas

__Con respecto a la lectura del archivo:__
En un principio, comencé utilizando la función getLine() para leer los archivos. Sin embargo, luego de lo explicado en clase sobre la lectura y el uso de delimitadores, retome la búsqueda de información, y considere que me convenía utilizar la función fgets() junto con el strtok . Además, no comprendía como leer línea por línea el archivo, luego la profesora me asesoro sobre el uso del for, al probarlo, me recorría correctamente el archivo al poner la cantidad de líneas que poseía.  Para implementar el strtok, tuve que mirar videos y sitios web para comprender su funcionamiento. Con lo explicado hasta ahora, la solución que surgió fue recorrer con un for el archivo.txt , leer línea por línea el mismo, y a la vez, con la función strtok, siguiendo la lógica de que el primer string de la línea era una acción y el resto los ingredientes, separe la acción de los ingredientes con “:”(Primer delimitador) y el resto de los ingredientes con “,”(Segundo delimitador).A la vez, en cada línea, luego de separar el primer string (la acción), la guardaba en la estructura pasos.accion, y al resto, los ingredientes, con otro for, los fui guardando en el “array” ingredientes de la estructura p
