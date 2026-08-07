.data
	men1: .asciiz "Antes:"
	men2: .asciiz "Despues:"
	arr: .word 7, 8, 9, 4, 5, 6, 1, 2, 3
.text
.globl main
	main:
		# Muestro el arreglo antes de usar Quick Sort:
		li $v0, 4
		la $a0, men1
		syscall
		la $a0, arr
		li $a1 8
		jal mostrarArreglo
		li $a1, 0
		li $a2, 8
		jal quickSort
		# Muestro el arreglo despues de usar Quick Sort:
		li $v0, 11
		li $a0, 10
		syscall
		li $v0, 4
		la $a0, men2
		syscall
		la $a0, arr
		li $a1, 8
		jal mostrarArreglo
		li $v0, 10
		syscall
	swap:
		# $a0: direccion del primer elemento, $a1: direccion del segundo elemento
		lw $t0, 0($a0)
		lw $t1, 0($a1)
		sw $t1, 0($a0)
		sw $t0, 0($a1)
		jr $ra
	partition:
		# $a0 corresponde a la direccion del arreglo "A[]". $a1 corresponde al indice del primer elemento
		# $a2 corresponde al indice del ultimo elemento.
		addi $sp, $sp, -16
		sw $ra, 12($sp)
		sw $a2, 8($sp)
		sw $a1, 4($sp)
		sw $a0, 0($sp)
		sll $t0, $a2, 2
		add $t0, $t0, $a0
		lw $t1, 0($t0) # $t1 = pivot
		add $t2, $a1, $zero # $t2 sera mi variable i para recorrer el arreglo.
		add $t3, $a1, $zero # $t3 sera mi variable j y apuntara al elemento despues del ultimo menor.
		forPartition:
			slt $t4, $t2, $a2
			beq $t4, $zero, finPartition
			sll $t4, $t2, 2
			add $t4, $t4, $a0
			lw $t5, 0($t4) # $t5 = A[i]
			slt $t6, $t1, $t5
			beq $t6, $zero, intercambio
			addi $t2, $t2, 1
			j forPartition
			intercambio:
			 	# Se intercambian los elementos A[i] y A[j]
				sll $t6, $t3, 2
				add $t6, $t6, $a0
				add $a0, $t4, $zero
				add $a1, $t6, $zero
				# Como swap modifica $t0 y $t1:
				add $t6, $t0, $zero
				add $t7, $t1, $zero
				jal swap
				# Recuperacion de datos importantes:
				add $t0, $t6, $zero
				add $t1, $t7, $zero
				lw $a0, 0($sp)
				lw $a1, 4($sp)
				#Incremento de variables controladoras:
				addi $t2, $t2, 1
				addi $t3, $t3, 1
				j forPartition
		finPartition:
			add $v0, $t3, $zero # $v0 retornara el indice de particion
			sll $t3, $t3, 2
			add $t3, $t3, $a0
			add $a0, $t0,  $zero
			add $a1, $t3, $zero
			jal swap
			lw $a0, 0($sp)
			lw $a1, 4($sp)
			lw $a2, 8($sp)
			lw $ra, 12($sp)
			addi $sp, $sp, 16
			jr $ra
		quickSort:
			# $a0 corresponde con la direccion del arreglo A[], $a1 corresponde al indice del primer elemento
			# $a2 corresponde al indice del ultimo elemento
			addi $sp, $sp, -20
			sw $ra, 16($sp)
			sw $a0, 12($sp)
			sw $a1, 8($sp)
			sw $a2, 4($sp)
			slt $t0, $a1, $a2
			beq $t0, $zero, finQuickSort
			jal partition
			sw $v0, 0($sp)
			addi $a2, $v0, -1
			jal quickSort
			lw $v0, 0($sp)
			lw $a2, 4($sp)
			addi $a1, $v0, 1
			jal quickSort
			finQuickSort:
				lw $a2, 4($sp)
				lw $a1, 8($sp)
				lw $a0, 12($sp)
				lw $ra, 16($sp)
				addi $sp, $sp, 20
				jr $ra
	mostrarArreglo:
		addi $sp, $sp, -12
		sw $ra, 8($sp)
		sw $a0, 4($sp)
		sw $a1, 0($sp)
		# $a0 es la direccion del vector A[], $a1 es el indice del ultimo elemento
		li $t0, 0
		addi $t3, $a1, 1
		loop:	
			slt $t2, $t0, $t3
			beq $t2, $zero, finLoop
			sll $t1, $t0, 2
			add $t1, $t1, $a0
			add $t2, $a0, $zero
			li $v0, 11
			li $a0, 32
			syscall
			lw $a0, 0($t1)
			li $v0, 1
			syscall
			add $a0, $t2, $zero
			addi $t0, $t0, 1
			j loop
			finLoop:
				lw $a1, 0($sp)
				lw $a0, 4($sp)
				lw $ra, 8($sp)
				addi $sp, $sp, 12
				jr $ra