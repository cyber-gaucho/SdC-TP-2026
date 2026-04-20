// declara la función ASM
int procesar(float x);

// wrapper que será visible para Python
int procesar_wrapper(float x) {
    return procesar(x);
}