.data
	tensionControl: .word 0
	tensionEstado: .word 0
	tensionSistol: .word 0
	tensionDiastol: .word 0
.text
.globl main
main:
	jal controlador_tension
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
		bnq $t1, $t2, en_espera
	la $t0, tensionSistol
	la $t1, tensionDiastol
	lw $v0, 0($t0) # Retornamos en $v0 el valor de la tension sistol.
	lw $v1, 0($t1) # Retornamos en $v1 el valor de la tension diastol.
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra
		
