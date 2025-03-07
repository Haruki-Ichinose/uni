.global inbyte
.include "equdefs.inc"
	
.text
.even

inbyte:
	movem.l %d1-%d3/%a6, -(%sp) 
	link.w	%a6, #-4
	
inbyte_loop:
	move.l  #SYSCALL_NUM_GETSTRING, %d0
   	move.l  #0, %d1
   	move.l  %a6, %d2
   	sub.l   #1,  %d2                 
    	move.l  #1, %d3                  
    	trap    #0                        

    	cmpi.l   #0, %d0                   
    	beq     inbyte_loop      
    	
    	move.b  -1(%a6), %d0
	move.b	%d0, LED0
	unlk    %a6
    	movem.l (%sp)+, %d1-%d3/%a6
    	
	rts
	


	
inkey:
	movem.l %d1-%d3, -(%sp)
	
inkey_loop:	
	move.l  %d0, %d1
	move.l  #SYSCALL_NUM_GETSTRING, %d0
   	move.l  #inchrw_buf, %d2                 
    	move.l  #1, %d3                  
    	trap    #0                        

    	cmpi.l   #0, %d0                   
    	beq     no_input
    	
    	move.b  inchrw_buf, %d0
	andi.l	#0xff, %d0	
	
no_input:
	move.l  #-1, %d0  
	
input_end:
	movem.l (%sp)+, %d1-%d3
	rts





.section .bss
.even
inchrw_buf: .ds.b 1
	.even
	
