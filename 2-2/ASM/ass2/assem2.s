fmt1: .asciz "==output== %d \n"
fmt2: .asciz " input = "
fmt3: .asciz "result = %x \n"
fmt4: .asciz "%d"
fmt5: .asciz " n = "
    .align 4
    .global main, scanf, printf

main: save %sp, -104, %sp
    set fmt2, %o0
    call printf
    nop
    ! 출력 input = 
    set fmt4, %o0
    add %fp, -4, %o1
    call scanf
    nop
    ! %d 입력받기
    add %fp, -4, %o0
    ld [%o0], %l0
    ! p $l0
    ! x/d $fp-4
   ! set fmt1, %o0
   ! mov %l0, %o1
   ! call printf
   ! nop
    ! ==output== 처음 수(%l0)
loop: set fmt5, %o0
    call printf
    nop
    ! n =
    set fmt4, %o0
    add %fp, -8, %o1
    call scanf
    nop
    ! 자리 이동 횟수 입력 받기
    add %fp, -8, %o0
    ld [%o0], %l2
    ! p $l2
    ! x/d $fp-8
    subcc %l2, 0, %g0
    bl exit_r
    nop
    !음수면 exit_r 종료
    srl %l0, %l2, %l1
    !l0을 n만큼 우측 쉬프트 후 l1에 저장
    add %g0, 32, %l4
    sub %l4, %l2, %l4
    ! 32-n을 l4에 저장
    sll %l0, %l4, %l3
    !l0을 32-n 만큼 좌측 쉬프트 후 l3에 저장
    or %l1, %l3, %l5
    ! p $l1
    ! p $l3
    ! p $l5
    set fmt3, %o0
    mov %l5, %o1
    call printf
    nop
    ! result = 16진수로 출력
    ba loop
    nop    
    ! 다시 입력받기

exit_r: ret
    restore
