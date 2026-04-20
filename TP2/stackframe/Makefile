# =========================
# Configuración
# =========================

CC = gcc
AS = as

CFLAGS = -g -O0
PICFLAGS = -fPIC

SRC_DIR = codigo
BUILD_DIR = build

# =========================
# Archivos
# =========================

MAIN_C = $(SRC_DIR)/main.c
MAIN_ASM_C = $(SRC_DIR)/main_asm.c
PROC_C = $(SRC_DIR)/procesar.c
PROC_ASM = $(SRC_DIR)/procesar.s

MAIN_O = $(BUILD_DIR)/main.o
MAIN_ASM_O = $(BUILD_DIR)/main_asm.o
PROC_O = $(BUILD_DIR)/procesar.o
PROC_ASM_O = $(BUILD_DIR)/procesar_asm.o

LIB = $(BUILD_DIR)/libprocesar.so
BIN = $(BUILD_DIR)/programa
BIN_ASM = $(BUILD_DIR)/programa_asm

PY_API = $(SRC_DIR)/gini_api.py
PY_CTYPES = $(SRC_DIR)/gini_ctypes.py

# =========================
# Targets principales
# =========================

all: c

# Crear carpeta build automáticamente
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# -------------------------
# Iteración 1: solo C
# -------------------------

c: $(BIN)

$(BIN): $(MAIN_O) $(PROC_O)
	$(CC) $(CFLAGS) -o $(BIN) $(MAIN_O) $(PROC_O)

$(MAIN_O): $(MAIN_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(MAIN_C) -o $(MAIN_O)

$(PROC_O): $(PROC_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PICFLAGS) -c $(PROC_C) -o $(PROC_O)

run-c: c
	./$(BIN)

# -------------------------
# Iteración 2: C + ASM
# -------------------------

asm: $(BIN_ASM)

$(BIN_ASM): $(MAIN_ASM_O) $(PROC_ASM_O)
	$(CC) $(CFLAGS) -o $(BIN_ASM) $(MAIN_ASM_O) $(PROC_ASM_O)

$(MAIN_ASM_O): $(MAIN_ASM_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(MAIN_ASM_C) -o $(MAIN_ASM_O)

$(PROC_ASM_O): $(PROC_ASM) | $(BUILD_DIR)
	$(AS) --64 -g -o $(PROC_ASM_O) $(PROC_ASM)

run-asm: asm
	./$(BIN_ASM)

# -------------------------
# Librería para ctypes
# -------------------------

lib: $(LIB)

$(LIB): $(PROC_O)
	$(CC) -shared -o $(LIB) $(PROC_O)

run-py: lib
	python $(PY_CTYPES)

run-api:
	python $(PY_API)

# -------------------------
# Debug
# -------------------------

debug-c: c
	gdb ./$(BIN)

debug-asm: asm
	gdb ./$(BIN_ASM)

# -------------------------
# Limpieza
# -------------------------

clean:
	rm -rf $(BUILD_DIR)

# -------------------------
# Ayuda
# -------------------------

help:
	@echo "Targets disponibles:"
	@echo "  make c        -> Compila versión en C"
	@echo "  make asm      -> Compila versión ASM"
	@echo "  make lib      -> Genera libprocesar.so"
	@echo "  make run-c    -> Ejecuta programa en C"
	@echo "  make run-asm  -> Ejecuta programa ASM"
	@echo "  make run-py   -> Ejecuta Python + ctypes"
	@echo "  make run-api  -> Ejecuta solo consulta API"
	@echo "  make debug-c  -> Debug C con GDB"
	@echo "  make debug-asm-> Debug ASM con GDB"
	@echo "  make clean    -> Limpia archivos generados"