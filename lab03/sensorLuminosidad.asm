.data
	luzControl: .word 0
	luzEstado: .word 0
	luzDatos: .word 0
.text
.globl main
main:
	jal inicializarSensorLuz
	jal leerLuminosidad
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