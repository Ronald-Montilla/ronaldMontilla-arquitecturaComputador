.data
	tensionControl: .word 0
	tensionEstado: .word 0
	tensionSistol: .word 0
	tensionDiastol: .word 0
	men1: .asciiz "Tension sistol: "
	men2: .asciiz "Tension diastol: "
.text
.globl main
main:
	# Valores de sistol y diastol.
	li $v0, 42
	li $a0, 0
	li $a1, 300
	syscall
	sw $a0, tensionSistol
	li $v0, 42
	li $a0, 0
	li $a1, 300
	syscall
	sw $a0, tensionDiastol
	# Llamo a la funcion principal de la simulacion
	jal controlador_tension
	move $t0, $v0
	move $t1, $v1
	# Muestro el resultado de la tension Sistol:
	li $v0, 4
	la $a0, men1
	syscall
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	# Muestro el resultado de la tension Diastol:
	li $v0, 4
	la $a0, men2
	syscall
	li $v0, 1
	move $a0, $t1
	syscall
	li $v0, 10
	syscall
controlador_tension:
addi $sp, $sp, -4
sw $ra, 0($sp)
la $t0, tensionControl
li $t1, 1
sw $t1, 0($t0) # Se inicia la medicion.
la $t0, tensionEstado
li $t1, 1
	en_espera: # Ciclo de espera, en el que no saldra hasta que tensionEstado sea 1.
		lw $t2, 0($t0)
		bne $t1, $t2, en_espera
	lw $v0, tensionSistol # Retornamos en $v0 el valor de la tension sistol.
	lw $v1, tensionDiastol # Retornamos en $v1 el valor de la tension diastol.
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra