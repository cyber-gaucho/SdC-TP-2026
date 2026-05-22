# TP4 - Sistemas de Computación

## Descripción

Este trabajo práctico consiste en la implementación de un módulo del kernel de Linux, un programa de usuario empaquetado mediante checkinstall y la generación de evidencia de ejecución del sistema.

El objetivo principal es comprender el funcionamiento básico de módulos del kernel, su ciclo de vida (carga y descarga), y la interacción con el sistema operativo.

---

## Estructura del proyecto

```
TP4/
    /modulo_kernel/
    /programa_hello/
    /evidencia/
```
## Componentes

### Módulo kernel
Implementa un módulo simple que imprime mensajes al ser cargado y descargado del kernel.

### Programa hello
Programa de usuario empaquetado utilizando checkinstall.

### Evidencia
Incluye logs del sistema y capturas que validan la ejecución del módulo.

---

## Flujo de uso del módulo

Compilación:  
`make`

Carga:  
`sudo insmod mimodulo.ko`

Verificación:  
`dmesg | tail` 
`lsmod | grep mimodulo`

Descarga:  
`sudo rmmod mimodulo`

---

## Objetivo

Demostrar el ciclo completo de un módulo del kernel:
compilación, carga, ejecución y descarga.