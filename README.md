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


Ejercicio 4: Creación de procesos con fork() 
Escribe un programa que cree un proceso hijo utilizando fork(). El proceso hijo debe imprimir su PID y el PPID (Parent Process ID). El proceso padre debe imprimir su propio PID y el PID del hijo. Además, el padre debe esperar a que el hijo termine usando wait().
En base a la realización del ejercicio responda con sus propias palabras:
•	¿Qué utilidad tiene la función fork() en este caso?
•	¿Cómo se puede aprovechar el uso de fork() en un software real?


Ejercicio 5: Creación de hilos con pthread_create() 
Escribe un programa en C que cree dos hilos utilizando la API de POSIX (pthreads). Cada hilo debe imprimir un mensaje identificativo (por ejemplo, "Hilo 1" y "Hilo 2"). El programa principal (hilo principal) debe esperar a que ambos hilos terminen su ejecución utilizando pthread_join(). Al final, el hilo principal debe imprimir un mensaje indicando que todos los hilos han terminado.
En base a la realización del ejercicio responda con sus propias palabras: 
•	¿Qué utilidad tienen los hilos en este caso? 
•	¿Cómo se puede aprovechar el uso de hilos en un software real?
