# labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  # What is the effect of the assembler directive .global? Why is the directive particularly 
  # important in this assignment? The teachers will help you with this if necessary.
  	# .global allows other files to access the function. This is important in this assignment
  	# since both time2string and delay should be accessible externally. 

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
hexasc:
	# The 4 least significant bits specify a number, from 0 through 15. 
	# All other bits in register $a0 can have any value and must be ignored.
	
	# 0xF = 00001111 in combination with "andi" ensures the 4 most significant bits are excluded:
	andi 	$a0,$a0,0xF
	
	# 0-9 is 48-57 in decimal and A-F is 65-70
	li	$t1,0x9		# = 9
	li	$v0,0x30	# = 48
	
	# check if less than or equal to 9
	ble	$a0,$t1,numberRange
	nop	
	
	addi	$v0,$v0,0x7  # Add 7 to jump to ASCII "letter range" ($v0 is already at 48)
	
	
numberRange:
	add	$v0,$a0,$v0
	jr	$ra	# return
	
.global delay
delay:
	move $t0, $a0
	while:
		ble $t0, 0, end
		nop
		addi $t0, $t0, -1
		addi $t1, $0, 7711
		li $t2, 0
	
		for:
		
			addi $t2, $t2, 1
			beq $t2, $t1, whileLoop
			nop
			addi $t2, $t2, 1
			j for
			nop
			
		whileLoop:
			j while
			nop
	end:
	jr $ra
	nop
  	
.global time2string
time2string:
	# Parameters (two): Register $a0 contains the address of an area in memory, suitably large for the 
	# output from time2string. The 16 least significant bits of register $a1 contains time-info, organized
	# as four NBCD-coded digits of 4 bits each. All other bits in register $a1 can have any value and 
	# must be ignored. Example: register $a0 can contain the address 0x100100017, and register $a1 can 
	# contain the value 0x00001653. 
	
	PUSH 	$ra
	PUSH	$s1
	andi	$a1,$a1,0xFFFF	# Exclude everything but 16 least significant bits
	move 	$s1, $a0
	
	# First digit (X0:00)
	andi 	$t0, $a1, 0xF000
	PUSH 	$ra
	srl 	$t0, $t0, 12
	move 	$a0, $t0
	jal 	hexasc
	nop
	sb 	$v0, 0($s1)
	POP 	$ra
	
	# Second digit (0X:00)
	andi 	$t0, $a1, 0x0F00
	PUSH 	$ra
	srl 	$t0, $t0, 8
	move 	$a0, $t0
	jal 	hexasc
	nop
	sb 	$v0, 1($s1)
	POP 	$ra
	
	# Colon
	addi 	$t2, $0, 0x3A
	sb	$t2, 2($s1)

	# Third digit (00:X0)
	andi 	$t0, $a1, 0x00F0
	PUSH 	$ra
	srl 	$t0, $t0, 4
	move 	$a0, $t0
	jal 	hexasc
	nop
	sb 	$v0, 3($s1)
	POP 	$ra
	
	# Fourth digit (00:0X)
	andi 	$t0, $a1, 0x000F
	PUSH 	$ra
	move 	$a0, $t0
	jal 	hexasc
	nop
	sb 	$v0, 4($s1)
	POP 	$ra
	
	# Null bit
	addi 	$t0, $0, 0x00
	sb	$t0, 5($s1)
	
	move 	$s1, $a0
	POP	$s1
	POP 	$ra
	
	jr 	$ra
	nop
