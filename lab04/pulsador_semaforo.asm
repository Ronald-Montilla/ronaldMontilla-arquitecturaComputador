.data
	estado: .byte 0
	men1: .asciiz "Semaforo en verde, esperando pulsador\n"
	men2: .asciiz "Pulsador activado: el semaforo cambiara a amarillo en: "
	men3: .asciiz "Semaforo en amarillo, cambiara a rojo en: "
	men4: .asciiz "Semaforo en rojo, cambiara a verde en: "
	menAux: .asciiz " Segundos\n"
	timer_1: .word 2000
	timer_2: .word 2000
	timer_3: .word 2000
.text
.globl main
main:
	# Habilito las interrupciones por teclado:
	li $t0, 0xffff0000
	lw $t1, 0($t0)
	li $t2, 2
	or $t1, $t1, $t2
	sw $t1, 0($t0)
	jal programa
programa:
	li $s0, -1
	la $s1, estado
	bucleImprimir:
	lb $t1, estado
	bne $s0, $t1, imprimir
	j bucleImprimir
imprimir:
	lb $s0, estado
	lb $t0, estado
	li $t1, 1
	beq $s0, $t1, pulsador
	li $t1, 2
	beq $s0,$t1, amarillo
	li $t1, 3
	beq $s0, $t1, rojo
	# verde:
	# Actualizo el estado y muestro el mensaje correspondiente:
	la $a0, men1
	li $v0, 4
	syscall
	# Activamos las interrupciones pertinentes:
	mfc0 $t0, $12
	li $t1, 257
	or $t0, $t0, $t1
	mtc0 $t0, $12
	j bucleImprimir
pulsador:
	# Actualizo el estado y muestro el mensaje correspondiente:
	li $t0, 2
	sb $t0, estado
	la $a0, men2
	li $v0, 4
	syscall
	lw $t0, timer_1 # Cronometro a amarillo.
	li $v0, 30
	syscall
	move $t1, $a0 # Tiempo inicial
	# Muestro los segundos del cronometro:
	li $v0, 1
	move $a0, $t0
	div $a0, $a0, 1000
	syscall
	li $v0, 4
	la $a0, menAux
	syscall
	buclePulsador:
		li $v0, 30
		syscall
		move $t2, $a0 # Tiempo actual
		sub $t3, $t2, $t1
		slt $t3, $t3, $t0
		bne $t3, $zero, buclePulsador
		j bucleImprimir	
amarillo:
	# Actualizo el estado y muestro el mensaje correspondiente:
	li $t0, 3
	sb $t0, estado
	la $a0, men3
	li $v0, 4
	syscall
	lw $t0, timer_2 # Cronometro a rojo.
	li $v0, 30
	syscall
	move $t1, $a0 # Tiempo inicial
	# Muestro los segundos del cronometro:
	li $v0, 1
	move $a0, $t0
	div $a0, $a0, 1000
	syscall
	li $v0, 4
	la $a0, menAux
	syscall
	bucleAmarillo:
		li $v0, 30
		syscall
		move $t2, $a0 # Tiempo actual
		sub $t3, $t2, $t1
		slt $t3, $t3, $t0
		bne $t3, $zero, bucleAmarillo
		j bucleImprimir
rojo:
	# Actualizo el estado y muestro el mensaje correspondiente:
	li $t0, 0
	sb $t0, estado
	la $a0, men4
	li $v0, 4
	syscall
	lw $t0, timer_3 # Cronometro a verde.
	li $v0, 30
	syscall
	move $t1, $a0 # Tiempo inicial
	# Muestro los segundos del cronometro:
	li $v0, 1
	move $a0, $t0
	div $a0, $a0, 1000
	syscall
	li $v0, 4
	la $a0, menAux
	syscall
	bucleRojo:
		li $v0, 30
		syscall
		move $t2, $a0 # Tiempo actual
		sub $t3, $t2, $t1
		slt $t3, $t3, $t0
		bne $t3, $zero, bucleRojo
		li $t0, 0
		sb $t0, estado
		j bucleImprimir
#--------------------------------------------------------------------------
.ktext 0x80000180 # Se produce una interrupcion
	addi $sp, $sp, -20
	sw $t2, 0($sp)
	sw $t1, 4($sp)
	sw $t0, 8($sp)
	sw $a0, 12($sp)
	sw $v0, 16($sp)
	# Si los bits 2-6 del registro cause son 0, significa que fue una interrupcion externa.
	mfc0 $t0, $13
	li $t1, 124
	and $t1, $t0, $t1
	bne $t1, $zero, finInterrupcion
	# Verifico si la interrupcion fue provocada por el teclado.
	li $t0, 0xffff0000
	lw $t0, 0($t0)
	li $t1, 1
	and $t0, $t0, $t1
	bne $t0, $t1, finInterrupcion
	# Verifico si el caracter es una s.
	li $t0, 0xffff0004
	lw $t0, 0($t0)
	li $t1, 115
	bne $t0, $t1, finInterrupcion
	#Actualizo el estado:
	li $t2, 1
	sb $t2, estado
	# Desactivo las interrupciones por teclado:
	mfc0 $t0, $12
	li $t1, 256
	nor $t1, $t1, $zero
	and $t0, $t0, $t1
	mtc0 $t0, $12
	finInterrupcion:
		lw $v0, 16($sp)
		lw $a0, 12($sp)
		lw $t0, 8($sp)
		lw $t1, 4($sp)
		lw $t2, 0($sp)
		addi $sp, $sp, 20
		eret
