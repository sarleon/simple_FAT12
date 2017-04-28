        SECTION .data
        color_red       db  1Bh, '[31;1m', 0
        color_green       db  1Bh, '[32;1m', 0
        color_yellow     db  1Bh, '[33] ;1m', 0
        color_blue       db  1Bh, '[34] ;1m', 0
        color_purple       db  1Bh, '[35] ;1m', 0
        color_darkgreen       db  1Bh, '[36] ;1m', 0
        color_white       db  1Bh, '[37] ;1m', 0

        SECTION .text
        global  my_print
        ;; void myprint(char* str,int color);
        ;; color:0 red 1 green
my_print:
        cmp     rsi , 0
        jnz     print_green

print_red:
        mov     rax , color_red
        call    sprint
        jmp     print_str
print_green:
        mov     rax , color_green
        call    sprint
        jmp     print_str
print_str:
        mov     rax , rdi
        call    sprint
        xor     rsi , rsi
        xor     rdi , rdi
        ret






;;; ; input and output functions
slen:
        push    rdi
        mov     rdi , rax


nextchar:
        cmp     byte[rax] , 0
        jz      finished
        inc     rax
        jmp     nextchar

finished:
        sub     rax , rdi
        pop     rdi
        ret


sprint:
        push    rcx
        push    rdx
        push    rsi
        push    rdi
        push    rax

        call    slen
        mov     rdx , rax
        pop     rax

        mov     rsi , rax
        mov     rax , 1
        mov     rdi , 1
        syscall


        pop     rdi
        pop     rsi
        pop     rdx
        pop     rcx
        ret

quit:
        mov     rdi , 0
        mov     rax , 60
        syscall
        ret
sprintLF:
        call    sprint

        push    rax
        mov     rax  , 0Ah
        push    rax
        mov     rax , rsp
        call    sprint
        pop     rax
        pop     rax
        ret
iprint:                         ;parameter in rax
        push    rax
        push    rcx
        push    rdx
        push    rsi
        mov     rcx , 0
divideLoop:
        inc     rcx     ;count byte to print
        mov     rdx , 0 ;empty  rdx
        mov     rsi , 10 ;mov rsi to 10
        idiv    rsi     ;divide rax by rsi,store the result to rax , the remainder to rdx
        add     rdx , 48
        push    rdx
        cmp     rax , 0 ;if the quotient is 0 ,the loop is stop and turn to print
        jnz     divideLoop
printLoop:
        dec     rcx
        mov     rax , rsp
        call    sprint  ;print
        pop     rax     ;parameter pop out stack
        cmp     rcx , 0
        jnz     printLoop

        pop     rsi
        pop     rdx
        pop     rcx
        pop     rax

        ret

iprintLF:
        call    iprint

        push    rax
        mov     rax , 0x20
        push    rax
        mov     rax , rsp
        call    sprint
        pop     rax
        pop     rax
        ret
