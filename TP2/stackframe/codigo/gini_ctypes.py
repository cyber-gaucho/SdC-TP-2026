from gini_api import obtener_gini_argentina
import ctypes

# cargar librería compartida
lib = ctypes.CDLL("./libprocesar.so")

# definir tipos
lib.procesar.argtypes = [ctypes.c_float]
lib.procesar.restype = ctypes.c_int

# Wrapper para llamar a la función de C
def procesar(x):
    return lib.procesar(x)

def main():
    datos = obtener_gini_argentina()

    if not datos:
        print("No se encontraron datos")
        return

    print("Índice GINI - Argentina\n")

    for year, value in datos:
        resultado = procesar(value)
        print(f"Año: {year} | GINI original: {value} | Procesado: {resultado}")


if __name__ == "__main__":
    main()
