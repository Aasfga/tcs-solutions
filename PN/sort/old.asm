BITS        64
SECTION     .text

;rdx -> i
;rax -> m
;r8 -> j
;r10, r9 -> pomocnicze
GLOBAL      sort


;rcx -> x
;r8 -> tmp
;rax -> i
;r9 -> j
;rdi -> data
partition:
    mov r8, rsi
    shr r8, 1
    lea r8, [rdi + 8*r8] ;r8 = mid

    mov rcx, [r8]
    xchg rcx, [rdi + 8*(rsi - 1)]
    xchg [r8], rcx ;swap i ustalenie wartości

    xor rax, rax
    dec rax ;i = -1
    xor r9, r9 ;j = 0

    .loop:
    cmp r9, rsi
    jae .end_loop
        cmp [rdi + 8*r9], rcx
        jg .end_if
            inc rax
            mov r8, [rdi + 8*r9]
            xchg [rdi + 8*rax], r8
            mov [rdi + 8*r9], r8
        .end_if:
        inc r9
        jmp .loop
    .end_loop:
    ret


same_check:
    mov rcx, [rdi]    
    xor r8, r8
        
    .loop:
    cmp r8, rsi
    jge .true
    cmp rcx, [rdi + 8*r8]
    jne .false
    inc r8
    jmp .loop

    .false:
        mov rax, 0
        ret
    .true:
        mov rax, 1
        ret


;rsi -> count
;r8 -> count
;rdi -> data
;r9 -> data
;rax -> s
q_sort:
    cmp rsi, 1
    jbe .end
    
    call same_check
    cmp rax, 1
    je .end


    push rdi
    push rsi
    call partition
    push rax
    mov rsi, rax
    call sort
    pop rax
    pop rsi
    pop rdi
    lea rdi, [rdi +8*(rax + 1)]
    sub rsi, rax
    dec rsi
    call sort
    
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
