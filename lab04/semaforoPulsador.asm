# !--- run speed debe estar en 30 inst/sec ---!
.data
	estado: .byte 0
	men1: .asciiz "Semaforo en verde, esperando pulsador\n"
	men2: .asciiz "Pulsador activado: en 20 segundos, el semaforo cambiara a amarillo\n"
	men3: .asciiz "Semaforo en amarillo,en 10 segundos, semaforo en rojo\n"
	men4: .asciiz "Semaforo en rojo, en 30 segundos, semaforo en verde\n"
.text
.globl main
main:
	# Habilito las interrupciones por teclado:
	mfc0 $t0, $12
	li $t1, 257
	or $t0, $t0, $t1
	mtc0 $t0, $12
	#Activamos el bit 1 para activar las interrupciones por teclear.
	li $t0, 0xffff0000
	lw $t1, 0($t0)
	li $t2, 2
	or $t1, $t1, $t2
	sw $t1, 0($t0)
	jal programa
programa:
	li $s0, -1
	la $t0, estado
	bucleInfinito:
	lb $t1, 0($t0)
	bne $s0, $t1, imprimirMensaje
	j bucleInfinito
	imprimirMensaje:
		move $s0, $t1
		li $t2, 0
		beq $t2, $s0, pulsador
		li $t2, 1
		beq $t2, $s0, cronoToAmarillo
		li $t2, 2
		beq $t2, $s0, cronoToRojo
		la $a0, men4
		li $v0, 4
		syscall
		j bucleInfinito
		pulsador:
			la $a0, men1
			li $v0, 4
			syscall
			j bucleInfinito
		cronoToAmarillo:
			la $a0, men2
			li $v0, 4
			syscall
			j bucleInfinito
		cronoToRojo:
			la $a0, men3
			li $v0, 4
			syscall
			j bucleInfinito
		
.ktext 0x80000180
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
	bne $t0, $t1, verificarCronometro
	# Si el bit 0 de la direccion del teclado mapeado en memoria es 1, significa que
	# la interrupcion vino de el.
	li $t0, 0xffff0004
	lw $t0, 0($t0)
	# Verifico si el caracter es una s.
	li $t1, 115
	bne $t0, $t1, finInterrupcion
	# Si es una s verifico si el semaforo esta en verde(estado == 0)
	lb $t0, estado
	bne $t0, $zero, finInterrupcion
	# El semaforo esta en verde, y pronto pasara a amarillo
	li $t1, 1
	sb $t1, estado
	# Establezco el cronometro en 20 segundos.
	mfc0 $t0, $9
	li $t1, 600
	add $t2, $t1, $t0
	mtc0 $t2, $11
	# Enciendo las interrupciones por cronometro:
	mfc0 $t0, $12
	li $t1, 32769
	or $t0, $t0, $t1
	mtc0 $t0, $12
	j finInterrupcion
	verificarCronometro:
		# Verifico si la interrupcion fue del cronometro
		mfc0 $t0, $13
		srl $t1, $t0, 15
		li $t2, 1
		and $t2, $t2, $t1
		beq $t2, $zero, finInterrupcion
		# Actualizo el semaforo:
		lb $t0, estado
		li $t1, 1
		bne $t0, $t1, posibleRojo
		# termina el cronometro hacia amarillo, el semaforo ahora esta en amarillo
		# Se activa un cronometro de 10s
		mfc0 $t0, $9
		li $t1, 300
		add $t0, $t0, $t1
		mtc0 $t0, $11
		# Cambio el estado a 2(cronometro hacia rojo)
		li $t0, 2
		sb $t0, estado
		j finInterrupcion
	posibleRojo:
		li $t1, 2
		bne $t0, $t1, posibleVerde
		# Si se termino el cronometro, ahora el semaforo es rojo
		# Cronometro en 30 segundos
		mfc0 $t0, $9
		li $t1, 900
		add $t0, $t0, $t1
		mtc0 $t0, $11
		# Cambio el estado a 2(semaforo en rojo)
		li $t0, 3
		sb $t0, estado
		j finInterrupcion
	posibleVerde:
		li $t1, 3
		bne $t0, $t1, finInterrupcion
		# Si estaba en rojo, pasa a verde
		# Cambio el estado a 2(semaforo en rojo)
		li $t0, 0
		sb $t0, estado
		# Apago el bit 15 del registro status.
		mfc0 $t0, $12
		li $t1, 32768
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
