# Avance 01 - Preparación del entorno de desarrollo

## Objetivo

Verificar el acceso al hardware objetivo y obtener información sobre el entorno donde se ejecutará el Device Driver.

## Actividades realizadas

Se estableció conexión remota mediante SSH con la Raspberry Pi Zero W utilizada para el desarrollo del Trabajo Práctico 5.

La conexión se realizó correctamente desde una estación de trabajo Ubuntu mediante:

```bash
ssh pi@192.168.1.38
```

Una vez establecida la conexión se verificó la versión del kernel y del sistema operativo.

### Información obtenida

Kernel:

```text
Linux rpizero 6.12.75+rpt-rpi-v6
```

Sistema operativo:

```text
Raspbian GNU/Linux 12 (Bookworm)
```

Arquitectura:

```text
armv6l
```

## Resultados

Se confirmó que la Raspberry Pi se encuentra operativa y accesible mediante SSH.

Además se identificó la versión exacta del kernel sobre la cual deberá compilarse y probarse el módulo del kernel correspondiente al Character Device Driver.

Esta información será utilizada posteriormente para configurar correctamente el entorno de compilación cruzada y verificar compatibilidad con las APIs GPIO disponibles.

# Avance 02 - Verificación del entorno para desarrollo de módulos del kernel

## Objetivo

Comprobar que la Raspberry Pi dispone de las herramientas y cabeceras necesarias para compilar módulos del kernel compatibles con la versión actualmente instalada.

## Actividades realizadas

Se verificó la existencia del árbol de compilación asociado al kernel en ejecución mediante:

```bash
ls /lib/modules/$(uname -r)/build
```

También se inspeccionaron los paquetes instalados relacionados con cabeceras del kernel:

```bash
dpkg -l | grep headers
```

Finalmente se verificó la disponibilidad de las herramientas de compilación:

```bash
gcc --version
make --version
```

## Resultados obtenidos

Se confirmó la presencia de las cabeceras:

```text
linux-headers-6.12.75+rpt-rpi-v6
```

correspondientes exactamente al kernel en ejecución:

```text
6.12.75+rpt-rpi-v6
```

Asimismo se verificó la presencia del directorio:

```text
/lib/modules/6.12.75+rpt-rpi-v6/build
```

lo que indica que el sistema está preparado para compilar módulos externos.

También se confirmó la disponibilidad de:

* GCC 12.2.0
* GNU Make 4.3

## Conclusiones

La plataforma de desarrollo se encuentra correctamente configurada para la construcción de módulos del kernel Linux.

No fue necesario instalar componentes adicionales para comenzar el desarrollo del Character Device Driver requerido por el trabajo práctico.

# Avance 03 - Construcción y ejecución del primer módulo del kernel

## Objetivo

Verificar la capacidad de compilar, cargar y descargar módulos externos sobre la Raspberry Pi utilizando las cabeceras del kernel instaladas previamente.

## Actividades realizadas

Se desarrolló un módulo mínimo del kernel Linux compuesto por una función de inicialización y una función de finalización.

Se enviaron los archivos `hello.c` y `Makefile` a la Raspberry Pi a través de SSH usando el comando `scp`.

La función de inicialización registra un mensaje en el log del kernel cuando el módulo es cargado mediante `insmod`.

La función de finalización registra un mensaje cuando el módulo es removido mediante `rmmod`.

Posteriormente se compiló el módulo utilizando la infraestructura de construcción provista por el kernel Linux.

## Verificaciones realizadas

Se ejecutaron los siguientes pasos:

1. Compilación del módulo.
2. Inserción del módulo en el kernel.
3. Verificación de mensajes mediante `dmesg`.
4. Remoción del módulo.
5. Nueva verificación mediante `dmesg`.

## Resultados

El módulo fue compilado exitosamente y generó un archivo objeto cargable (`.ko`).

Los mensajes registrados en el log del kernel confirmaron la ejecución correcta de las funciones de inicialización y finalización.

## Conclusiones

Se verificó el flujo básico de desarrollo de módulos del kernel Linux:

* compilación,
* carga,
* ejecución,
* descarga.

Este mecanismo constituye la base para la implementación posterior del Character Device Driver requerido por el trabajo práctico.

# Diseño incremental del Character Device Driver

Luego de analizar los ejemplos provistos por la cátedra (`drv3` y `drv4`), se decidió adoptar una estrategia de desarrollo incremental.

Los ejemplos muestran una progresión natural:

1. Registro dinámico del dispositivo.
2. Creación automática del archivo de dispositivo en `/dev`.
3. Implementación de las operaciones básicas `open`, `read`, `write` y `release`.
4. Transferencia de datos entre espacio de usuario y espacio de kernel mediante `copy_to_user()` y `copy_from_user()`.

Siguiendo este enfoque, el desarrollo del TP se dividirá en etapas sucesivas, validando cada funcionalidad antes de incorporar acceso a GPIO y sensado de señales externas.

El primer objetivo será construir un Character Device Driver mínimo que permita verificar correctamente la interacción entre aplicaciones de usuario y el módulo del kernel mediante llamadas a `open`, `read`, `write` y `close`.

# Avance 04 - Implementación del Character Device Driver básico

## Objetivo

Implementar un Character Device Driver mínimo que permita registrar un dispositivo de caracteres en el kernel Linux y verificar la interacción básica entre espacio de usuario y espacio de kernel.

## Base utilizada

Se tomó como referencia el ejemplo `drv3` provisto por la cátedra, realizando modificaciones orientadas a adaptarlo a la estructura del Trabajo Práctico.

## Cambios realizados respecto al ejemplo original

### Renombrado de estructuras y funciones

Se reemplazaron los nombres genéricos utilizados en el ejemplo por nombres relacionados con el proyecto TP5.

Por ejemplo:

* `drv3_init()` → `tp5_init()`
* `drv3_exit()` → `tp5_exit()`
* `my_open()` → `tp5_open()`
* `my_close()` → `tp5_release()`
* `pugs_fops` → `tp5_fops`

Esto facilita la identificación del código y mejora su mantenibilidad a medida que se incorporen nuevas funcionalidades.

### Definición de constantes para nombres del dispositivo

Se incorporaron macros para centralizar la configuración:

```c
#define DEVICE_NAME "tp5"
#define CLASS_NAME  "tp5_class"
```

De esta forma, cualquier cambio futuro en el nombre del dispositivo puede realizarse modificando una única definición.

### Mejora de mensajes de depuración

Se actualizaron los mensajes enviados mediante `printk()` para utilizar un formato uniforme:

```text
TP5: open()
TP5: read()
TP5: write()
TP5: release()
```

Esto simplifica la interpretación de los eventos registrados en el log del kernel.

### Reorganización del código

El código fue dividido en secciones claramente diferenciadas:

* Operaciones del dispositivo.
* Estructura `file_operations`.
* Inicialización del módulo.
* Finalización del módulo.

Esta organización facilitará la incorporación posterior de:

* Transferencia de datos mediante `copy_to_user()` y `copy_from_user()`.
* Lectura de GPIO.
* Selección de canales de entrada.
* Muestreo periódico de señales.

## Resultado esperado

Al cargar el módulo deberá crearse automáticamente el archivo:

```text
/dev/tp5
```

y las operaciones realizadas desde espacio de usuario (`cat`, `echo`, etc.) deberán reflejarse mediante mensajes de depuración en el registro del kernel.

## Adaptación a Linux 6.12

Durante la implementación del Character Device Driver se detectó una incompatibilidad entre los ejemplos provistos por la cátedra y la versión del kernel instalada en la Raspberry Pi.

Los ejemplos utilizaban la llamada:

```c
class_create(THIS_MODULE, "nombre");
```

Sin embargo, en Linux 6.12 la interfaz fue simplificada y la función recibe únicamente el nombre de la clase:

```c
class_create("nombre");
```

Fue necesario adaptar el código para compatibilizarlo con la API del kernel presente en la plataforma de desarrollo.

# Avance 05 - Intercambio de datos entre espacio de usuario y espacio de kernel

## Objetivo

Implementar un mecanismo básico de intercambio de datos utilizando las funciones `copy_from_user()` y `copy_to_user()`.

## Modificaciones realizadas

Se incorporó una variable interna del kernel utilizada como almacenamiento temporal.

La operación `write()` copia un byte desde espacio de usuario hacia espacio de kernel mediante `copy_from_user()`.

La operación `read()` devuelve dicho byte nuevamente al espacio de usuario mediante `copy_to_user()`.

## Resultados

Las pruebas realizadas permitieron escribir datos sobre el dispositivo de caracteres mediante:

```bash
echo A > /dev/tp5
```

y recuperarlos posteriormente mediante:

```bash
cat /dev/tp5
```

verificando el correcto funcionamiento del intercambio de información entre ambos espacios de memoria.

## Conclusiones

Esta etapa valida el mecanismo fundamental que utilizará el driver definitivo para transferir mediciones obtenidas desde los GPIO hacia las aplicaciones de usuario.

# Diseño preliminar de la interfaz GPIO

La consigna requiere el sensado de dos señales externas y la posibilidad de seleccionar cuál de ellas será leída por la aplicación de usuario.

Para cumplir este requisito se decidió utilizar dos entradas GPIO de la Raspberry Pi conectadas a una única placa XIAO ESP32-S3.

La selección de canal se realizará mediante operaciones de escritura sobre el dispositivo de caracteres:

```text
echo 1 > /dev/tp5
```

Selecciona la primera señal.

```text
echo 2 > /dev/tp5
```

Selecciona la segunda señal.

Posteriormente una operación de lectura:

```text
cat /dev/tp5
```

obtendrá el valor correspondiente al canal actualmente seleccionado.

Este esquema mantiene separadas las funciones de configuración y adquisición de datos, simplificando la implementación del controlador y de la aplicación de usuario.

# Avance 06 - Exploración del subsistema GPIO

Se verificó la disponibilidad del subsistema GPIO de la Raspberry Pi mediante la interfaz de depuración del kernel:

```bash
cat /sys/kernel/debug/gpio
```

La salida confirmó la presencia del controlador `pinctrl-bcm2835` y permitió identificar los GPIO disponibles.

Particularmente se localizaron los pines que serán utilizados durante el desarrollo:

```text
GPIO17 -> gpio-529
GPIO27 -> gpio-539
```

Estos pines fueron seleccionados por ser GPIO de propósito general que no interfieren con periféricos habitualmente utilizados por el sistema.

La inspección del sistema también confirmó la disponibilidad de la interfaz GPIO exportada por el kernel Linux, lo que permitirá realizar pruebas preliminares de hardware antes de integrar el acceso a GPIO dentro del Character Device Driver.

# Avance 07 - Preparación del entorno de compilación cruzada

## Objetivo

Preparar el entorno de desarrollo de la estación de trabajo para generar binarios destinados a la arquitectura ARM utilizada por la Raspberry Pi Zero W.

## Actividades realizadas

Mediante la [guía de instalación](https://learn.arm.com/install-guides/gcc/cross/) se instaló la cadena de herramientas de compilación cruzada para ARM de 32 bits:

* `gcc-arm-linux-gnueabihf`
* `binutils-arm-linux-gnueabihf`

> Acerca de [`bintuils`](https://packages.debian.org/unstable/binutils-arm-linux-gnueabihf)

Estas herramientas permiten generar ejecutables ARM desde un sistema anfitrión x86_64 sin necesidad de compilar directamente sobre la Raspberry Pi.

## Validación prevista

Como primera prueba se desarrolló una aplicación mínima denominada `hello_arm.c`.

El programa será compilado utilizando el compilador cruzado y posteriormente transferido a la Raspberry Pi para verificar:

1. La correcta generación de ejecutables ARM.
2. La compatibilidad de los binarios generados con la plataforma objetivo.
3. El flujo de trabajo requerido por la consigna para la compilación cruzada.

## Relación con el trabajo práctico

La compilación cruzada constituye uno de los requisitos explícitos de la consigna.

La misma metodología será utilizada posteriormente para generar:

* El módulo del kernel (`.ko`).
* Las aplicaciones de usuario.
* Las herramientas de prueba asociadas al Character Device Driver.

# Observación sobre compatibilidad de arquitectura

Durante las pruebas iniciales de compilación cruzada se generó correctamente un ejecutable ARM utilizando la herramienta `arm-linux-gnueabihf-gcc`.

La verificación mediante `file` confirmó la generación de un binario ARM de 32 bits.

Sin embargo, al ejecutar el programa sobre la Raspberry Pi Zero W se produjo un error de segmentación (`Segmentation Fault`).

> ver imagen [`09_compile_segfault_readelf.png`](./capturas/09_compile_segfault_readelf.png)

Dado que la Raspberry Pi Zero W utiliza un [procesador ARMv6](https://developer.arm.com/documentation/ddi0301/h/introduction/about-the-processor), se inició una etapa adicional de validación para verificar la compatibilidad entre la arquitectura objetivo y las opciones de compilación utilizadas por la herramienta de compilación cruzada.

Este análisis permitirá ajustar correctamente los parámetros de compilación antes de generar los módulos y aplicaciones definitivas del trabajo práctico.

# Avance 08 - Validación de compilación cruzada para aplicaciones de usuario

## Objetivo

Validar el flujo de compilación cruzada requerido por la consigna mediante la construcción de una aplicación simple en lenguaje C desde el host Ubuntu para su posterior ejecución en la Raspberry Pi Zero W.

## Herramientas utilizadas

Se instaló el toolchain:

```bash
gcc-arm-linux-gnueabihf
binutils-arm-linux-gnueabihf
```

Posteriormente se desarrolló un programa mínimo:

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, Arm World!\n");
    return 0;
}
```

## Resultados obtenidos

La compilación se realizó exitosamente utilizando:

```bash
arm-linux-gnueabihf-gcc hello-world.c -o hello-world
```

La inspección mediante:

```bash
file hello-world
```

confirmó la generación de un ejecutable ARM de 32 bits.

Sin embargo, al ejecutar el binario en la Raspberry Pi Zero W se produjo un error:

```text
Segmentation fault
```

El análisis posterior mediante:

```bash
readelf -A hello-world
```

reveló que el ejecutable continuaba siendo generado para arquitectura ARMv7 a pesar de los intentos de forzar ARMv6 mediante opciones de compilación.

También se evaluó el uso del toolchain:

```bash
arm-linux-gnueabi-gcc
```

pero los binarios resultantes requerían un cargador dinámico inexistente en el sistema operativo instalado en la Raspberry Pi.

## Verificación del entorno objetivo

Se comprobó que la Raspberry Pi Zero W utiliza:

```text
armv6l
ARMv6-compatible processor rev 7
arm-linux-gnueabihf
```

Asimismo se verificó que la compilación nativa en la Raspberry funciona correctamente mediante:

```bash
gcc hello.c -o hello
./hello
```

obteniéndose la salida esperada.

## Conclusión

Se logró validar:

* La instalación y funcionamiento básico de la herramienta de compilación cruzada.
* La transferencia de binarios mediante SSH/SCP.
* La existencia de incompatibilidades entre la toolchain utilizada y la arquitectura ARMv6 de la Raspberry Pi Zero W.

Dado que los módulos del kernel no dependen de la biblioteca estándar de C ni del cargador dinámico utilizado por las aplicaciones de usuario, se decidió continuar con la preparación de la compilación cruzada del Character Device Driver.

# Avance 09 - Preparación del entorno de compilación cruzada para módulos

## Objetivo

Preparar en el host una copia local de los encabezados del kernel utilizados por la Raspberry Pi Zero W para permitir la compilación cruzada de módulos externos.

## Verificaciones realizadas

Se comprobó la versión exacta del kernel objetivo:

```text
6.12.75+rpt-rpi-v6
```

También se verificó la presencia de los enlaces simbólicos estándar:

```text
/lib/modules/6.12.75+rpt-rpi-v6/build
/lib/modules/6.12.75+rpt-rpi-v6/source
```

apuntando a los paquetes de cabeceras instalados en el sistema.

## Resultado

Se exportaron los directorios:

* `linux-headers-6.12.75+rpt-rpi-v6`
* `linux-headers-6.12.75+rpt-common-rpi`

para ser utilizados posteriormente desde el entorno de desarrollo Ubuntu.

Esta información permitirá compilar módulos del kernel compatibles con la Raspberry Pi sin necesidad de realizar la compilación directamente sobre el dispositivo.

### Dependencias adicionales del sistema de compilación del kernel

Durante la preparación del entorno de compilación cruzada se observó que los paquetes de encabezados del kernel no son autocontenidos.

Los directorios `scripts` y `tools` son enlaces simbólicos hacia archivos ubicados en:

`/usr/lib/linux-kbuild-6.12.75+rpt`

Por este motivo fue necesario exportar también dicho árbol de directorios desde la Raspberry Pi para reproducir correctamente el entorno de construcción en el host Ubuntu.

### Ejecución de herramientas auxiliares del sistema de build

Durante la compilación cruzada del módulo se observó que el sistema de construcción del kernel utiliza herramientas auxiliares ejecutadas en la máquina anfitriona.

Una de ellas (`fixdep`) había sido exportada desde la Raspberry Pi junto con el árbol `linux-kbuild`, por lo que estaba compilada para arquitectura ARM.

Al ejecutarse en el host Ubuntu (x86_64) produjo el error:

```text
Exec format error
```

Esto permitió identificar una diferencia importante entre:

* Herramientas que deben ejecutarse en el host.

* Binarios que deben generarse para la arquitectura destino.

La resolución consiste en utilizar las herramientas de construcción nativas del host Ubuntu y reservar la compilación cruzada únicamente para los módulos del kernel.

# Avance 10 - Validación del entorno de compilación cruzada

### Objetivo

Verificar la posibilidad de compilar módulos del kernel para la Raspberry Pi Zero W desde el host Ubuntu utilizando compilación cruzada.

### Preparación del entorno

Se instaló el compilador cruzado para arquitectura ARM (`arm-linux-gnueabihf-gcc`) y se exportaron desde la Raspberry Pi los encabezados del kernel y los archivos auxiliares utilizados por el sistema de construcción Kbuild.

Con este entorno se intentó compilar un módulo de prueba (`hello.ko`) desde Ubuntu utilizando la misma versión de kernel instalada en la Raspberry Pi.

### Análisis de errores durante la compilación

La compilación avanzó correctamente hasta la etapa de generación del objeto del módulo:

```text
CC [M] hello.o
```

Sin embargo, el proceso se detuvo con errores del tipo:

```text
Exec format error
```

correspondientes a herramientas auxiliares de Kbuild tales como:

```text
fixdep
modpost
recordmcount
```

Para investigar el problema se analizaron los ejecutables involucrados utilizando la herramienta `file`.

Se comprobó que dichas herramientas habían sido exportadas desde la Raspberry Pi y estaban compiladas para arquitectura ARM, mientras que el host de desarrollo utiliza arquitectura x86_64.

Por este motivo el sistema de construcción intentaba ejecutar binarios ARM sobre Ubuntu, provocando los errores observados.

### Conclusiones

El análisis permitió comprobar que:

* El compilador cruzado ARM se encontraba correctamente instalado.
* Los encabezados del kernel exportados eran compatibles con la versión ejecutada por la Raspberry Pi.
* El sistema Kbuild alcanzaba efectivamente la etapa de compilación del módulo.
* Las fallas observadas no estaban relacionadas con el módulo desarrollado sino con herramientas auxiliares precompiladas para ARM que debían ejecutarse localmente en el host.

A partir de este resultado se concluyó que la estrategia más robusta consiste en utilizar un árbol completo de fuentes del kernel Raspberry Pi en el host, permitiendo que Kbuild genere automáticamente las herramientas auxiliares para la arquitectura anfitriona mientras mantiene la compilación del módulo para ARM.

# Avance 11 - Obtención del árbol completo del kernel Raspberry Pi

### Objetivo

Preparar un entorno de compilación cruzada completo utilizando las fuentes oficiales del kernel Raspberry Pi en lugar de depender exclusivamente de los encabezados exportados desde la placa.

### Procedimiento

Se descargó la rama correspondiente a la serie 6.12 del repositorio oficial del kernel Raspberry Pi:

```bash
git clone --depth=1 --branch rpi-6.12.y \
https://github.com/raspberrypi/linux.git kernel-src
```

Posteriormente se generó la configuración base para Raspberry Pi Zero utilizando:

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- olddefconfig
```

### Resultado

Durante la configuración se observó la compilación automática de herramientas auxiliares del sistema Kbuild:

```text
HOSTCC scripts/basic/fixdep
HOSTCC scripts/kconfig/conf.o
HOSTLD scripts/kconfig/conf
```

Esto confirmó que dichas herramientas fueron construidas para la arquitectura anfitriona (x86_64), eliminando el problema detectado anteriormente al exportar directamente los paquetes de encabezados desde la Raspberry Pi.

### Conclusión

El árbol completo del kernel proporciona un entorno de compilación cruzada más adecuado que la simple exportación de encabezados, ya que genera automáticamente las herramientas auxiliares necesarias para el host mientras mantiene la capacidad de producir módulos destinados a la arquitectura ARM.

# Avance 12 - Compilación cruzada exitosa del módulo GPIO

### Objetivo

Lograr la compilación cruzada de un módulo del kernel para Raspberry Pi Zero W utilizando un entorno de desarrollo x86_64.

### Procedimiento

Se utilizó un árbol de encabezados exportado desde la Raspberry Pi junto con las herramientas Kbuild necesarias para compilar módulos externos desde una PC Ubuntu.

Durante el proceso se detectaron problemas de compatibilidad entre distintas versiones del kernel y diferencias entre la configuración utilizada para construir el kernel en la Raspberry Pi y la empleada localmente.

Se verificó la correspondencia entre:

* versión del kernel;
* vermagic;
* configuración del kernel;
* encabezados exportados;
* herramientas auxiliares de compilación.

Tras varias iteraciones se obtuvo una compilación cruzada funcional del módulo `cdd_gpio`.

### Resultado

El módulo generado pudo cargarse correctamente sobre la Raspberry Pi Zero W ejecutando el kernel correspondiente.

```bash
sudo insmod cdd_gpio.ko
```

Los mensajes del kernel confirmaron la inicialización correcta del dispositivo de caracteres y la obtención de los descriptores GPIO.

### Conclusión

La compilación cruzada de módulos para Raspberry Pi requiere no sólo los encabezados del kernel sino también una correspondencia estricta entre la configuración, la versión del kernel y las herramientas Kbuild utilizadas durante el proceso.

# Avance 13 - Implementación del driver GPIO

### Objetivo

Modificar el driver de prueba desarrollado previamente para que permitiera adquirir señales digitales reales desde los GPIO de la Raspberry Pi.

### Procedimiento

Se desarrolló el módulo `cdd_gpio`, basado en la infraestructura de dispositivos de caracteres implementada en avances anteriores.

El driver:

* registra dinámicamente un dispositivo de caracteres;
* crea el nodo `/dev/tp5`;
* permite seleccionar un canal mediante `write()`;
* permite leer el estado lógico mediante `read()`.

Se utilizaron los GPIO:

| Canal   | GPIO   |
| ------- | ------ |
| Canal 1 | GPIO17 |
| Canal 2 | GPIO27 |

### Resultado

La selección de canal pudo realizarse mediante:

```bash
echo -n "1" > /dev/tp5
```

o

```bash
echo -n "2" > /dev/tp5
```

y la lectura mediante:

```bash
cat /dev/tp5
```

obteniendo correctamente los niveles lógicos presentes en cada entrada.

### Conclusión

El módulo desarrollado permitió validar la interacción entre espacio kernel y hardware real, cumpliendo el objetivo principal del trabajo práctico.


# Avance 14 - Aplicación de monitoreo y visualización web

### Objetivo

Desarrollar una aplicación de usuario capaz de visualizar las señales adquiridas por el driver en tiempo real.

### Procedimiento

Inicialmente se implementó una herramienta de consola para monitorear las señales y calcular:

* frecuencia;
* período;
* duty cycle.

Posteriormente se desarrolló una aplicación web utilizando Flask y JavaScript.

La aplicación:

* accede al dispositivo `/dev/tp5`;
* selecciona el canal solicitado;
* adquiere muestras periódicamente;
* calcula frecuencia y duty cycle;
* publica los datos mediante una interfaz web.

### Arquitectura

```text
Raspberry Pico 2 WH
        │
        ▼
GPIO17 / GPIO27
        │
        ▼
cdd_gpio
        │
        ▼
/dev/tp5
        │
        ▼
Flask
        │
        ▼
Interfaz Web
```

### Resultado

Se obtuvo una interfaz web accesible desde la red local que permite:

* seleccionar canales;
* visualizar la forma de onda;
* medir frecuencia;
* medir duty cycle.

Las pruebas se realizaron utilizando una Raspberry Pi Pico 2 WH como generador de señales digitales.

### Conclusión

La aplicación completa permitió integrar hardware, driver del kernel y software de usuario en una única plataforma de monitoreo accesible mediante navegador web.

