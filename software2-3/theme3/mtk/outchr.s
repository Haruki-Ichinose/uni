.global outbyte
.include "equdefs.inc"

.text
.even

outbyte:
	movem.l	%d1-%d3/%a0, -(%sp)
	
outbyte_loop:
	move.l  #SYSCALL_NUM_PUTSTRING, %d0
	
	movea.l	%sp, %a0
	adda.l	#20, %a0
	move.l (%a0), %d1
	
   	move.l  %sp, %d2                 
   	addi.l  #27, %d2
   	
    	move.l  #1, %d3                  
    	trap    #0    

	cmpi.l   #0, %d0
	beq	outbyte_loop

	movem.l  (%sp)+, %d1-%d3/%a0
	rts

