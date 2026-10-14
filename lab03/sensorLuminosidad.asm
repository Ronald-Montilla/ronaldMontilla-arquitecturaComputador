.data
	luzControl: .word 0
	luzEstado: .word 0
	luzDatos: .word 0
	minLuminosidad: .word 0
	maxLuminosidad: .word 1023
	men1: .asciiz "Valor de luminosidad: "
	men2: .asciiz "Se produjo un error de hardware."
.text
.globl main
main:
	jal inicializarSensorLuz
	# Cargamos un valor aleatorio en el registro de datos.
	lw $a0, minLuminosidad
	lw $a1, maxLuminosidad
	addi $a1, $a1, 1
	li $v0, 42
	syscall
	sw $a0, luzDatos
	jal leerLuminosidad
	# Verificamos que no se produjo un error de hardware
	beq $v1, $zero, mostrarValor
		li $v0, 4
		la $a0, men2
		syscall
		j salir
	mostrarValor:
		move $t0, $v0
		li $v0, 4
		la $a0, men1
		syscall
		li $v0, 1
		move $a0, $t0
		syscall
	salir:
		li $v0, 10
		syscall
	
inicializarSensorLuz:
       	addi $sp, $sp, -4
       	sw $ra, 0($sp)
       	# Inicializamos el sensor:
	la $t0, luzControl
	li $t1, 1
	sw $t1, 0($t0)
	# Bucle en espera del resultado del codigo de estado:
	la $t0, luzEstado
	espera:
		lw $t1, 0($t0)
		beq $t1, $zero, espera
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
leerLuminosidad:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	# Leemos la luminocidad de luzDatos:
	la $t0, luzDatos
	lw $t1, 0($t0)
	move $v0, $t1 # Se guarda el valor leido de luminosidad
	# Leemos el codigo de estado:
	la $t0, luzEstado
	lw $t1, 0($t0)
	li $t2, 1
	bne $t1, $t2, error
	li $v1, 0 # Retornamos lectura correcta.
	j finLeerLuminosidad
	error:
		li $v1, -1 # Retornamos error.
	finLeerLuminosidad:
		lw $ra, 0($sp)
		addi $sp, $sp, 4
		jr $ra
