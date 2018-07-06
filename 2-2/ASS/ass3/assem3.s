fmt1: .asciz  "input = "
fmt2: .asciz "%d"
fmt3: .asciz "output = "
fmt4: .asciz "non-primenumber( %d )\n"
fmt5: .asciz "prime number\n"
fmt6: .asciz "0 or 1 input invalid\n"
    .align 4
    .global main, scanf, printf
main: save %sp, -104, %sp
loop: set fmt1, %o0
    call printf
    nop ! printf("input = ")

    set fmt2, %o0
    add %fp, -4, %o1
    call scanf
    nop 
    ld [%fp-4], %l0 ! %l0에 수 입력 받기
    ! p $l0
    ! x/d %fp-4

    cmp %l0, 0
    bl exit
    nop ! %l0이 음수일때 exit 으로 분기
    
    cmp %l0, 2
    bl exce
    nop ! 0이나 1일때 예외 처리

    mov 1, %l1 ! 맨처음 나누는수 1 설정
test: inc %l1
    cmp%l1, %l0
    be prime
    nop

    mov %l0, %o0 !
    mov %l1, %o1
    call .rem
    nop
    
    cmp %o0, 0 ! 나머지가 0이면 소수가 아님
    be nonp
    nop

!    inc %l1 ! 나머지가 0이 아니면 나누는 수 1 더하기
!    cmp %l1, %l0
!    be prime
!    nop ! 1 더한 수가 %l0과 같다면 소수로 분기 (위로 올림)

    ba test
    nop 

prime: set fmt3, %o0
    call printf
    nop ! output = 

    set fmt5, %o0
    call printf
    nop
    ba loop
    nop

nonp: set fmt3, %o0
    call printf
    nop ! output = 
    
    set fmt4, %o0
    mov %l1, %o1
    call printf
    nop 
    ba loop
    nop

exce: set fmt6, %o0
    call printf
    nop ! 0 1 입력 예외 처리
    ba loop ! 다시 입력 받기
    nop

exit: ret
    restore       
