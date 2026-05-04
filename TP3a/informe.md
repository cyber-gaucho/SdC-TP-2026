![](./assets/1_1_Isologotipo%20FCEFyN%20y%20UNC-%20original_%20fondo-Con%20bajada.jpg "UNC y FCEFyN")

# TRABAJO PRÁCTICO N° 3a  
## Interfaz de Firmware Extensible Unificada (UEFI)

**Integrantes:**
- García, Lautaro Misael
- Gomez, Dolores
- Renaudo Gaggioli, Valentino

**Mayo 2026**

---

## Introducción

---


## TP 1: Exploración del entorno UEFI y la Shell

### 1. Introducción

En este trabajo se exploró el entorno UEFI utilizando QEMU con firmware OVMF. Se analizaron dispositivos, memoria, variables NVRAM y la arquitectura basada en handles y protocolos.

---

### 2. Entorno de ejecución

Se utilizó QEMU junto con OVMF para simular un entorno UEFI:   
`qemu-system-x86_64 -m 512 -bios /usr/share/ovmf/OVMF.fd -net none`

---

### 3. Exploración del sistema

#### 3.1 Dispositivos

Comando: `map`

Se observó la presencia de dispositivos de tipo BLK, sin sistemas de archivos montados (FS), lo que evidencia que UEFI no asume automáticamente la existencia de particiones accesibles.

---

#### 3.2 Handles y Protocolos

Comando: `dh -b`

Se verificó que UEFI utiliza un modelo basado en handles y protocolos para representar dispositivos y servicios, en lugar de direcciones de hardware fijas.

---

#### 3.3 Variables NVRAM

Comando: `dmpstore -b`

Se alizaron variables como `BootOrder` y `Boot####`, observando que el orden de arranque se define mediante un arreglo de identificadores almacenado en formato binario.

Ejemplo observado: `00 00 01 00`

Interpretación:

- Boot0000
- Boot0001

---

#### 3.4 Memoria

Comando: `memmap -b`

Se identificaron regiones como `RuntimeServices`, las cuales permanecen activas incluso después de que el sistema operativo toma control.

---

#### 3.5 Drivers

Comando: `drivers -b` 

Se observó que el firmware UEFI está compuesto por múltiples módulos (drivers), evidenciando una arquitectura modular.

---

### 4. Respuestas

**Pregunta de Razonamiento 1:**

*Pregunta de Razonamiento 1: Al ejecutar el comando map y dh, vemos protocolos e identificadores en lugar de puertos de hardware fijos. ¿Cuál es la ventaja de seguridad y compatibilidad de este modelo frente al antiguo BIOS?*

El modelo de UEFI basado en handles y protocolos permite abstraer el hardware físico mediante interfaces estandarizadas. Esto reduce la dependencia del hardware, mejora la portabilidad y aumenta la seguridad al evitar accesos directos a recursos críticos.

---

**Pregunta de Razonamiento 2:**

*Observando las variables `Boot####` y `BootOrder`, ¿cómo determina el Boot Manager la secuencia de arranque?*

El Boot Manager determina la secuencia de arranque utilizando la variable `BootOrder`, que contiene una lista ordenada de identificadores. Cada uno corresponde a una variable `Boot####`, que incluye un Device Path hacia el ejecutable `.efi`. El firmware recorre esta lista hasta encontrar una opción válida.

---

**Pregunta de Razonamiento 3:**

*En el mapa de memoria (`memmap`), existen regiones marcadas como `RuntimeServicesCode`. ¿Por qué estas áreas son un objetivo principal para los desarrolladores de malware (Bootkits)?*

Las regiones `RuntimeServices` permanecen accesibles después del arranque del sistema operativo, lo que las convierte en un objetivo crítico para ataques como bootkits. Estas permiten mantener código con altos privilegios y dificultan su detección desde el sistema operativo.

---

### 5. Conclusión

UEFI introduce un modelo más moderno y modular que BIOS, basado en abstracciones y protocolos. Esto mejora la portabilidad y la seguridad, pero también introduce nuevos vectores de ataque a nivel firmware.


---

## TP 2: Desarrollo, Compilación y Análisis de Seguridad

### Desarrollo de la Aplicación UEFI
Se desarrolló una aplicación nativa en C (`aplicacion.c`) que utiliza la tabla de sistema para imprimir en consola y contiene un breakpoint estático (`INT3`).

**Pregunta de Razonamiento 4:**

*¿Por qué utilizamos `SystemTable->ConOut->OutputString` en lugar de la función `printf` de C?*

  > Se debe a que estamos desarrollando para un entorno de pre-arranque o freestanding (compilado con la bandera `-ffreestanding`). La función `printf` pertenece a la biblioteca estándar de C (`libc`), la cual asume y requiere que haya un Sistema Operativo ejecutándose por debajo para proveer llamadas al sistema que envíen los caracteres a la pantalla.
  >
  > Al ejecutar una aplicación UEFI, el Sistema Operativo aún no existe en memoria. Por lo tanto, no contamos con `libc`. Para imprimir en pantalla, debemos interactuar directamente con la API que nos provee el firmware a través de la Tabla de Sistema (`SystemTable`), utilizando el protocolo de salida de texto simple (`ConOut`) para enviar cadenas de caracteres Unicode (`L"..."`) nativas del entorno UEFI.

### Proceso de Compilación y Automatización
Para la generación del binario `.efi`, se utilizó un script de automatización `build.sh` que realiza los siguientes pasos:
1. Compilación a código objeto con `gcc` (flags: `-fpic`, `-ffreestanding`, etc.).
2. Enlace con `ld` utilizando el script de linker de EFI.
3. Conversión de formato con `objcopy` a `efi-app-x86_64`.

![Generación del archivo `.efi` mediante script](./assets/captura_script.png)

### Análisis con Ghidra

Se importó el binario `aplicacion.efi` en Ghidra para analizar la lógica de seguridad y el comportamiento de los opcodes.

**Captura de pantalla:**  
![Función efi_main en Ghidra](./assets/captura_ghidra_int3.png)

**Pregunta de Razonamiento 5:**

*En el pseudocódigo de Ghidra, la condición `0xCC` suele aparecer como `-52`. ¿A qué se debe este fenómeno y por qué importa en ciberseguridad?*

  > **El fenómeno -52:** Se debe a un problema de interpretación de signos. El valor hexadecimal `0xCC` en binario es `1100 1100`. El motor de decompilación de Ghidra asume que las variables de 1 byte son del tipo con signo (`signed char`), lee el bit más significativo como el bit de signo negativo. Al aplicar la regla del complemento a dos para 8 bits, el valor lógico de `1100 1100` resulta en el número decimal `-52`.
  > 
  > **Importancia en Ciberseguridad:** El valor `0xCC` corresponde a la instrucción `INT3`, que se utiliza para establecer software breakpoints reemplazando el código en la dirección objetivo y generando la excepción `EXCEPTION_BREAKPOINT`. El malware escanea activamente la memoria en busca del byte `0xCC` para detectar si el código está siendo analizado por un depurador, permitiéndole evadir el análisis (técnicas anti-debugging).

**Nota:** Para lograr que el descompilador mostrara el chequeo, fue necesario declarar la variable como volatile en el scope global, forzando a las herramientas a leer la memoria asumiendo que un agente externo o hardware podría haberla modificado.

```C
#include <efi.h>
#include <efilib.h>

// DECLARACIÓN GLOBAL Y VOLÁTIL
volatile unsigned char mi_breakpoint = 0xCC;

EFI_STATUS efi_main(...) { ... }
```

---

## TP 3: Ejecución en Hardware Físico (Bare Metal)

---

## 5. Conclusiones
