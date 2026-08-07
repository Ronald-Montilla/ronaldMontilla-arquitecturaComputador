.data
	men1: .asciiz "Introduzca un numero entero positivo: "
	men2: .asciiz "Es par"
	men3: .asciiz "Es impar"
	men4: .asciiz "Error, numero invalido"
.text
.globl main
main:
	li $v0, 4	# Pido un numero al usuario
	la $a0, men1
	syscall
	li $v0, 5	# Obtengo el numero y lo guardo
	syscall
	move $a0, $v0
	jal pariedadRecursivo # Llamo a la funcion recursiva y evaluo el numero
	slt $t0, $v0, $zero
	bne $t0, $zero, error # Verifico el numero, debe ser positivo
	bne $v0, $zero, impar
	la $a0, men2
	j imprimir
	impar:
		la $a0, men3
		j imprimir
	error:
		la $a0, men4
	imprimir:
		li $v0, 4 # Basado en el resultado dado por la funcion recursiva imprimo el resultado.
		syscall
	li $v0, 10 # Finalizacion del programa
	syscall
pariedadRecursivo: # Funcion recursiva que calcula la pariedad de un numero
	# $a0 contendra el valor "n" cuya pariedad se determinara.
	addi $sp, $sp, -12
	sw $ra, 8($sp)
	sw $a0, 4($sp)
	slt $t0, $a0, $zero
	bne $t0, $zero, errorRecursivo	# Validacion de entrada, debe ser positiva.
	beq $a0, $zero, casoBase	# caso base: el numero es igual a cero
	li $v0, 1
	sw $v0, 0($sp)
	addi $a0, $a0, -1
	jal pariedadRecursivo	# Caso recursivo, se evaluo el numero menos una unidad.
	move $t0, $v0
	lw $v0, 0($sp)
	sub $v0, $v0, $t0
	j finPariedadRecursivo
	errorRecursivo:
		li $v0, -1
		j finPariedadRecursivo
	casoBase:
		li $v0, 0
	finPariedadRecursivo:
		lw $a0, 4($sp)
		lw $ra, 8($sp)
		addi $sp, $sp, 12
		jr $ra
	