.data
	men1: .asciiz "Antes: "
	men2: .asciiz "Despues: "
	arr: .word 3, 4, 1, 7, 2, 5
	arr2: .space 100
.text
.globl main
	main:
	# Muestro el arreglo antes de usar Merge Sort:
		li $v0, 4
		la $a0, men1
		syscall
		la $a0, arr
		li $a1 5
		jal mostrarArreglo
		li $a1, 0
		li $a2, 6
		la $a0, arr
		jal mergeSort
		# Muestro el arreglo despues de usar Merge Sort:
		li $v0, 11
		li $a0, 10
		syscall
		li $v0, 4
		la $a0, men2
		syscall
		la $a0, arr
		li $a1, 5
		jal mostrarArreglo
		li $v0, 10
		syscall
	merge:
		# $a0: direccion del arreglo, $a1: indice al primer elemento.
		# $a2: indice al medio del arreglo, $a3: indice del ultimo elemento.
		addi $sp, $sp, -20
		sw $ra, 16($sp)
		sw $a0, 12($sp)
		sw $a1, 8($sp)
		sw $a2, 4($sp)
		sw $a3, 0($sp)
		add $t0, $a1, $zero # primer indice de la primera mitad del arreglo "i".
		addi $t1, $a2, 1 # primer indice de la segunda mitad del arreglo "j".
		li $t2, 0 # indice actual del arreglo auxiliar. "k".
		addi $t3, $a3, 1
		subu $t3, $t3, $a1 # $t3: numero de elementos del arreglo
		add $t4, $a0, $zero
		# Arreglo auxiliar "arr2" con tamaño igual al numero de elementos del arreglo pasado por argumentos.
		# Su direccion se guarda en $t9
		la $t9, arr2
		add $a0, $t4, $zero
		whileMerge:
			slt $t4, $a2, $t0
			bne $t4, $zero, whileDosMerge
			slt $t4, $a3, $t1
			bne $t4, $zero, whileUnoMerge
			# Primera comparacion y guardado:
			sll $t4, $t0, 2
			add $t4, $t4, $a0
			lw $t4, 0($t4) # $t4 = A[i]
			sll $t5, $t1, 2
			add $t5, $t5, $a0
			lw $t5, 0($t5) # $t5 = A[j]
			sll $t7, $t2, 2
			add $t7, $t7, $t9
			addi $t2, $t2, 1
			slt $t6, $t4, $t5
			beq $t6, $zero, guardarSegundo
			sw $t4, 0($t7)
			addi $t0, $t0, 1
			j whileMerge
			guardarSegundo:
				sw $t5, 0($t7)
				addi $t1, $t1, 1
				j whileMerge
		whileUnoMerge:
			slt $t4, $a2, $t0
			bne $t4, $zero, reemplazoArreglo
			sll $t4, $t0, 2
			add $t4, $t4, $a0
			lw $t4, 0($t4) # $t4 = A[i]
			sll $t7, $t2, 2
			add $t7, $t7, $t9
			addi $t2, $t2, 1
			sw $t4, 0($t7)
			addi $t0, $t0, 1
			j whileUnoMerge
		whileDosMerge:
			slt $t4, $a3, $t1
			bne $t4, $zero, reemplazoArreglo
			sll $t5, $t1, 2
			add $t5, $t5, $a0
			lw $t5, 0($t5) # $t5 = A[j]
			sll $t7, $t2, 2
			add $t7, $t7, $t9
			addi $t2, $t2, 1
			sw $t5, 0($t7)
			addi $t1, $t1, 1
			j whileDosMerge
		reemplazoArreglo:
			add $t0, $a1, $zero # indice del primer elemento del arreglo.
			li $t1, 0 # indice del primer elemenr del arreglo auxiliar.
			forMerge:
			slt $t2, $a3, $t0
			bne $t2, $zero, finMerge
			sll $t3, $t1, 2
			add $t3, $t3, $t9
			lw $t3, 0($t3) # $t3 = Au[k]
			addi $t1, $t1, 1
			sll $t4, $t0, 2
			add $t4, $t4, $a0
			sw $t3, 0($t4)
			addi $t0, $t0, 1
			j forMerge
		finMerge:
			lw $a3, 0($sp)
			lw $a2, 4($sp)
			lw $a1, 8($sp)
			lw $a0, 12($sp)
			lw $ra, 16($sp)
			addi $sp, $sp, 20
			jr $ra
	min:
		# Recibe dos numeros enteros y retorna el menor.
		# $a0: numero a, $a1: numero b, $v0 salida del numero menor
		slt $t1, $a0, $a1
		beq $t1, $zero, retornarB
		add $v0, $a0, $zero
		j finMin
		retornarB:
			add $v0, $a1, $zero
		finMin:
			jr $ra 
	mergeSort:
		# $a0: direccion del arreglo, $a1: indice al primer elemento.
		# $a2: valor correspondiente al ultimo indice + 1
		addi $sp, $sp, -32
		sw $s3, 28($sp)
		sw $s2, 24($sp)
		sw $s1, 20($sp)
		sw $s0, 16($sp)
		sw $ra, 12($sp)
		sw $a2, 8($sp)
		sw $a1, 4($sp)
		sw $a0, 0($sp)
		li $s0, 1 # Numero de elementos * 2 a fusionar (numElements)
		move $s1, $a2 # $t1 = total de elementos.
		whileMergeSort:
			slt $t2, $s0, $s1
			beq $t2, $zero, finMergeSort
			# Entra en un for:
			add $s2, $a1, $zero # $s2 empezara en el primer elemento i = 0
			addi $s3, $a2, -1
			sub $s3, $s3, $s0 # $s3 sera la condicion de parada del for.
			forMergeSort:
			slt $t4, $s3, $s2
			bne $t4, $zero, incrementoWhileMergeSort
			# Calculo los argumentos para la funcion merge:
			sll $a0, $s0, 1
			add $a0, $a0, $s2
			addi $a0, $a0, -1 
			addi $a1, $a2, -1
			jal min
			add $a2, $s0, $s2
			addi $a2, $a2, -1 # $a2 sera el indice medio del subarreglo.
			add $a3, $v0, $zero  # $a3 sera el min ($s1 + ($s0 * 2) - 1, n - 1) y ultimo indice del subarreglo.
			lw $a0, 0($sp)
			move $a1, $s2 # $a1 sera el primer indice del subarreglo.
			jal merge
			lw $a0, 0($sp)
			lw $a1, 4($sp)
			lw $a2, 8($sp)
			sll $t4, $s0, 1
			add $s2, $s2, $t4 # Incremento para el for.
			j forMergeSort
			incrementoWhileMergeSort:
				sll $s0, $s0, 1
				j whileMergeSort
		finMergeSort:
			lw $a0, 0($sp)
			lw $a1, 4($sp)
			lw $a2, 8($sp)
			lw $ra, 12($sp)
			lw $s0, 16($sp)
			lw $s1, 20($sp)
			lw $s2, 24($sp)
			lw $s3, 28($sp)
			addi $sp, $sp, 32
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