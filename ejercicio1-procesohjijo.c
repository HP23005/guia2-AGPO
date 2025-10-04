#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();  // Crear un proceso hijo

    if (pid == -1) {
        // Si hay un error en fork
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Proceso hijo
        printf("Soy el hijo, voy a terminar inmediatamente.\n");
        exit(0);  // El hijo termina inmediatamente
    } else {
        // Proceso padre
        printf("Soy el padre, voy a dormir 30 segundos.\n");
        sleep(30);  // El padre duerme 30 segundos
        printf("El padre ha despertado.\n");
    }

    return 0;
}
