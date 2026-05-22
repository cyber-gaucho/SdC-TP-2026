# Módulo Kernel - TP4

## Descripción

Este módulo del kernel de Linux es un ejemplo básico de carga dinámica de código en el kernel.

Al cargarse, escribe un mensaje en el buffer del kernel.
Al descargarse, escribe un mensaje indicando su finalización.

---

## Funciones

### Inicialización
Se ejecuta al cargar el módulo.
Utiliza printk para mostrar un mensaje en el kernel.

### Limpieza
Se ejecuta al descargar el módulo.
También utiliza printk para registrar la salida.

---

## Compilación

Ejecutar:

`make`

Se genera el archivo:
`mimodulo.ko`

---

## Carga del módulo

`sudo insmod mimodulo.ko`

Verificación:
`lsmod | grep mimodulo`

---

## Logs del sistema

`dmesg | tail`

---

## Descarga del módulo

`sudo rmmod mimodulo`

---

## Observaciones

- Requiere permisos de administrador
- Depende de headers del kernel instalado
- Se compila contra el kernel activo del sistema