# Stackframe - Guía de compilación y ejecución

Según lo indicado en el documento [TP calculadora de inidces GINI](https://docs.google.com/document/d/1iR36JSzibt84sx7Pg65Ty6jhb3hQ4PmNnv7oCygqSwY/edit?tab=t.0#heading=h.l9frjjl9vc9b)

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
python gini_api.py
python gini_ctypes.py
```

### Salir del entorno

```bash
deactivate
```

> Nota: la carpeta `venv/` no se versiona.

---

# 3. Estructura esperada

```
codigo/
  main.c
  procesar.c      ← implementación en C (iteración 1)
  procesar.asm    ← implementación en ASM (iteración 2)
  gini_api.py     ← consulta a la API
  gini_ctypes.py  ← integración Python - C
```

---

# 4. Compilación

## Iteración 1 (solo C)

```bash
gcc codigo/main.c codigo/procesar.c -o programa
```

---

## Iteración 2 (C + ASM)

```bash
gcc -c codigo/procesar.asm -o procesar.o
gcc -c codigo/main.c -o main.o
gcc main.o procesar.o -o programa
```

> Nota: según la sintaxis usada, el archivo ASM puede necesitar extensión `.s` en lugar de `.asm`.

---

# 5. Compilación como librería compartida (para Python)

Para usar la función de C desde Python con `ctypes`, se debe compilar como librería dinámica.

### Paso 1: generar objeto
```bash
gcc -c procesar.c -o procesar.o
```

### Paso 2: generar librería
```bash
gcc -shared -W -o libprocesar.so procesar.o
```

Esto genera `libprocesar.so`

---

# 6. Ejecución con Python + C

Asegurarse de que `libprocesar.so` esté en el mismo directorio que el script o indicar la ruta correcta.

```bash
python gini_ctypes.py
```

---

# 7. Notas importantes
- No se versionan archivos compilados (.o, .so)
- La librería debe generarse en cada entorno
- Si no existe `libprocesar.so`, la ejecución en Python fallará