fmt0: .asciz "%d"
fmt1: .asciz "\narray size n(0 < n < 16) : "
fmt2: .asciz "input array[%d] : "
fmt3: .asciz "sorted array\n"
fmt4: .asciz "%d "
    .align 4
n_s = -4
a_s = -64
    .global main, printf, scanf

main: save %sp, -160, %sp
loop:
    set fmt1, %o0
    call printf
    nop ! array size n input
    set fmt0, %o0
    add %fp, n_s, %o1
    call scanf
    nop ! scanf %d &fp-4
    
    ld [%fp-4], %l0 ! n을 %l0에
    cmp %l0, 0
    ble exit
    nop ! %l0 <= 0
    cmp %l0, 16
    bge exit
    nop
! 0 < n < 16 이 아닐 경우 종료
    
    add %fp, a_s, %l1 ! %l1 = %fp-64

!for(int i = 0 ; i < n ; i++) 구현하기
    mov 0, %i0 ! %i0 = 0 (int i = 0) 
for: 
    cmp %i0, %l0 ! %i0 >= %l0 즉  i >= n
    bge sort ! i < n 이면 그대로 실행
    nop ! i >= n 이면 sort로 분기

    set fmt2 ,%o0
    add %g0, %i0, %o1
    call printf
    nop ! printf("input array[%d] : ", i)
    
    set fmt0, %o0
    sll %i0, 2, %o1 ! %o1 = i * 4
    add %l1, %o1, %o1 ! %o1 = %fp - 64 + i * 4
    call scanf
    nop ! scanf("%d", &arr[i])
    inc %i0 ! i++
    ba for
    nop
    
sort: 
!for(int i=0, i < n-1; i++)
    sub %l0, 1, %l2 ! %l2 = n - 1
    mov 0, %i0 ! %i0 = 0 (int i = 0)
sort_i_start:
    cmp %i0, %l2 ! i < n-1
    bg sorted ! i >= n-1이면 sorted 로 분기
    nop

    mov %i0, %l3 ! %l3 = %i0 (current = i)
    add %i0, 1, %i1 ! %i1 = %i0 + 1 (j = i+1)

sort_j_start:
    cmp %i1, %l0 
    bg sort_j_out ! j < n 이면 for문 실행
    nop ! j > n 이면 sort_j_out으로 분기
    
    
    sll %i1, 2, %i4 ! %i4 = j * 4
    ld [%l1 + %i4], %l4 ! %fp-64+4*j를 %l4에 저장 즉 arr[j]
    sll %l3, 2, %i5 ! %i5 = current * 4
    ld [%l1 + %i5], %l5 ! %fp-64+4*current를 %i3에 저장 즉 arr[current]

    cmp %l4, %l5 ! if(arr[j] < arr[current])
    bge incj ! else
    nop
tr:    
    mov %i1, %l3 ! current = j
incj:
    inc %i1 ! j++
    ba sort_j_start
    nop
    
sort_j_out:
    sll %i0, 2, %i3 ! i*4
    ld [%l1 + %i3], %l7 ! %l7 = arr[i]
    st %l5, [%l1 + %i3] ! %i5를 arr[i]에 저장 즉 arr[i]=arr[current]
    st %l7, [%l1 + %i5] ! arr[current] = arr[i]

    inc %i0 ! i++
    ba sort_i_start
    nop
    
sorted:
    set fmt3, %o0
    call printf
    nop ! printf("sorted array\n")

! for(int i=0; i < n; i++)   
    mov 0, %i0 !int i = 0
pr:
    cmp %i0, %l0
    bge loop ! i < n 이면 for문 실행
    nop ! i >= n 이면 for문 나가기 처음으로.

    sll %i0, 2, %i1
    set fmt4, %o0
    ld [%l1 + %i1], %i2
    mov %i2, %o1 ! %fp -64 + i * 4
    call printf
    nop ! printf("%d ", arr[i])

    inc %i0 ! i++
    ba pr
    nop

exit: ret
    restore
