# Evidencia - TP4

## Descripción

Esta carpeta contiene evidencia del funcionamiento de los ejercicios realizados en el TP4.

Incluye:
- evidencia textual del módulo del kernel
- capturas de compilación y ejecución
- evidencia del empaquetado con `checkinstall`

---

## Archivos incluidos

### dmesg_lautaro.txt
Contiene los mensajes generados por el kernel durante:
- carga del módulo
- ejecución de `printk`
- descarga del módulo

---

### lsmod_lautaro.txt
Muestra el estado del módulo en el sistema operativo.

Permite verificar que el módulo fue cargado correctamente mediante:

`lsmod | grep mimodulo`

---

## Carpeta assets

La carpeta `assets/` contiene capturas relacionadas con:

- compilación del módulo
- carga y descarga del módulo
- compilación del programa hello
- empaquetado con `checkinstall`
- ejecución del programa instalado

---

## Objetivo

Demostrar:

- compilación de módulos del kernel
- carga dinámica en Linux
- verificación mediante `lsmod` y `dmesg`
- empaquetado de software usando `checkinstall`