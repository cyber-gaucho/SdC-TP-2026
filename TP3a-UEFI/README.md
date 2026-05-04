# TP3a - UEFI: Exploración del entorno

## 1. Introducción

En este trabajo se exploró el entorno UEFI utilizando QEMU con firmware OVMF. Se analizaron dispositivos, memoria, variables NVRAM y la arquitectura basada en handles y protocolos.

---

## 2. Entorno de ejecución

Se utilizó QEMU junto con OVMF para simular un entorno UEFI:   
`qemu-system-x86_64 -m 512 -bios /usr/share/ovmf/OVMF.fd -net none`

---

## 3. Exploración del sistema

### 3.1 Dispositivos

Comando: `map`

Se observó la presencia de dispositivos de tipo BLK, sin sistemas de archivos montados (FS), lo que evidencia que UEFI no asume automáticamente la existencia de particiones accesibles.

---

### 3.2 Handles y Protocolos

Comando: `dh -b`

Se verificó que UEFI utiliza un modelo basado en handles y protocolos para representar dispositivos y servicios, en lugar de direcciones de hardware fijas.

---

### 3.3 Variables NVRAM

Comando: `dmpstore`

Se analizaron variables como `BootOrder` y `Boot####`, observando que el orden de arranque se define mediante un arreglo de identificadores almacenado en formato binario.

Ejemplo observado: `00 00 01 00`

Interpretación:

- Boot0000
- Boot0001

---

### 3.4 Memoria

Comando: `memmap -b`

Se identificaron regiones como `RuntimeServices`, las cuales permanecen activas incluso después de que el sistema operativo toma control.

---

### 3.5 Drivers

Comando: `drivers -b` 

Se observó que el firmware UEFI está compuesto por múltiples módulos (drivers), evidenciando una arquitectura modular.

---

## 4. Respuestas

### Pregunta 1

El modelo de UEFI basado en handles y protocolos permite abstraer el hardware físico mediante interfaces estandarizadas. Esto reduce la dependencia del hardware, mejora la portabilidad y aumenta la seguridad al evitar accesos directos a recursos críticos.

---

### Pregunta 2

El Boot Manager determina la secuencia de arranque utilizando la variable `BootOrder`, que contiene una lista ordenada de identificadores. Cada uno corresponde a una variable `Boot####`, que incluye un Device Path hacia el ejecutable `.efi`. El firmware recorre esta lista hasta encontrar una opción válida.

---

### Pregunta 3

Las regiones `RuntimeServices` permanecen accesibles después del arranque del sistema operativo, lo que las convierte en un objetivo crítico para ataques como bootkits. Estas permiten mantener código con altos privilegios y dificultan su detección desde el sistema operativo.

---

## 5. Conclusión

UEFI introduce un modelo más moderno y modular que BIOS, basado en abstracciones y protocolos. Esto mejora la portabilidad y la seguridad, pero también introduce nuevos vectores de ataque a nivel firmware.
