# Stackframe - Guía de compilación y ejecución

Según lo indicado en el documento [TP calculadora de indices GINI](https://docs.google.com/document/d/1iR36JSzibt84sx7Pg65Ty6jhb3hQ4PmNnv7oCygqSwY/edit?tab=t.0#heading=h.l9frjjl9vc9b)

---

# 1. Requisitos

### Herramientas necesarias

- GCC (compilador de C)
- GNU Assembler (GAS)
- GDB (debugger)
- Python 3
- pip (gestor de paquetes de Python)

### Instalación (Linux)

```bash
sudo apt update
sudo apt install build-essential gdb python3 python3-venv python3-pip
```

---

# 2. Entorno Python (venv)

Se recomienda usar un entorno virtual para evitar problemas con dependencias.

### Crear entorno

```bash
python3 -m venv venv
```

o especificando una ruta personalizada:

```bash
python3 -m venv <ruta>/<nombre_del_entorno>
```

### Activar entorno

```bash
source venv/bin/activate
```

Si está en otra ubicación:

```bash
source <ruta>/<nombre_del_entorno>/bin/activate
```

### Instalar dependencias

```bash
pip install requests
```

### Ejecutar scripts

```bash
python codigo/gini_api.py
python codigo/gini_ctypes.py
```

### Salir del entorno

```bash
deactivate
```

> Nota: la carpeta `venv/` no se versiona.

---

# 3. Estructura del proyecto

```
codigo/
  main.c
  main_asm.c
  procesar.c          ← implementación en C (iteración 1)
  procesar.s          ← implementación en ASM (iteración 2)
  procesar_wrapper.c  ← wrapper C → ASM
  gini_api.py
  gini_ctypes.py

build/                ← archivos generados (no versionados)
```

---

# 4. Compilación

## Iteración 1 (solo C)

```bash
gcc codigo/main.c codigo/procesar.c -o build/programa
```

Ejecutar:

```bash
./build/programa
```

---

## Iteración 2 (C + ASM)

```bash
as --64 -g -o build/procesar.o codigo/procesar.s
gcc -g -O0 -c codigo/main_asm.c -o build/main_asm.o
gcc build/main_asm.o build/procesar.o -o build/programa_asm
```

Ejecutar:

```bash
./build/programa_asm
```

---

# 5. Librería compartida (C → ASM → Python)

## Compilación manual

```bash
as --64 -g -o build/procesar.o codigo/procesar.s
gcc -g -O0 -fPIC -c codigo/procesar_wrapper.c -o build/wrapper.o
gcc -shared -o build/libprocesar.so build/procesar.o build/wrapper.o
```

---

# 6. Ejecución Python + C + ASM

El script Python debe cargar la librería desde `build/`.

Ejecutar:

```bash
python codigo/gini_ctypes.py
```

---

# 7. Uso con Makefile

Alternativa recomendada:

```bash
make run-c
make run-asm
make run-py
```

---

# 8. Debug con GDB

```bash
gdb ./build/programa
gdb ./build/programa_asm
```

---

# 9. Notas importantes

* No versionar archivos compilados:

  * `.o`
  * `.so`
  * binarios
* La carpeta `build/` está en `.gitignore`
* La librería `libprocesar.so` debe generarse localmente
* Si la librería no existe, Python fallará al cargarla
* El código ASM debe respetar la convención **System V AMD64 ABI**