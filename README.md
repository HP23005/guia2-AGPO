# guia3-GPO
Ejercicio 1: Creación y Activación de Memoria Swap
Actividad: 
Crear un archivo de swap de 512MB con dd if=/dev/zero of=/swapfile bs=1M count=512. 
```bash
sudo dd if=/dev/zero of=/swapfile bs=1M count=512
```

Dar permisos y activar con mkswap, swapon. 
```bash
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
swapon --show
```

Pregunta: ¿Qué efecto tiene el swap en sistemas con poca RAM?
```bash
El swap es muy importante en sistemas con poca RAM, ya que esta evita que los programas se cierren o bloqueen por falta de memoria RAM dandole estabilidad a estos, con un pequeño detalle, al no ser tan rapido el swap como la memoria RAM habrá menor rendimiendo en el sistema.
```

Ejercicio 2: Simulación de Fragmentación de Memoria 
Actividad: 
Escribir un programa en C que reserve bloques de memoria de distintos tamaños y luego los libere de forma intercalada. Usar valgrind o pmap para analizar. 
```bash
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    const int N = 6; // pocos bloques para hacerlo liviano
    size_t sizes[N] = {1024, 2048, 4096, 8192, 16384, 32768}; // tamaños pequeños
    void *ptrs[N];

    printf("Reserva de memoria...\n");

    // Reservar memoria
    for (int i = 0; i < N; i++) {
        ptrs[i] = malloc(sizes[i]);
        if (!ptrs[i]) {
            fprintf(stderr, "Error al reservar bloque %d\n", i);
            return 1;
        }
        printf("Bloque %d: %zu bytes reservados en %p\n", i, sizes[i], ptrs[i]);
    }

    // Liberar algunos bloques intercalados
    printf("\nLiberando bloques intercalados...\n");
    for (int i = 0; i < N; i += 2) {
        free(ptrs[i]);
        printf("Bloque %d liberado\n", i);
    }

    // Pausa corta para poder usar pmap o valgrind
    sleep(5);

    // Liberar el resto
    for (int i = 1; i < N; i += 2) {
        free(ptrs[i]);
        printf("Bloque %d liberado\n", i);
    }

    printf("\nTodos los bloques liberados correctamente.\n");
    return 0;
}
```
```bash
gcc -g swap_intercalado.c -o swap_intercalado
./swap_intercalado
valgrind --leak-check=full ./swap_intercalado
pmap -x $(pgrep swap_intercalado)
```

Pregunta: ¿Cómo afecta la fragmentación al rendimiento?
```bash
La fragmentación afecta el rendimiento de la memoria porque reduce la eficiencia con la que el sistema usa el espacio disponible, haciendo que las operaciones de asignación y acceso sean más lentas o menos óptimas.
```

Ejercicio 3: Simulación de Algoritmos de Reemplazo de Páginas 
Actividad: 
Programar en C un simulador que reciba una secuencia de referencias a memoria y aplique distintos algoritmos de reemplazo. 
```bash
#include <stdio.h>
#include <stdlib.h>

#define MAX 30  // máximo número de referencias

// Función para buscar si una página ya está cargada
int buscar(int paginas[], int n, int pagina) {
    for (int i = 0; i < n; i++)
        if (paginas[i] == pagina)
            return i;
    return -1;
}

// ----------------- FIFO -----------------
int fifo(int referencias[], int n, int marcos) {
    int paginas[marcos], pos = 0, fallos = 0;
    for (int i = 0; i < marcos; i++) paginas[i] = -1;

    for (int i = 0; i < n; i++) {
        int pagina = referencias[i];
        if (buscar(paginas, marcos, pagina) == -1) {
            paginas[pos] = pagina;
            pos = (pos + 1) % marcos;
            fallos++;
        }
    }
    return fallos;
}

// ----------------- LRU -----------------
int lru(int referencias[], int n, int marcos) {
    int paginas[marcos], tiempo[marcos], contador = 0, fallos = 0;
    for (int i = 0; i < marcos; i++) paginas[i] = -1;

    for (int i = 0; i < n; i++) {
        int pagina = referencias[i];
        int idx = buscar(paginas, marcos, pagina);

        if (idx != -1) {
            tiempo[idx] = ++contador;
        } else {
            int pos = 0;
            for (int j = 1; j < marcos; j++)
                if (tiempo[j] < tiempo[pos])
                    pos = j;
            paginas[pos] = pagina;
            tiempo[pos] = ++contador;
            fallos++;
        }
    }
    return fallos;
}

// ----------------- Óptimo -----------------
int optimo(int referencias[], int n, int marcos) {
    int paginas[marcos], fallos = 0;
    for (int i = 0; i < marcos; i++) paginas[i] = -1;

    for (int i = 0; i < n; i++) {
        int pagina = referencias[i];
        if (buscar(paginas, marcos, pagina) != -1)
            continue;

        int pos = -1, farthest = i + 1;
        for (int j = 0; j < marcos; j++) {
            if (paginas[j] == -1) {
                pos = j;
                break;
            }
            int k;
            for (k = i + 1; k < n; k++)
                if (paginas[j] == referencias[k])
                    break;
            if (k > farthest) {
                farthest = k;
                pos = j;
            }
            if (k == n) {
                pos = j;
                break;
            }
        }
        paginas[pos] = pagina;
        fallos++;
    }
    return fallos;
}

// ----------------- MAIN -----------------
int main() {
    int referencias[MAX], n, marcos;

    printf("Número de referencias (máx %d): ", MAX);
    scanf("%d", &n);
    printf("Ingrese la secuencia de referencias:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &referencias[i]);

    printf("Número de marcos: ");
    scanf("%d", &marcos);

    printf("\n--- Resultados ---\n");
    printf("FIFO:   %d fallos de página\n", fifo(referencias, n, marcos));
    printf("LRU:    %d fallos de página\n", lru(referencias, n, marcos));
    printf("Óptimo: %d fallos de página\n", optimo(referencias, n, marcos));

    return 0;
}
```
```bash
gcc -g reemplazo.c -o reemplazo
./reemplazo
```

Pregunta: ¿Por qué LRU suele ser más eficiente que FIFO en la práctica?
```bash
LRU suele ser más eficiente que FIFO porque se adapta al comportamiento de los programas, manteniendo en memoria las páginas más relevantes según su uso reciente, esto reduce significativamente los fallos de página y mejora el rendimiento general del sistema.
```

Ejercicio 4: Configuración de Límites de Memoria por Proceso 
Actividad: 
Usar ulimit -v [kB] para limitar la memoria de un proceso y ejecutar un programa que intente exceder ese límite. 
```bash
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t mb = 0;
    while (1) {
        void *p = malloc(1024 * 1024); // reserva 1 MB por iteración
        if (!p) {
            printf("Error: no se pudo asignar más memoria después de %zu MB\n", mb);
            return 1;
        }
        mb++;
        printf("Memoria asignada: %zu MB\n", mb);
    }
}
```
```bash
gcc memtest.c -o memtest
ulimit -v 5000
./memtest
```

Pregunta: ¿Qué ocurre cuando un proceso supera el límite de memoria asignado?
```bash
La terminal muestra el mensaje Killed, significa que si a un proceso se le limita el acceso a memoria y este trata de superar el limite el sistema lo termina de manera inmediata liberando el espacio en memoria que este estaba usando.
```
