BITS        64
SECTION     .text

GLOBAL      _Z2sid


;xmm2 - x
;xmm1 - x^4
;
_Z2sid:
    movq xmm1, xmm0
    mulpd xmm1, xmm1
    mulpd xmm1, xmm1    ;xmm1 = x^4
    movlhps xmm1, xmm1  ;xmm1 = x^4 | x^4
    movq xmm2, xmm0     ;xmm2 = x
    mulpd xmm2, xmm0
    mulpd xmm2, xmm0   ;xmm2 = _ | x^3
    shufpd xmm2, xmm0, 0        ;xmm2 = x | x^3
    shufpd xmm2, xmm2, 1        ;xmm2 = x^3 | x
    xorpd xmm4, xmm4
    xor rsi, rsi
    .loop:
    cmp rsi, 6
    jae .end
    movapd xmm3, xmm2
    lea rdi, [table + 8 *rsi]
    mulpd xmm3, [table + 8 * rsi]       ;xmm3 = x^k/x*k! w obu
    addpd xmm4, xmm3
    mulpd xmm2, xmm1    ;xmm2 = x^2*k + 1 + 2 + 4 | x^2*k + 1 + 4
    inc rsi
    inc rsi
    jmp .loop
    .end
    
    movhlps xmm2, xmm4
    movq xmm0, xmm4
    subpd xmm0, xmm2
    ret


align 16
table:
    dq 0x3ff0000000000000,
    dq 0x3fac71c71c71c71c,
    dq 0x3f5b4e81b4e81b4f,
    dq 0x3efdb8b6f9266dd4,
    dq 0x3e948c5892f7cd83,
    dq 0x3e2390327a26bdbd,
    dq 0x3dab2a2edd1e100c,
    dq 0x3d2cb3264bf2e687,
    dq 0x3ca7d570699739af,
    dq 0x3c1feccf9f2a88fe,
    dq 0x3b919b179783c732,
    dq 0x3b0043f7b43bfa81,
    dq 0x3a6989fbfc45992a,
    dq 0x39d13f39eebbbcd2,
    dq 0x39343ff8e04a5033,
    dq 0x3894dbb06adacc1d
