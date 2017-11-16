.section ".data"
n: .skip 4
arr: .skip 4*19*19 ! int arr[19][19]
mark: .skip 4*19*19 ! int mark[19][19]

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

    set n, %o0 ! n의 주소를 %o0에
    ld [%o0], %l0 ! %o0에 있는 값을 %l0에 저장
    cmp %l0, 2
    ble exit !
    nop
    cmp %l0, 20
    bge exit ! 2 < n < 20이 아니면 종료
    nop
    ! 1 행 가운데 열에 1 저장하기
    mov %l0, %o0
    mov 2, %o1
    call .div ! 가운데 값 구하기
    nop
    clr %l1 ! %l1 을 i로
    mov %o0, %l2 ! %l2 를 j로
    mov 1, %l3 ! %l3에 1 저장

    mov %l0, %o0
    mov %l0, %o1
    call .mul
    nop
    mov %o0, %i1

for:
    cmp %l3, %i1 ! k와 n*n비교
    bge pr1_init
    nop ! 프린트하러가기
    ! 첫행, 마지막열 검사
    cmp %l1, 0 ! 첫 행 검사
    bne normal
    nop
    cmp %l2, %l0 ! 마지막 열 검사
    bne normal
    nop
    ! 둘 다 참일 경우
    add %l1, 2, %l1 ! 밑으로 2번
    sub %l2, 1, %l2 ! 왼쪽으로 1번
    ba con1
    nop ! mark 검사하러

normal:
    sub %l1, 1, %l1 ! i--;
    add %l2, 1, %l2 ! j++;
con1:
    ! i 검사
    cmp %l1, 0 ! i >= 0 이면 그냥 진행 i < 0이면 n-1로
    bge non_m
    nop
    mov %l0, %l1
    sub %l1, 1, %l1 ! i = n-1
non_m:
    ! j 검사
    mov %l2, %o0
    mov %l0, %o1
    call .rem
    nop
    mov %o0, %l2
    ! j = (%o0) % (%o1)
index:
    mov %l0, %o0
    mov %l1, %o1
    call .mul
    nop ! i * 열의 수
    add %o0, %l2, %l4 ! %l4 index
    sll %l4, 2, %l4 ! %l4 = (i * 열의수 + j ) * 4;
mark_:
    set mark, %o0
    add %o0, %l4, %o0
    ld [%o0], %i0 ! %i0 = mark[i][j];
    cmp %i0, 1
    bne n_writ
    nop
written:
    ! 쓰여진곳이면
    add %l1, 2, %l1 ! i = i + 2;
    sub %l2, 1, %l2 ! j = j - 1;
    ba index
    nop
n_writ:
    set arr, %o0
    add %o0, %l4, %o0
    st %l3, [%o0] ! arr[i][j] = k;
    inc %l3 ! k++
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
    ba loop
    nop


exit:
    ret
    restore
