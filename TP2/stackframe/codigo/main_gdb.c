#include <stdio.h>

// Equivalente en C a la instruccion cvttss2si + add 1 del ASM
// Usamos C puro para que GDB muestre el stack frame completo
int procesar_gdb(float x) {
    int truncado = (int)x;   // cvttss2si %xmm0, %eax
    int resultado = truncado + 1; // addl $1, %eax
    return resultado;
}

int main() {
    // Valores GINI Argentina (Banco Mundial)
    float gini_2011 = 42.3f;
    float gini_2014 = 40.9f;
    float gini_2017 = 40.1f;

    int r1 = procesar_gdb(gini_2011); // <- breakpoint aqui
    int r2 = procesar_gdb(gini_2014);
    int r3 = procesar_gdb(gini_2017);

    printf("GINI 2011: %.1f -> %d\n", gini_2011, r1);
    printf("GINI 2014: %.1f -> %d\n", gini_2014, r2);
    printf("GINI 2017: %.1f -> %d\n", gini_2017, r3);

    return 0;
}
