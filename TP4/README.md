# Trabajo Práctico N° 4: Módulos de Kernel

**Grupo: mate --refill**

## Integrantes
- García, Lautaro Misael
- Gomez, Dolores
- Renaudo Gaggioli, Valentino

## Introducción
Este directorio contiene el código fuente, los scripts y la documentación inicial correspondientes al Trabajo Práctico N° 4. El objetivo principal de este práctico es interactuar de forma directa con el kernel de Linux, comprendiendo la diferencia entre el espacio de usuario y el espacio de núcleo mediante el desarrollo y la gestión de módulos cargables.

## Experiencias Realizadas
Durante el desarrollo de este trabajo práctico se llevaron a cabo las siguientes actividades:

* **Gestión de paquetes:** Uso de la herramienta `checkinstall` para empaquetar e instalar un programa básico en C de forma limpia en el sistema.
* **Análisis de llamadas al sistema:** Utilización de `strace` para trazar e inspeccionar las *syscalls* ejecutadas por un binario de espacio de usuario.
* **Inspección de Hardware:** Relevamiento de la información del hardware físico del equipo host mediante la utilidad `hwinfo`.
* **Desarrollo de un Módulo de Kernel:** Escritura de un módulo en C (`mimodulo.c`) diseñado para interactuar con las estructuras internas del kernel y registrar el nombre del equipo en el log del sistema.
* **Gestión del Módulo:** Compilación del código fuente mediante un `Makefile` específico para el kernel, e inserción/remoción del módulo utilizando utilidades como `insmod`, `rmmod`, `modinfo` y `dmesg`.
* **Análisis de Seguridad (Secure Boot):** Investigación sobre el impacto de la característica UEFI Secure Boot en la carga de módulos de terceros, los mecanismos de firma digital y el proceso de enrolamiento de claves con `mokutil`.
