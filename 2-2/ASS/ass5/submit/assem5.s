.section ".data"
n: .word 4
arr: .skip 4*361 ! int arr[19][19]
in_n: .asciz "input n = "
dec: .asciz "%d"
pr_dec: .asciz "%d "
pr_n: .asciz "\n"
.section ".text"
    .align 4
    .global main, printf, scanf
main:
    ! printf("input n = ");
    ! scanf("%d", &n);
    save %sp, -96, %sp
loop:
    set in_n, %o0
    call printf
    nop
    set dec, %o0
    set n, %o1
    call scanf
    nop
    !if(2 < n && n < 20)
    !else exit

    ld [%o1], %l0 ! %o0에 있는 값을 %l0에 저장
    cmp %l0, 2
    ble exit !
    nop
    cmp %l0, 20
    bge exit ! 2 < n < 20이 아니면 종료
    nop
    ! 1 행 가운데 열에 1 저장하기
    sub %l0, 1, %l3
    mov %l3, %o0
    mov 2, %o1
    call .div ! 가운데 값 구하기
    nop
    
    mov %g0, %l1 ! %l1 을 i로
    mov %o0, %l2 ! %l2 를 j로
    mov 1, %l3 ! %l3에 1 저장
    
    sll %l2, 2, %o0 ! 0행 j열에 1 저장
    set arr, %o1
    st %l3, [%o0+%o1] 
    
    sub %l0, 1, %i2 ! %i2 = n - 1
    mov %l0, %o0
    mov %l0, %o1
    call .mul
    nop
    mov %o0, %i1
for:
    cmp %l3, %i1 ! k와 n*n비교
    bge pr1_init
    nop ! 프린트하러가기
    
    ! 증감식 가는 조건  우측 위 판별
cond_:
    cmp %l1, 0
    bne cond0
    nop
    cmp %l2, %i2
    bne cond0
    nop
    add %l1, 1, %l1 ! i = i - 1 + 2, j = j + 1 - 1
    ba cond1 
    nop
    ! 증감식 안가는 조건 우측 위 판별
cond__:
    cmp %l1, 0
    bne cond1
    nop
    cmp %l2, %i2
    bne cond1
    nop
    add %l1, 1, %l1
    ba cond1
    nop

cond0:
    sub %l1, 1, %l1
    add %l2, 1, %l2
     
cond1: ! 행이 맨위
    cmp %l1, 0
    bge cond2 
    nop
    add %l1, %l0, %l1 ! i = i + n ( -1 일때 n - 1)

cond2: ! 열이 맨오른쪽
    cmp %l2, %l0
    bl cond3
    nop ! j < n
    sub %l2, %l0, %l2 ! j = j - n ( n 일때 0)

cond3:
    mov %l1, %o0
    mov %l0, %o1
    call .mul
    nop ! n * i
    add %o0, %l2, %o0 ! n * i + j
    mov 4, %o1
    call .mul
    nop ! (n * i + j) * 4
    set arr, %o1
    add %o0, %o1, %o3
    ld [%o3], %l5! %i0 = arr[i][j]
    
    cmp %l5, 0
    be st_
    nop
    ba cond4 
    nop
cond4:
    sub %l2, 1, %l2 ! 왼쪽으로 한자리 j = j - 1
    add %l1, 2, %l1 ! 밑으로 두자리 i = i + 2
    ba cond__
    nop
st_:
    inc %l3 ! k++ 
    st %l3, [%o0+%o1] ! arr[i][j] = k
    ba for
    nop

    ! for(int a=0; a<n; a++)
    !   for(int b=0; b<n; b++)
pr1_init:
    clr %l5 ! a=0
pr1_:
    cmp %l5, %l0
    bge pr1_end
    nop

pr2_init:
    clr %l6 ! b=0
pr2_:
    cmp %l6, %l0
    bge pr2_end
    nop

    mov %l5, %o0
    mov %l0, %o1
    call .mul
    nop ! a * 열의 수
    add %o0, %l6, %l7 ! %l7 index
    sll %l7, 2, %l7 ! %l7 = (a * 열의수 + b) * 4;
    set arr, %o0
    add %o0, %l7, %o0
    ld [%o0], %i3
    set pr_dec, %o0
    mov %i3, %o1
    call printf
    nop
    inc %l6 ! b++;
    ba pr2_
    nop
pr2_end:
    set pr_n, %o0
    call printf
    nop
    inc %l5
    ba pr1_
    nop
pr1_end:

i1_init:
    clr %l1
i1_:
    cmp %l1, %l0
    be i1_out
    nop

    i2_init:
     clr %l2
    i2_:
        cmp %l2, %l0
        be i2_out
        nop

        mov %l1, %o0
        mov %l0, %o1
        call .mul
        nop
        add %o0, %l2, %o0
        sll %o0, 2, %o0
        set arr, %o1
        st %g0, [%o0+%o1]
    
        inc %l2
        ba i2_
        nop
    i2_out:
        inc %l1
        ba i1_
        nop
i1_out:
    ba loop
    nop
exit:
    ret
    restore
