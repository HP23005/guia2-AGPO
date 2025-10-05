# guia2-AGPO
Ejercicio 1: Procesos huérfanos y zombies 
Escribe un programa en C que cree un proceso hijo que termine inmediatamente mientras el padre hace sleep(30). Observa con ps aux y explica el estado del hijo (zombie). Luego mata al padre y verifica qué sucede con el hijo (huérfano). 
NOTA: para la verificación en terminal es muy conveniente que se auxilie de grep para filtrar los resultados que contienen la palabra zombie recuerde el uso del comando “pipe” → | para unir salidas con entradas en la terminal.

Analisis ejercicio 1
Antes de matar al proceso padre
Si el padre no hace wait(), el hijo finaliza y permanece en la tabla de procesos como zombie (Z o defunct) hasta que el padre lo recoja.
Estado Z significa que el hijo terminó, pero su padre no ha recogido su estado de salida con wait().

Despues de matar al proceso padre
El proceso hijo zombie es adoptado por el proceso init o systemd (PID 1).
El nuevo PPID (ID del padre) del hijo zombie será 1
Luego el proceso init automáticamente realiza el wait() por él, y el zombie desaparece.

La finalidad del código es demostrar cómo los procesos hijos pueden pasar por diferentes estados en un sistema operativo Unix/Linux, específicamente el estado zombie y el estado huérfano. Primero, el hijo se convierte en un zombie cuando termina su ejecución, pero el padre no llama a wait() para recolectar su estado. Mientras el padre está dormido, el hijo permanece como zombie. Cuando el padre se mata a sí mismo, el hijo se convierte en huérfano y es adoptado por el proceso init (PID 1), que se encarga de recolectar el estado del hijo y eliminarlo de la tabla de procesos. Este ciclo ilustra cómo el sistema maneja la terminación y limpieza de procesos, evitando que se acumulen procesos zombies.

Codigo consola ejercicio 1
cd /home/hp23005/guia2GPO
gcc -o huerfano_zombie ejercicio1-procesohijo.c
./huerfano_zombie
ps aux | grep huerfano_zombie

ps aux | grep pregunta1

cd /home/hp23005/guia2GPO
gcc -o proceso_padre ejercicio1-matarprocesopadre.c
./proceso_padre
ps aux | grep proceso_padre


Ejercicio 2: Creación masiva de procesos 
Ejecuta un script que cree 50 procesos en background en la terminal: for i in {1..50}; do sleep 200 & done. Usa ps -o pid,ppid,stat,cmd --forest para observar la jerarquía. Explica con tus palabras cómo el kernel gestiona múltiples procesos concurrentes.

En la imagen que compartiste, podemos ver cómo el kernel gestiona múltiples procesos concurrentes. Los 50 procesos sleep 200 están en ejecución, cada uno con su propio PID (identificador de proceso). El kernel, a través del planificador de procesos, distribuye el tiempo de CPU entre estos procesos y asegura que cada uno pueda ejecutarse sin interferir con los demás. Aunque los procesos están "durmiendo" (esperando), el kernel sigue manteniendo el control sobre ellos, gestionando sus estados y recursos. La jerarquía de procesos muestra que todos los procesos fueron creados por el proceso padre (bash) y gestionados como hijos, con cada uno teniendo su propio PID y un estado de "dormido" (sleep 200).

Ejercicio 3: Comunicación entre procesos 
Usa un pipe para conectar tres procesos: En la terminal ejecuta ps aux | grep bash | sort - k 2 -n | tee procesos.txt. Explica cómo se transmiten los datos entre procesos en el pipeline. Analiza la eficiencia frente a escribir/leer en disco. 
NOTA: previo a la ejecución del comando muévete a un directorio adecuado puesto que se creará un archivo llamado procesos.txt en el directorio en el que la terminal se encuentre en ese momento. Puedes visualizar el archivo usando el comando cat.

cd /home/hp23005/guia2GPO
ps aux | grep bash | sort -k 2 -n | tee procesos.txt

Transmisión de datos entre procesos en el pipeline
En un pipeline de Linux, los procesos están conectados entre sí por medio de pipes, lo que significa que los datos fluyen de un proceso a otro sin necesidad de almacenarse en archivos intermedios. Aquí está cómo se transmite la información entre los procesos:
Pipe (|): Los pipes son mecanismos en memoria que permiten que los datos se transmitan directamente de un proceso a otro. No es necesario escribir los datos a disco; se mantienen en memoria. El proceso ps aux escribe en el pipe, el cual es leído inmediatamente por grep, y luego por sort, y finalmente por tee.
Los datos no se escriben en disco hasta que el comando tee guarda la salida en el archivo procesos.txt, lo que ocurre solo al final de la cadena de procesos.

Eficiencia de los pipes frente a la escritura/lectura en disco
Uso de pipes: Los pipes en memoria son mucho más rápidos que las operaciones de escritura y lectura en disco. Los datos no tienen que ser físicamente almacenados en el disco, sino que simplemente se transmiten a través de la memoria RAM. Este enfoque es altamente eficiente, especialmente cuando se está manejando un flujo continuo de datos entre procesos.
Escritura/lectura en disco: Si los datos tuvieran que ser escritos en un archivo en disco en cada paso, habría una latencia significativa debido al acceso al sistema de archivos. La escritura en disco involucra varias operaciones I/O, lo que puede ser mucho más lento que el acceso directo a la memoria, especialmente si los archivos son grandes o si se realizan muchas operaciones de lectura y escritura.

Ejercicio 4: Creación de procesos con fork() 
Escribe un programa que cree un proceso hijo utilizando fork(). El proceso hijo debe imprimir su PID y el PPID (Parent Process ID). El proceso padre debe imprimir su propio PID y el PID del hijo. Además, el padre debe esperar a que el hijo termine usando wait().
En base a la realización del ejercicio responda con sus propias palabras:
•	¿Qué utilidad tiene la función fork() en este caso?
•	¿Cómo se puede aprovechar el uso de fork() en un software real?

La función fork() en este caso se utiliza para crear un proceso hijo a partir del proceso padre. Permite la duplicación del proceso en dos procesos independientes que se ejecutan de forma concurrente. Esto es útil para crear tareas paralelas en un programa.

En software real, fork() se puede usar para:

Concurrencia: Crear múltiples procesos que pueden ejecutarse en paralelo, como en servidores web donde cada solicitud de cliente es manejada por un proceso hijo creado mediante fork().

Procesamiento en segundo plano: Para realizar tareas en segundo plano sin bloquear el proceso principal. Por ejemplo, un programa que debe realizar un cálculo largo puede usar fork() para crear un proceso hijo que lo realice mientras el proceso principal sigue respondiendo a otras tareas.

Sistemas multitarea: En sistemas que requieren de un alto nivel de multitarea, como en servidores de bases de datos, donde cada consulta o tarea puede ser manejada por un proceso hijo.

Simulación de procesos independientes: fork() puede utilizarse para simular procesos separados que ejecutan tareas específicas de forma autónoma, lo que es común en sistemas distribuidos o en la creación de aplicaciones con arquitectura de microservicios.

cd /home/hp23005/guia2GPO
gcc -o ejercicio4 ejercicio4.c
./ejercicio4

Ejercicio 5: Creación de hilos con pthread_create() 
Escribe un programa en C que cree dos hilos utilizando la API de POSIX (pthreads). Cada hilo debe imprimir un mensaje identificativo (por ejemplo, "Hilo 1" y "Hilo 2"). El programa principal (hilo principal) debe esperar a que ambos hilos terminen su ejecución utilizando pthread_join(). Al final, el hilo principal debe imprimir un mensaje indicando que todos los hilos han terminado.
En base a la realización del ejercicio responda con sus propias palabras: 
•	¿Qué utilidad tienen los hilos en este caso? 
•	¿Cómo se puede aprovechar el uso de hilos en un software real?


cd /home/hp23005/guia2GPO
gcc -o ejercicio5 ejercicio5.c
./ejercicio5

En este caso, los hilos permiten ejecutar múltiples tareas de manera concurrente dentro de un único programa. Al utilizar dos hilos, podemos realizar operaciones en paralelo, lo cual es más eficiente que si esas operaciones se realizaran secuencialmente en un solo hilo.

Los hilos se utilizan ampliamente en el desarrollo de software real por las siguientes razones:

Paralelismo: Los hilos permiten realizar varias tareas en paralelo, lo que mejora el rendimiento, especialmente en sistemas con múltiples núcleos de CPU. Por ejemplo, un servidor web puede manejar múltiples solicitudes de clientes simultáneamente, cada una en su propio hilo, mejorando la eficiencia y tiempos de respuesta.

Procesamiento concurrente: Los hilos pueden ejecutarse de forma concurrente, lo que permite realizar tareas como la lectura y escritura de archivos, procesamiento de datos, o interacciones con la red, todo al mismo tiempo sin bloquearse mutuamente. Esto es útil en aplicaciones como servidores de bases de datos, aplicaciones de procesamiento de imágenes, y software de redes.

Interactividad en aplicaciones: En aplicaciones de usuario, los hilos permiten que una interfaz gráfica de usuario (GUI) siga siendo interactiva mientras se realizan operaciones de fondo (como cargar datos o realizar cálculos largos), evitando que la interfaz se congele o bloquee.

Mejor utilización de recursos: El uso de hilos puede hacer un mejor uso de los recursos del sistema, como los múltiples núcleos del procesador, haciendo que un programa se ejecute más rápidamente y sea más eficiente en su uso de CPU.


#include <stdio.h>
#include <unistd.h>

int main() {
    printf("1. Este es el programa original.\n");

    execlp("ls", "ls", "-l", NULL);
    
    
    printf("2. Me ejecutare?.\n");
    return 0;
}



#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Ejecutando el comando 'date' desde execlp...\n");
    execlp("date", "date", NULL);
    printf("Este mensaje nunca se mostrara.\n");
    return 0;
}


#include <stdio.h>
#include <pthread.h>

void* imprimir(void* args) {
    printf("Hilo en ejecucion con ID: %lu\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t hilo;
    pthread_create(&hilo, NULL, imprimir, NULL);
    pthread_join(hilo, NULL);X
    return 0;
}
