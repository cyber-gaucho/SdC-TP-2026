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
