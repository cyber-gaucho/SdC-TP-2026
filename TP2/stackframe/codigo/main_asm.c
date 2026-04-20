#include <stdio.h>

int procesar(float x);  // prototipo ASM

int main() {
    float valores[] = {3.7, 2.1, -1.9, 0.0};

    for (int i = 0; i < 4; i++) {
        int r = procesar(valores[i]);
        printf("x = %.2f → resultado = %d\n", valores[i], r);
    }

    float input;

    printf("\nIngrese un número: ");
    if(scanf("%f", &input) != 1) {
        printf("Error al leer el valor\n");
        return 1;
    }

    int resultado = procesar(input);

    printf("Resultado: %d\n", resultado);

    return 0;
}
