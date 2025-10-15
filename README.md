# guia3-AGPO
Ejercicio 1: Creación y Activación de Memoria Swap
Actividad: 
Crear un archivo de swap de 512MB con dd if=/dev/zero of=/swapfile bs=1M count=512. 
```bash

```

Dar permisos y activar con mkswap, swapon. 
```bash

```

Pregunta: ¿Qué efecto tiene el swap en sistemas con poca RAM?


Ejercicio 2: Simulación de Fragmentación de Memoria 
Actividad: 
Escribir un programa en C que reserve bloques de memoria de distintos tamaños y luego los libere de forma intercalada. Usar valgrind o pmap para analizar. 
```bash

```

Pregunta: ¿Cómo afecta la fragmentación al rendimiento?

Ejercicio 3: Simulación de Algoritmos de Reemplazo de Páginas 
Actividad: 
Programar en C un simulador que reciba una secuencia de referencias a memoria y aplique distintos algoritmos de reemplazo. 
```bash

```

Pregunta: ¿Por qué LRU suele ser más eficiente que FIFO en la práctica?

Ejercicio 4: Configuración de Límites de Memoria por Proceso 
Actividad: 
Usar ulimit -v [kB] para limitar la memoria de un proceso y ejecutar un programa que intente exceder ese límite. 
```bash

```

Pregunta: ¿Qué ocurre cuando un proceso supera el límite de memoria asignado?
