# procesar.s  —  función procesar(float x) en AT&T syntax
# Convención System V AMD64 ABI:
#   %xmm0 = 1er argumento (float x)
#   %eax = valor de retorno (int de 32 bits)

.section .text
.globl  procesar       # exportar símbolo para el linker
.type   procesar, @function

procesar:
    # Prólogo — no necesitamos frame propio para esta función simple
    cvttss2si   %xmm0, %eax     # truncar float a int (32 bits)
    addl        $1   , %eax     # eax = a + 1
    ret                         # retorna

.size   procesar, .-procesar
