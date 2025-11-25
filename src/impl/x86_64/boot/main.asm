global start
section .text
bits 32

start:
    mov edi, 0xb8000     ; VGA text buffer

    mov ax, 0x2F49       ; 'I'
    stosw
    mov ax, 0x2F20       ; ' '
    stosw
    mov ax, 0x2F4C       ; 'L'
    stosw
    mov ax, 0x2F4F       ; 'O'
    stosw
    mov ax, 0x2F56       ; 'V'
    stosw
    mov ax, 0x2F45       ; 'E'
    stosw
    mov ax, 0x2F20       ; ' '
    stosw
    mov ax, 0x2F41       ; 'A'
    stosw
    mov ax, 0x2F56       ; 'V'
    stosw
    mov ax, 0x2F49       ; 'I'
    stosw
    mov ax, 0x2F53       ; 'S'
    stosw
    mov ax, 0x2F48       ; 'H'
    stosw
    mov ax, 0x2F4B       ; 'K'
    stosw
    mov ax, 0x2F41       ; 'A'
    stosw
    mov ax, 0x2F7E       ; '~'
    stosw
    mov ax, 0x2F7E       ; '~'
    stosw

    hlt
