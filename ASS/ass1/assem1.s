fmt2: .asciz "input ="
fmt4: .asciz "%d"
    .align 4
    .global main, scanf, printf

main: save %sp, -104, %sp
loop: set fmt2, %o0
    call printf
    nop ! printf("input = ")
    
    set fmt4, %o0
    add %fp, -4, %o1 ! %d 키보드로 입력받아 %fp-4에 저장
    call scanf
    nop

    add %fp, -4, %o0
    ld [%o0], %l1 ! l1에 %fp-4 에 기억되어있는 정보를 저장
    
    subcc %l1, 0, %g0
    bl exit
    nop ! 음수면 exit 으로 이동

    set fmt2, %o0
    call printf
    nop ! 두번째 input값 받기

    set fmt4, %o0
    add %fp, -8, %o1
    call scanf
    nop

    add %fp, -8, %o0
    ld [%o0], %l2

    mov %g0, %l7
    
    loop_r: subcc %l1, %l2, %g0
            bg loop
            nop

            add %l1, %l7, %l7
            inc %l1

            ba loop_r
            nop
    
exit: ret
    restore
