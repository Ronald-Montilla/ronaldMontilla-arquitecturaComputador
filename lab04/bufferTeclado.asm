.data
	buffer: .space 200
	maximo: .word 200
	head: .word 0
	tail: .word 0
	contador: .word 0 
.text
.globl main
main:
	#Configuramos el registro status
	# Activamos los bits 0 y 9 para habilitar las interrupciones por teclado.
	mfc0 $t0, $12
	li $t1, 513
	or $t0, $t0, $t1
	mtc0 $t0, $12
	# Configuracion del teclado:
	#Activamos el bit 1 para activar las interrupciones por teclear.
	li $t0, 0xffff0000
	lw $t1, 0($t0)
	li $t2, 2
	or $t1, $t1, $t2
	sw $t1, 0($t0)
	jal programaPrincipal
	li $v0, 10
	syscall	
programaPrincipal:
	#Iniciamos el cronometro de 20 segundos
	li $t0, 20000 # Tiempo maximo del cronometro
	li $v0, 30
	syscall
	move $t1, $a0 #Tiempo inicial
bucleCronometro:
	li $v0, 30
	syscall
	move $t2, $a0 # Tiempo actual
	sub $t2, $t2, $t1
	slt $t3, $t2, $t0
	bne $t3, $zero, bucleCronometro
	# Culminaron los 20 segundos, se procede a vaciar el buffer:
	mfc0 $t1, $12
	li $t2, -2
	and $t1, $t1, $t2
	mtc0 $t1, $12
	vaciarBuffer:
		lw $t1, maximo
		lw $t2, contador
		la $t3, buffer
		lw $t4, tail
		li $t5, 0 # Contador para forVaciarBuffer
		forVaciarBuffer:
			slt $t6, $t5, $t2
			beq $t6, $zero, finForVB
			# Leemos el caracter
			add $t6, $t4, $t3
			lb $a0, 0($t6)
			li $v0, 11
			syscall
			# Aumentamos el indice tail
			addi $t4, $t4, 1
			slt $t6, $t4, $t1
			bne $t6, $zero, aumentarForVB
			li $t4, 0
			aumentarForVB:
				addi $t5, $t5, 1
				j forVaciarBuffer
			finForVB:
				li $t2, 0
				sw $t2, contador
				sw $t2, head
				sw $t2, tail
				li $a0, '\n'
				li $v0, 11
				syscall
				mfc0 $t1, $12
				li $t2, 1
				or $t1, $t1, $t2
				mtc0 $t1, $12
				j programaPrincipal
.ktext 0x80000180
	#Guardo los registros:
	addi $sp, $sp, -16
	sw $t0, 0($sp)
	sw $t1, 4($sp)
	sw $t2, 8($sp)
	sw $t3, 12($sp)
	# Leo el registro cause para verificar el tipo de interrupcion, bit 2 - bit 6:
	# Si los bits 2-6 son 0, significa que fue una interrupcion externa.
	mfc0 $t0, $13
	li $t1, 124
	and $t0, $t0, $t1
	bne $t0, $zero, finAnalisisInter
	# Revisamos si el teclado produjo la interrupcion:	
	# Si hay un 1 en su bit 0, entonces fue el teclado
	li $t0, 0xFFFF0000
	lw $t0, 0($t0)
	andi $t0, $t0, 1	
	beq $t0, $zero, finAnalisisInter
	# Si fue el teclado, entonces leemos el caracter
	li $t0, 0xFFFF0004
	lw $t0, 0($t0)
	# Verificamos si el caracter es una letra del alfabeto mayuscula.
	slti $t1, $t0, 65
	bne $t1, $zero, finAnalisisInter
	slti $t1, $t0, 91
	beq $t1, $zero, finAnalisisInter
	# Guardo el caracter en el buffer
	la $t1, buffer
	lw $t2, head
	add $t2, $t2, $t1
	sb $t0, 0($t2)
	# Actualizamos el contador	
	# !--- valor maximo en el registro $t0 ---!
	lw $t0, maximo
	lw $t1, contador
	slt $t3, $t1, $t0
	beq $t3, $zero, maximoActivado
	# Si no salta a maximoActivado, significa que el buffer nunca se ha llenado por completo
	addi $t1, $t1, 1
	sw $t1, contador
	j evaluarHead
	maximoActivado:
		# Aumentamos el indice tail
		lw $t1, tail
		addi $t1, $t1, 1
		slt $t3, $t1, $t0
		bne $t3, $zero, guardarTail
		li $t1, 0
	guardarTail:
		sw $t1, tail
	evaluarHead:
		# Aumento el indice head
		lw $t2, head
		addi $t2, $t2, 1
		slt $t3, $t2, $t0
		bne $t3, $zero, guardarHead
		li $t2, 0
	guardarHead:
		sw $t2, head
	finAnalisisInter:
	lw $t3, 12($sp)
	lw $t2, 8($sp)
	lw $t1, 4($sp)
	lw $t0, 0($sp)
	addi $sp, $sp, 16
	eret
