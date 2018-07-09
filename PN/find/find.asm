BITS        64
SECTION     .text

GLOBAL      _start


    

not_dots:
    mov al, [rdi]
    cmp al, 46
    jne .end
    mov al, [rdi + 1]
    cmp al, 46
    jne .zero
    mov al, [rdi + 2]
    .zero:
    cmp al, 0
    jne .end
    xor rax, rax
    ret
    .end:
    xor rax, rax
    inc rax
    ret



no_slash:
    xor cl, cl
    mov r11, rdi
    call length
    mov cl, [r11 + rax - 1]
    cmp cl, 47
    jne .no_slash
    xor rax, rax
    ret
    .no_slash:
    mov rax, 1
    ret

;rdi - string address
;rcx - counter
;al  - null pattern
;-------length -> int-------
length:
    xor rcx, rcx        
    not ecx             ;ecx = -1 || ecx = 2^33 - 1
    xor al, al
    cld                 ;df = 0
    repne scasb
    not ecx
    dec ecx
    mov rax, rcx
    ret

;-------exit -> void-------
exit:
    xor rdi, rdi
    mov rax, _exit_
    syscall

;1)rdi - string address/file descriptor
;rdx - string length
;rsi - string address
;-------print -> void-------
print:
    push rdi
    call length
    mov rdx, rax
    mov rax, _write_
    pop rsi
    mov rdi, 1
    syscall
    ret

;1)rdi - string address/file descriptor
;-------println -> void-------
println:
    call print
    mov rdi, newline
    call print
    ret

;1)rdi - first string
;2)rsi - second string
;3)rdx - output string
;r8 - input iter
;r9 - output iter
;-------str_concat -> void-------
str_concat:
    xor r8, r8
    xor r9, r9
    .first_str:
        mov cl, [rdi + r8]
        test cl, cl
    jz .first_end
        mov [rdx + r9], cl
        inc r9
        inc r8
        jmp .first_str
    .first_end:
    xor r8, r8
    .second_str:
        mov cl, [rsi + r8]
        test cl, cl
    jz .second_end
        mov [rdx + r9], cl
        inc r9
        inc r8
        jmp .second_str
    .second_end:
    xor cl, cl
    mov [rdx + r9], cl
    ret

;1)rdi - dir name
;rbx - dir name
;r12 - fd
;r13 - name
;r14 - current dirent
;r15 - return value of getdents
;rsp - memory for getdents
find:
    push rbx
    push r12
    push r13
    push r14
    push r15

    sub rsp, STRING_SIZE
    mov r13, rsp        ;allocation for name[4096]
    sub rsp, STRING_SIZE;allocation memory for getdents
    mov rbx, rdi        ;rbx = dir_name
    ;^variables declaration^

    mov rax, _open_
    mov rsi, DIR_FLAGS
    syscall             ;open(dir_name, O_NOFOLLOW | O_DIRECTORY)
    test rax, rax
    js .end             ;fd < 0 => return error
    mov r12, rax        ;r12 = fd
    
    .gd_loop:
    mov rdi, r12
    mov rsi, rsp
    mov rdx, STRING_SIZE
    mov rax, _getdents_
    syscall             ;getdents
    test rax, rax
    jz .gd_end          ;bytes == 0 => break

    mov r15, rax        ;remember return value
    mov r14, rsp        ;r14 points to first dirent
    .inner:
        mov rcx, r14
        sub rcx, rsp
        cmp rcx, r15
        jge .inner_end         ;if(r14 - rsp < r15) break;
        
        mov rdi, rbx
        call no_slash
        test rax, rax
        jz .yes_slash
            mov r11, slash
            jmp .concat
        .yes_slash:
            mov r11, empty
        .concat:
        mov rdi, rbx
        mov rsi, r11
        mov rdx, r13
        call str_concat
        .slash_before:
        mov rdi, r13
        lea rsi, [r14 + 18]
        mov rdx, r13
        call str_concat         ;r13 -> full path
        lea rdi, [r14 + 18]     ;rdi -> file name
        xor r8, r8
        mov r8w, [r14 + 16]
        add r14, r8
                                ;here crr, name are set correctly, 
        call not_dots
        test rax, rax
        jz .inner                ;dots        
        mov rdi, r13
        call println
        mov rdi, r13
        call find
        jmp .inner
    .inner_end:
    jmp .gd_loop
    .gd_end:
    mov rdi, r12
    mov rax, 3
    syscall             ;close(fd)
    
    xor rax, rax        ;everything is fine
    .end:
    add rsp, STRING_SIZE
    add rsp, STRING_SIZE
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

;argc - rbx
;r12 - iter
_start:
    mov rcx, [rsp]
    cmp rcx, 1
    jne .normal
    mov rdi, no_arguments
    call println
    jmp .exit
    .normal:
    mov rdi, [rsp + 16]
    mov rsi, DIR_FLAGS
    mov rax, _open_
    syscall
    test rax, rax
    jns .dir
    mov rdi, [rsp + 16]  
    mov rsi, O_RDONLY
    mov rax, _open_
    syscall
    mov rbx, rax
    cmp rbx, ENOTDIR
    jne .not_slash
        mov rdi, prefix
        call print
        mov rdi, [rsp + 16]
        call print
        mov rdi, not_dir_suffix
        call println
        jmp .exit
    .not_slash:
    cmp rbx, ENOENT
    jne .normal_file
        mov rdi, prefix
        call print
        mov rdi, [rsp + 16]
        call print
        mov rdi, no_file_suffix
        call println
        jmp .exit
    .normal_file:
        mov rdi, [rsp + 16]
        call println
        jmp .exit
    .dir:
        mov rdi, rax
        mov rax, _close_
        syscall
        mov rdi, [rsp + 16]
        call println
        mov rdi, [rsp + 16]
        call find
    .exit:
    
    call exit
    ; no 'ret' here, not a function!



SECTION .data

;syscalls
_write_ equ 1
_read_ equ 0
_exit_ equ 60
_open_ equ 2
_getdents_ equ 78
_close_    equ 3
;open flags
DIR_FLAGS equ 196608
O_RDONLY equ 0
;errors
EACCES equ -13
ENOTDIR equ -20
ELOOP equ -40
ENOENT equ -2
;strings
newline db 10, 0
empty db 0
slash db 47, 0
same_dir db ".", 0
prev_dir db "..", 0
prefix db "find: '", 0
no_file_suffix db "': No such file or directory", 0
not_dir_suffix db "': Not a directory", 0
no_arguments db "I need arguments!!", 0
test_a db "abcdefghij", 0
test_b db "klmnopqrst", 0
;const
STRING_SIZE equ 4097


