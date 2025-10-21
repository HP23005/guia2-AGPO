# Proyecto Final
Desarrollo teórico: Introducción a las llamadas al sistema (system calls) en Linux: fork(), exec(), wait(), exit(). Diferencias entre procesos y programas. Estructura de un proceso en memoria.

Ejercicios prácticos:
1. Escribir un programa en C que cree un proceso hijo usando fork(). El hijo debe ejecutar el comando ls -l usando exec(), y el padre debe esperar a que el hijo termine.
```bash
#include <stdio.h>      // Incluye funciones estándar como printf()
#include <stdlib.h>     // Incluye funciones como exit()
#include <unistd.h>     // Contiene fork(), execvp() y otras funciones de UNIX
#include <sys/types.h>  // Define tipos de datos como pid_t
#include <sys/wait.h>   // Contiene la función wait() para sincronizar procesos

int main() {
    pid_t pid; // Variable que almacenará el ID del proceso hijo

    printf("Creando proceso hijo...\n");

    // fork() crea un nuevo proceso duplicando el proceso actual.
    // Devuelve 0 al hijo, y el PID del hijo al proceso padre.
    pid = fork();

    if (pid < 0) {
        // Si el valor es negativo, la creación del proceso falló.
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Este bloque se ejecuta solo en el proceso hijo
        printf("Soy el proceso hijo (PID=%d), ejecutando 'ls -l'...\n", getpid());
        
        // Creamos un arreglo de argumentos para execvp().
        // "ls" es el comando, y "-l" es su argumento.
        char *args[] = {"ls", "-l", NULL};

        // execvp() reemplaza el proceso actual con el nuevo programa ("ls").
        // Si execvp tiene éxito, el resto del código del hijo no se ejecutará.
        if (execvp(args[0], args) == -1) {
            perror("Error al ejecutar execvp");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        // Este bloque se ejecuta solo en el proceso padre
        printf("Soy el proceso padre (PID=%d), esperando a que el hijo termine...\n", getpid());
        
        int status;
        // wait() suspende al padre hasta que el hijo termine su ejecución.
        wait(&status);

        // Analiza el estado de salida del hijo.
        if (WIFEXITED(status)) {
            printf("El proceso hijo terminó normalmente con código de salida %d.\n", WEXITSTATUS(status));
        } else {
            printf("El proceso hijo terminó de forma anormal.\n");
        }

        printf("El proceso padre ha finalizado.\n");
    }

    return 0; // Fin del programa
}
```
```bash
El programa comienza creando un nuevo proceso con la llamada al sistema fork(). Esta función duplica el proceso actual, de modo que ahora existen dos procesos: el padre y el hijo. El valor que devuelve fork() permite distinguirlos: el hijo recibe 0, mientras que el padre recibe el PID del hijo.

En el proceso hijo, se usa execvp() para ejecutar el comando ls -l. Esta función reemplaza la imagen del proceso actual por la del nuevo programa, en este caso el comando ls. Por eso, una vez que execvp() se ejecuta correctamente, el código posterior no se sigue ejecutando en el hijo.

Mientras tanto, el proceso padre no finaliza inmediatamente; en cambio, espera la terminación del hijo usando wait(). Esta función bloquea la ejecución del padre hasta que el hijo termina, garantizando una correcta sincronización. Al terminar, el padre obtiene el estado de salida del hijo y muestra un mensaje final antes de finalizar su ejecución.
```

2. Modificar el programa para que el padre envíe una señal SIGUSR1 al hijo antes de que termine.
```bash
#include <stdio.h>      // Incluye funciones estándar como printf()
#include <stdlib.h>     // Incluye funciones como exit()
#include <unistd.h>     // Contiene fork(), execvp() y otras funciones de UNIX
#include <sys/types.h>  // Define tipos de datos como pid_t
#include <sys/wait.h>   // Contiene la función wait() para sincronizar procesos
#include <signal.h>     // Para usar señales como SIGUSR1 y funciones de manejo

// Función que manejará la señal SIGUSR1 en el proceso hijo
void manejador_senal(int senal) {
    printf("Proceso hijo recibió la señal SIGUSR1 (%d)\n", senal);
}

int main() {
    pid_t pid; // Variable que almacenará el ID del proceso hijo

    printf("Creando proceso hijo...\n");

    // fork() crea un nuevo proceso duplicando el proceso actual.
    // Devuelve 0 al hijo, y el PID del hijo al proceso padre.
    pid = fork();

    if (pid < 0) {
        // Si el valor es negativo, la creación del proceso falló.
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Este bloque se ejecuta solo en el proceso hijo
        printf("Soy el proceso hijo (PID=%d), ejecutando 'ls -l'...\n", getpid());
        
        // Asignamos el manejador de la señal SIGUSR1 antes de ejecutar execvp
        signal(SIGUSR1, manejador_senal);

        // Pausa breve para permitir que el padre envíe la señal antes del exec
        sleep(2);

        // Creamos un arreglo de argumentos para execvp().
        // "ls" es el comando, y "-l" es su argumento.
        char *args[] = {"ls", "-l", NULL};

        // execvp() reemplaza el proceso actual con el nuevo programa ("ls").
        // Si execvp tiene éxito, el resto del código del hijo no se ejecutará.
        if (execvp(args[0], args) == -1) {
            perror("Error al ejecutar execvp");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        // Este bloque se ejecuta solo en el proceso padre
        printf("Soy el proceso padre (PID=%d), esperando a que el hijo termine...\n", getpid());

        // Esperamos un poco antes de enviar la señal
        sleep(1);

        // Enviamos la señal SIGUSR1 al proceso hijo
        printf("El padre envía la señal SIGUSR1 al hijo (PID=%d)\n", pid);
        kill(pid, SIGUSR1);

        int status;
        // wait() suspende al padre hasta que el hijo termine su ejecución.
        wait(&status);

        // Analiza el estado de salida del hijo.
        if (WIFEXITED(status)) {
            printf("El proceso hijo terminó normalmente con código de salida %d.\n", WEXITSTATUS(status));
        } else {
            printf("El proceso hijo terminó de forma anormal.\n");
        }

        printf("El proceso padre ha finalizado.\n");
    }

    return 0; // Fin del programa
}
```
```bash
Se incluye la librería <signal.h>, necesaria para trabajar con señales.

Se define una función llamada manejador_senal() que se ejecuta cuando el proceso recibe una señal SIGUSR1.
En este caso, solo imprime un mensaje indicando que la recibió.

En el proceso hijo, antes de ejecutar execvp(), se instala el manejador con:
signal(SIGUSR1, manejador_senal);

El padre usa la función kill(pid, SIGUSR1); para enviar la señal al hijo.

Se agregan pequeñas pausas (sleep()) para asegurar que el hijo esté preparado antes de recibir la señal.
```
