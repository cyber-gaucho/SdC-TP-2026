![Isologotipo FCEFyN y UNC](./assets/1_1_Isologotipo%20FCEFyN%20y%20UNC-%20original_%20fondo-Con%20bajada.jpg "UNC y FCEFyN")

# TRABAJO PRÁCTICO N° 3  
## Modo Protegido

**Integrantes:**
- García, Lautaro Misael
- Gomez, Dolores
- Renaudo Gaggioli, Valentino

**Abril 2026**

---

## Desafío UEFI

---

## Desafío Linker

### Teoría del Linker
El **linker** (o enlazador) es una herramienta que toma uno o más archivos objeto generados por el compilador o ensamblador y los combina en un único archivo **ejecutable**. Se encarga de ubicar las secciones (como `.text` para código o `.data` para datos) y resolver las direcciones de los símbolos.

Normalmente, un linker genera archivos ejecutables con un formato específico para un sistema operativo, los cuales incluyen metadatos o "cabeceras" que que le dicen al sistema operativo dónde debe cargarse en memoria. Sin embargo, utilizamos la opción `--oformat binary` porque estamos programando sin sistema operativo. Necesitamos generar **código máquina puro** (binario crudo), sin cabeceras, para que la BIOS/UEFI pueda cargarlo directamente en memoria y ejecutarlo como un sector de arranque. Al exportar en formato **binario crudo**, toda esa información de cabecera se elimina. Obtenemos únicamente una "imagen de memoria" exacta de los bytes que deben cargarse, perdiendo la información explícita de la dirección de inicio. Por lo tanto, el código máquina generado debe estar pre-calculado por el linker para funcionar asumiendo una dirección base específica.

### El Script del Linker (`link.ld`)

El **Linker Script** controla explícitamente cómo se mapeará nuestro programa, supliendo la falta de metadatos del archivo binario. Dentro del script, la variable `.` representa el **Location Counter** (contador de posición). Observamos que se le indica que coloque la sección de código (`.text`) a partir de la dirección física `0x7C00`, mediante la instrucción: `. = 0x7C00`.    

Esta dirección es usada por una convención histórica de la arquitectura x86, cuando la PC arranca, la BIOS busca un dispositivo booteable, lee su primer sector (los primeros 512 bytes) y lo copia exactamente en la dirección de memoria RAM `0x0000:0x7C00`. Luego, el procesador hace un salto a esa dirección para comenzar a ejecutar el código.

Además, el script permite usar el Location Counter para forzar que los últimos dos bytes del sector (desplazamientos `0x1FE` y `0x1FF`) contengan la firma mágica `0x55` y `0xAA`.

### El origen del `0x7C00`

La dirección `0x7C00` es una herencia directa del equipo de desarrollo de la **IBM PC 5150** (1981). En aquel entonces, la memoria mínima requerida para correr DOS 1.0 era de **32 KiB** (`0x8000`). El equipo de BIOS decidió ubicar el sector de arranque lo más lejos posible de la zona inicial para dejarle espacio contiguo al Sistema Operativo.
El cálculo fue simple:
* La BIOS ocupaba la parte baja con vectores de interrupción (`0x0 - 0x3FF`) y su área de datos se ubicaba a continuación.
* Se necesitaban 512 bytes para el *Master Boot Record* (MBR) y otros 512 bytes para la pila y datos del bootloader.
* Total requerido para el arranque: **1024 bytes (`0x400`)**.

Entonces se eligieron los últimos 1024B. Si restamos a los 32 KiB el espacio del bootloader: `0x8000 - 0x400 = 0x7C00`.

Desde entonces, por compatibilidad hacia atrás, cualquier BIOS x86 copia el primer sector del disco a la dirección `0x7C00` y salta hacia ella para ceder el control.

### Preparación del Entorno y Submódulos

Siguiendo las instrucciones de la cátedra, el primer paso para la resolución de este trabajo práctico consistió en obtener el código base y los ejemplos provistos. Para ello, clonamos el repositorio oficial de la materia e inicializamos el submódulo correspondiente a los ejemplos *bare-metal* (repositorio de Ciro Santilli). 

Esto se logró mediante los comandos de inicialización y actualización de submódulos en Git:

![Clonación del repositorio y submódulo](assets/clone_repo_profe.png)

Una vez desvinculado del historial original para evitar conflictos, este código fue integrado a la carpeta `codigo/` de nuestro repositorio grupal para facilitar el trabajo colaborativo. Modificamos la copia para incluir nuesto mensaje personalizado, la salida del comando `cowsay -W 14 Hello world! mate --refill`:

```
msg:
    .ascii " _______________\r\n"
    .ascii "/ Hello world!  \\\r\n"
    .ascii "\\ mate --refill /\r\n"
    .ascii " ---------------\r\n"
    .ascii "        \\   ^__^\r\n"
    .ascii "         \\  (oo)\\_______\r\n"
    .ascii "            (__)\\       )\\/\\\r\n"
    .ascii "                ||----w |\r\n"
    .asciz "                ||     ||\r\n"
```

### Análisis del Archivo Objeto (`objdump`)

Al ejecutar `objdump -d 01HelloWorld/main.o`, observamos el código desensamblado antes de que el linker asigne las direcciones finales.

![Análisis de objdump](assets/captura_objdump.png)

**Hallazgos técnicos:**
* **Direcciones Relativas:** En esta etapa, el programa comienza en la dirección `00000000`. Esto se debe a que el ensamblador genera código relativo; es el Linker quien debe "reubicar" estas instrucciones.
* **Interpretación de Datos:** Un detalle muy interesante es que `objdump` es incapaz de distinguir entre código ejecutable y datos puros. Como resultado, intenta decodificar el string ASCII de nuestro dibujo (el *cowsay* con la frase "Hello World! mate --refill") como si fuesen instrucciones x86. 

A continuación, destacamos el fragmento exacto donde nuestro texto se muestra como código ensamblador:

![Desensamblado de datos ASCII](assets/captura_objdump_nombre.png)

Si analizamos los códigos hexadecimales de estas instrucciones, podemos leer nuestro mensaje. Por ejemplo:
  * En la dirección `19 y 24:`, los bytes `48 65 6c 6c 6f` corresponden a **"Hello"**, interpretados como `gs insb`.
  * En la dirección `35:`, los bytes `6d 61 74 65` corresponden a **"mate"**, interpretados como un `and`.
  * En la dirección `3a:`, los bytes `2d 2d 72 65 66...` forman nuestro nombre de grupo **"--refill"**, siendo leídos como otro `and` e `imul`.

### Verificación de la Imagen Final (`hexdump`)

Una vez vinculado el código con `ld --oformat binary`, generamos `main.img`. Usamos `hd 01HelloWorld/main.img` para verificar la estructura cruda del binario.

![Análisis de hexdump](assets/captura_hexdump.png)

**Puntos clave verificados:**
* **Firma de Booteo:** Al final del sector (`0x1FE` y `0x1FF`), verificamos la presencia de los bytes `55 aa`. Sin esta firma obligatoria, la BIOS no reconocería el disco como un medio booteable.
* **Padding Inteligente (Instrucciones NOP):** El linker rellenó el espacio entre el final de nuestro código y la firma de booteo para asegurar que el archivo pese exactamente 512 bytes. Sin embargo, **no rellenó con ceros**. Como estamos dentro de la sección ejecutable (`.text`), rellenó utilizando secuencias de instrucciones **NOP (No Operation) de múltiples bytes** (la secuencia `66 2e 0f 1f...` y `66 90`). Esto es una medida de seguridad de los linkers, ya que el byte `00` se interpreta como la instrucción `ADD`, lo que podría causar fallos de memoria si el flujo de ejecución cayera allí. Los NOPs garantizan que el procesador no haga nada dañino en ese espacio libre.

### Ejecución en Emulador (QEMU)

Finalmente, probamos la imagen en QEMU. El resultado muestra nuestra "vaca" saludando, lo que confirma que el bucle de impresión y las interrupciones de video del BIOS (`int 0x10`) están funcionando correctamente en modo real de 16 bits.

![Ejecución en QEMU](assets/captura_qemu.png)

### Prueba en Hardware Real

Para comprobar que nuestra imagen es un sector de arranque válido más allá del emulador, grabamos el archivo `main.img` en un pendrive físico utilizando el comando `dd`.

![Ejecución en Hardware Real](assets/foto_pendrive.jpg)

---

## Desafío Modo Protegido