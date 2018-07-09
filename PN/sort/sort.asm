BITS        64
SECTION     .text

GLOBAL      sort


;rax -> i
;rdx -> j
;rcx -> x
;r10 -> temp
partition:
    mov r11, [rdi + 8*rsi - 8]    ;moge to zrobic wczesniej
    mov r8, rsi
    shr r8, 1
    lea r8, [rdi + 8*r8]        ;r8 -> mid
    mov rcx, [r8]
    mov [r8], r11
    mov [rdi + 8*rsi - 8], rcx
    ;rcx = x i zamiana srodkowego i ostatniego
    xor rax, rax
    lea rdx, [rsi - 2]  
    ;i = 0 i j = size - 1
    
    .loop:
    cmp rax, rdx
    jg .end
        .rax_loop:
            cmp rax, rdx
            jg .end
            cmp [rdi + 8*rax], rcx
            jge .rax_end
            inc rax
            jmp .rax_loop
        .rax_end:
        .rdx_loop:
            cmp rax, rdx
            jg .end
            cmp [rdi + 8*rdx], rcx
            jle .rdx_end
            dec rdx
            jmp .rdx_loop
        .rdx_end:
        mov r8, [rdi + 8*rax]
        mov r9, [rdi + 8*rdx]
        mov [rdi + 8*rax], r9
        mov [rdi + 8*rdx], r8
        inc rax
        dec rdx  
        jmp .loop
    .end:
    mov r8, [rdi + 8*rax]
    mov r9, [rdi + 8*rsi - 8]
    mov [rdi + 8*rax], r9
    mov [rdi +8*rsi - 8], r8
    ret


;rdi - data
;rsi - size
q_sort:
    cmp rsi, 1
    jbe .end
    push r12    ;pomocniczy data
    push r13    ;pomocniczy size
    push r14    ;pomocniczy result of partition
    mov r12, rdi
    mov r13, rsi
    
    call partition
    mov r14, rax

    mov rsi, rax
    call sort
    
    lea rdi, [r12 + 8*r14 + 8] 
    mov rsi, r13
    sub rsi, r14
    dec rsi
    call sort

    pop r14
    pop r13
    pop r12
    .end:
    ret

i_sort:
    mov rdx, 1
    .m_loop:
    cmp rdx, rsi
    jae .end_m
        mov rcx, [rdi + 8*rdx]
        mov r8, rdx
        dec r8
        .s_loop:
        cmp r8, 0
        jl .end_s
        cmp [rdi + 8*r8], rcx
        jle .end_s
            mov r9, [rdi + 8*r8]
            mov [rdi + 8*(r8 + 1)], r9
            dec r8
            jmp .s_loop
        .end_s:

        mov [rdi + 8*(r8 + 1)], rcx

        inc rdx
        jmp .m_loop
    .end_m:
    ret

sort:
    call q_sort
    call i_sort
    ret
