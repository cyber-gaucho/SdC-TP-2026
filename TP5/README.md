# Estado actual del proyecto

## Completado

* Configuración y acceso remoto a Raspberry Pi Zero W.
* Verificación de kernel y cabeceras instaladas.
* Módulo de prueba (`hello`).
* Character Device Driver básico.
* Comunicación usuario-kernel mediante `copy_to_user()` y `copy_from_user()`.
* Preparación de la herramienta de compilación cruzada ARM.

## En desarrollo

* Driver GPIO definitivo (`cdd_gpio`).
* Generador de señales para XIAO ESP32-S3.
* Aplicación de usuario para adquisición de datos.

## Pendiente

* Integración GPIO ↔ Driver.
* Muestreo periódico.
* Selección de canal.
* Generación de gráficos.
* Interfaz web.
* Compilación cruzada del módulo del kernel.

## Estructura actual

```
TP5
├── docs
│   ├── avances.md
│   └── capturas
│       ├── 01_ssh_conexion_exitosa.png
│       ├── ...
│       └── 08_cross_compile_hello_arm.png
├── driver
│   ├── cdd_basico
│   │   ├── cdd_basico.c
│   │   └── Makefile
│   ├── cdd_gpio
│   ├── cdd_loopback
│   │   ├── cdd_loopback.c
│   │   └── Makefile
│   └── hello
│       ├── hello.c
│       └── Makefile
├── firmware
├── README.md
└── user
    └── arm_toolchain_test
        ├── hello-world.c
        └── Makefile
```
