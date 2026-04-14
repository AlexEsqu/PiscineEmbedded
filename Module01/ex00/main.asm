	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
	.type	delay_500ms, @function
delay_500ms:
	push r28
	push r29
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
/* stack size = 6 */
.L__stack_usage = 6
	ldi r24,lo8(32)
	ldi r25,lo8(-95)
	ldi r26,lo8(7)
	ldi r27,0
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	rjmp .L2
.L3:
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r26,Y+3
	ldd r27,Y+4
	sbiw r24,1
	sbc r26,__zero_reg__
	sbc r27,__zero_reg__
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
.L2:
	ldd r24,Y+1
	ldd r25,Y+2
	ldd r26,Y+3
	ldd r27,Y+4
	sbiw r24,0
	cpc r26,__zero_reg__
	cpc r27,__zero_reg__
	brne .L3
	nop
	nop
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	delay_500ms, .-delay_500ms
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	ldi r24,lo8(36)
	ldi r25,0
	movw r30,r24
	ld r18,Z
	ldi r24,lo8(36)
	ldi r25,0
	ori r18,lo8(2)
	movw r30,r24
	st Z,r18
.L5:
	ldi r24,lo8(37)
	ldi r25,0
	movw r30,r24
	ld r19,Z
	ldi r24,lo8(37)
	ldi r25,0
	ldi r18,lo8(2)
	eor r18,r19
	movw r30,r24
	st Z,r18
	call delay_500ms
	nop
	rjmp .L5
	.size	main, .-main
	.ident	"GCC: (Fedora 15.2.0-1.fc42) 15.2.0"
