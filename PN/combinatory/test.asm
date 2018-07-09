BITS	64
EXTERN	putchar
EXTERN	K, S
GLOBAL	main

SECTION	.text

; the 'write 0' combinator
W0:
	mov		dil,	'0'
	call	putchar
	ret

; the 'write 1' combinator
W1:
	mov		dil,	'1'
	call	putchar
	ret

; this should print '01011011'
print:
	push	K
	push	W1
	push	W0
	push	K
	push	W1
	push	W0
	push	S
	push	S
	push	S
	push	S
	push	S
	push	S
	ret

; wrap it up
main:
	call	print
	mov		dil,	10
	call	putchar
	xor		al,		al
	ret

