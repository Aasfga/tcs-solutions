BITS 64

SECTION .text
GLOBAL expr_eval:function
EXTERN malloc

;rdi - *result (przekazuje go dalej)
;rsi - expr
;rdx - data

switch:
    dq handle_const - switch
    dq handle_var - switch
    dq handle_plus - switch
    dq handle_minus - switch
    dq handle_add - switch
    dq handle_sub - switch
    dq handle_min - switch
    dq handle_max - switch
    

;rdi - result
;rsi - expr
;rdx - data
bin_expr:
    push rbx
    push r12
    push r13
    mov r12, rdi        ;r12 - result
    mov rbx, rsi        ;rbx - expr
    mov r13, rdx        ;r13 - data
    sub rsp, 16
    ;first call 
    lea rdi, [rsp + 8]
    mov rsi, [rbx + 8]
    mov rdx, r13
    call expr_eval_one
    ;second call
    lea rdi, [rsp]
    mov rsi, [rbx + 16]
    mov rdx, r13
    call expr_eval_one

    xor r8, r8
    xor r9, r9
    mov r8, [rsp + 8]      ;first result
    mov r9, [rsp]  ;second result

    add rsp, 16
    pop r13
    pop r12
    pop rbx
    pop rdi     ;return pointer
    push r8     ;stack: return pointer
    push r9     ;       second var
    push rdi    ;       first var
    ret
    
;rdi - result
;rsi - expr
;rdx - data
expr_eval_one:
    push rbx
    push r12    
    push r13
    push r14
    mov rbx, rsi        ;rbx - expr
    mov r12, rdi        ;r12 - result
    mov r13, rdx        ;r13 - data

    xor r8, r8          ;first var
    xor r9, r9          ;second var
    
    mov r8d, [rbx]
    lea rcx, [rel switch]
    add rcx, [rcx + 8*r8]
    jmp rcx

    handle_const:
        mov r8d, [rbx + 8]
        mov [r12], r8d
        jmp end_expr_eval
    handle_var:
        mov r8b, [rbx + 8]
        mov r8d, [r13 + 4*r8]
        mov [r12], r8d
        jmp end_expr_eval
    handle_plus:
        mov rsi, [rbx + 8]
        call expr_eval_one
        jmp end_expr_eval
    handle_minus:
        mov rsi, [rbx + 8]
        call expr_eval_one
        neg dword [r12]
        jmp end_expr_eval
    handle_add:
        call bin_expr
        pop r9
        pop r8
        add r8d, r9d
        mov [r12], r8d
        jmp end_expr_eval
    handle_sub:
        call bin_expr
        pop r9
        pop r8
        sub r8d, r9d
        mov [r12], r8d
        jmp end_expr_eval
    handle_min:
        call bin_expr
        pop r9
        pop r8
        cmp r8d, r9d
        jl .less
            mov [r12], r9d
            jmp end_expr_eval
        .less:
            mov [r12], r8d
            jmp end_expr_eval
    handle_max:
        call bin_expr
        pop r9
        pop r8
        cmp r8d, r9d
        jl .less
            mov [r12], r8d
            jmp end_expr_eval
        .less:
            mov [r12], r9d
            jmp end_expr_eval
    end_expr_eval:
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

; rdi -- struct expr * expr
; rsi -- rows
; rdx -- cols
; rcx -- int const * data
expr_eval:
    push rbx
    push r12
    push r13
    push r14
    push r15
    mov rbx, rdi        ;rbx - expr
    mov r12, rsi        ;r12 - rows     
    mov r13, rdx        ;r13 - cols
    mov r14, rcx        ;r14 - data
    lea rdi, [4*rsi]
    call malloc WRT ..plt
    mov r15, rax        ;r15 - results
    
    xor r11, r11          ;temp - stores i*cols
    xor r10, r10          ;r8 - i
    .loop:
        cmp r10, r12
        jae .end
        lea rdi, [r15 + 4*r10]
        mov rsi, rbx
        lea rdx, [r14 + 4 * r11]
        call expr_eval_one
        add r11, r13
        inc r10
    jmp .loop
    

    
    .end:
    mov rax, r15
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

