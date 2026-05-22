# Programa Hello - TP4 (checkinstall)

## Descripción

Este programa es un ejecutable simple en C utilizado para practicar el empaquetado de software en sistemas Linux mediante `checkinstall`.

El objetivo es generar un paquete `.deb` a partir de un programa compilado e instalado en el sistema, en lugar de usar únicamente `make install`.

---

## Estructura del proyecto
```
programa_hello/
    /Makefile
    /src/
    /hello.c
```

---

## Compilación del programa

Desde la carpeta `programa_hello`:
`make`

Esto genera el binario:
`hello`

---

## Instalación tradicional

Instala el binario en el sistema:
`sudo make install`

Ubicación final del ejecutable:
`/usr/local/bin/hello-checkinstall`

---

## Empaquetado con checkinstall

En lugar de instalar directamente, se puede generar un paquete `.deb`:   
`sudo checkinstall`

Durante el proceso:

- Se define el nombre del paquete
- Se define la versión
- Se genera automáticamente el `.deb`

El resultado final es un paquete instalable del sistema.

---

## Instalación del paquete generado
`sudo dpkg -i hello-checkinstall_1.0-1_amd64.deb`

---

## Ejecución
`hello-checkinstall`

Salida esperada:
`Hola desde checkinstall`

---

## Archivos importantes

- `src/hello.c`: código fuente del programa
- `Makefile`: reglas de compilación e instalación
- `hello`: binario generado (no siempre se versiona en git)
- `hello-checkinstall_1.0-1_amd64.deb`: paquete generado (NO se recomienda subirlo al repo)

---

## Notas

- El binario y el `.deb` son artefactos generados y normalmente se ignoran en Git mediante `.gitignore`
- El foco del ejercicio está en el proceso de empaquetado, no en el programa en sí