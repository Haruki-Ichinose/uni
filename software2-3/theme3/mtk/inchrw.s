.global inbyte
.include "equdefs.inc"
	
.text
.even

inbyte:
	link.w	%a6, #-4
	movem.l %d1-%d3/%a0, -(%sp) 

inbyte_loop:
	move.l  #SYSCALL_NUM_GETSTRING, %d0
	move.l  %sp, %a0
   	adda.l  #28, %a0
   	move.l  (%a0),%d1  

   	move.l  %a6, %d2
   	sub.l   #1,  %d2            
    	move.l  #1, %d3              
    	trap    #0                        
   	
    	cmpi.l  #0, %d0         
    	beq     inbyte_loop     
    	move.b	'A', LED0     
    	
    	move.b	'0', LED1 
    	
    	move.b  -1(%a6), %d0
    	
    	movem.l (%sp)+, %d1-%d3/%a0
    	unlk    %a6
	rts
