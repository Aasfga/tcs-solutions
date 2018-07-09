BITS	64
GLOBAL	K, S
EXTERN _Z7mallocxm

SECTION	.text



boring_start:
    mov r8, qword 1
    push r8
    mov r8, qword 2
    push r8
    ret
boring_end:
    dq boring_end - boring_start

K:
    pop r8
    pop r9
    jmp r8

S:
    pop r8; x
    pop r9; y
    pop r10; z
    mov rdi, [boring_end]
    call _Z7mallocxm
    mov rcx, [boring_end]
    mov rdi, rax
    mov rsi, boring_start
    rep movsb 
    mov [rax + 2], r10
    mov [rax + 14], r9
    push rax
    push r10
    push r8
    ret

