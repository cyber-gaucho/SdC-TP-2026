# Trabajo Práctico 3: Modo Protegido

## Descripción del Proyecto
Este trabajo práctico aborda la evolución del procesador durante el proceso de arranque. Los procesadores x86 mantienen compatibilidad con sus antecesores y para agregar nuevas funcionalidades deben ir “evolucionando” en el tiempo durante el proceso de arranque. 

Todos los CPUs x86 comienzan en **modo real** en el momento de carga (boot time). A través de este TP, configuraremos el procesador para realizar el salto al **modo protegido**, el cual desbloquea características como la protección de memoria, soporte para memoria virtual y conmutación de tareas.

## Estructura del Directorio
* `/codigo`: Contiene los scripts de enlazado (`link.ld`), código en ensamblador y binarios generados.
* `/assets`: Capturas de ejecución en QEMU, depuración con GDB y pruebas en hardware real.
* `informe.md`: Respuestas a los desafíos teóricos (UEFI, Coreboot, Linker y Modo Protegido).

## Requisitos del Sistema
Para compilar y emular el código de este trabajo práctico es necesario contar con:
* **NASM** o **GAS** (Ensamblador)
* **QEMU**: Emulador de hardware (`sudo apt install qemu-system-x86`)
* **GDB**: Para la depuración de la imagen booteable.