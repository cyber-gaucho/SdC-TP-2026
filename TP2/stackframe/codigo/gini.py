import requests
import ctypes

# cargar librería compartida
lib = ctypes.CDLL("./libprocesar.so")

# definir tipos
lib.procesar.argtypes = [ctypes.c_float]
lib.procesar.restype = ctypes.c_int

URL = "https://api.worldbank.org/v2/en/country/all/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=32500&page=1"

def obtener_gini_argentina():
    response = requests.get(URL)

    if response.status_code != 200:
        print("Error al consultar la API")
        return []

    data = response.json()
    registros = data[1]

    resultados = []

    for item in registros:
        if item["country"]["value"] == "Argentina" and item["value"] is not None:
            resultados.append((item["date"], item["value"]))

    resultados.sort(reverse=True)
    return resultados


def main():
    datos = obtener_gini_argentina()

    if not datos:
        print("No se encontraron datos")
        return

    print("Índice GINI - Argentina\n")

    for year, value in datos:
        resultado = lib.procesar(value)
        print(f"Año: {year} | GINI original: {value} | Procesado: {resultado}")


if __name__ == "__main__":
    main()
