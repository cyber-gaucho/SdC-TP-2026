.code16
    mov $msg, %si
    mov $0x0e, %ah
loop:
    lodsb
    or %al, %al
    jz halt
    int $0x10
    jmp loop
halt:
    hlt
msg:
    .ascii " _______________\r\n"
    .ascii "/ Hello world!  \\\r\n"
    .ascii "\\ mate --refill /\r\n"
    .ascii " ---------------\r\n"
    .ascii "        \\   ^__^\r\n"
    .ascii "         \\  (oo)\\_______\r\n"
    .ascii "            (__)\\       )\\/\\\r\n"
    .ascii "                ||----w |\r\n"
    .asciz "                ||     ||\r\n"
