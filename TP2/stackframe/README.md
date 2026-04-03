# Stackframe - Guía de compilación y ejecución

Según lo indicado en el documento [TP calculadora de inidces GINI](https://docs.google.com/document/d/1iR36JSzibt84sx7Pg65Ty6jhb3hQ4PmNnv7oCygqSwY/edit?tab=t.0#heading=h.l9frjjl9vc9b)

# 1. Requisitos

### Herramientas necesarias

- GCC (compilador de C)
- GNU Assembler (GAS)
- GDB (debugger)

### Instalación (Linux)

```bash
sudo apt update
sudo apt install build-essential gdb
```

---

# 2. Estructura esperada

```
codigo/
  main.c
  procesar.c      ← implementación en C (iteración 1)
  procesar.asm    ← implementación en ASM (iteración 2)
```

---

# 3. Compilación

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
