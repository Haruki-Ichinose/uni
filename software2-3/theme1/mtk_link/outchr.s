.global outbyte
.include "equdefs.inc"

.text
.even

outbyte:
	movem.l	%d1-%d3, -(%sp)
	
outbyte_loop:
	move.l  #SYSCALL_NUM_PUTSTRING, %d0
   	move.l  #0, %d1
   	move.l  %sp, %d2                 
   	addi.l  #19, %d2
    	move.l  #1, %d3                  
    	trap    #0    

	cmpi.l   #0, %d0
	beq	outbyte_loop

	movem.l  (%sp)+, %d1-%d3
	rts
	
	

