#include <stdio.h>
#include "procesar.h"

int main() {
    float input;

    printf("Ingrese un número: ");
    if(scanf("%f", &input) != 1) {
        printf("Error al leer el valor\n");
        return 1;
    }

    int resultado = procesar(input);

    printf("Resultado: %d\n", resultado);

    return 0;
}