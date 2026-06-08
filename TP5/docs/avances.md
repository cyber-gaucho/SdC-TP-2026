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
