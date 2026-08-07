.data
	men1: .asciiz "Introduzca un numero entero positivo: "
	men2: .asciiz "Es par"
	men3: .asciiz "Es impar"
	men4: .asciiz "Error, numero invalido"
.text
.globl main
main:
	li $v0, 4	# Pido un numero al usuario.
	la $a0, men1
	syscall
	li $v0, 5	# Obtengo el numero y lo guardo.
	syscall
	move $a0, $v0
	jal pariedad # Llamo a la funcion  y evaluo el numero.
	slt $t0, $v0, $zero
	bne $t0, $zero, error # Verifico el numero, debe ser positivo.
	bne $v0, $zero, impar
	la $a0, men2
	j imprimir
	impar:
		la $a0, men3
		j imprimir
	error:
		la $a0, men4
	imprimir:
		li $v0, 4 # Basado en el resultado dado por la funcion  imprimo el resultado.
		syscall
	li $v0, 10 # Finalizacion del programa.
	syscall
pariedad: # Funcion que calcula la pariedad de un numero.
	addi $sp, $sp, 8
	sw $ra, 4($sp)
	sw $a0, 0($sp)
	slt $t0, $a0, $zero
	bne $t0, $zero, errorPariedad # Validacion de dato de entrada, debe ser positivo.
	li $v0, 0
	for: # Ciclo que calcula la pariedad de un numero.
		beq $a0, $zero, finPariedad
		li $t0, 1
		sub $v0, $t0, $v0
		sub $a0, $a0, $t0
		j for
	errorPariedad:
		li $v0, -1
	finPariedad:
		lw $a0, 0($sp)
		lw $ra, 4($sp)
		addi $sp, $sp, 8
		jr $ra
		
	
