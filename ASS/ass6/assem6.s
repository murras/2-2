fmt1: .asciz "input = "
fmt2: .asciz "%d"
fmt3: .asciz "output = %d\n"
fmt4: .asciz "input 1 or 2\n"
.global main, printf, scanf
.align 4
main:
    save %sp, -104, %sp
start:
    ! printf("input = ");
    ! scanf("%d", &n); n 주소 %fp-4
    set fmt1, %o0
    call printf
    nop
    set fmt2, %o0
    add %fp, -4, %o1
    call scanf
    nop
    !n 값이 음수이면 종료
    ld [%fp-4], %l0
    cmp %l0, 0
    bl exit
    nop
    ! printf("input = ");
    ! scanf("%d", &m); m 주소 %fp-8
    set fmt1, %o0
    call printf
    nop
    set fmt2, %o0
    add %fp, -8, %o1
    call scanf
    nop
    ! printf("input = ");
    ! scanf("%d", &l); l 주소 %fp-12
input_l:
    set fmt1, %o0
    call printf
    nop
    set fmt2, %o0
    add %fp, -12, %o1
    call scanf
    nop
    ! l 값 불러와서 서브루틴 1인지 2인지 확인후 분기
    ld [%fp-12], %l0
    cmp %l0, 1
    be sub1
    nop
    cmp %l0, 2
    be sub2
    nop
    set fmt4, %o0
    call printf
    nop
    ba input_l
    nop   
    
    ! 서브루틴1 n m 값 넘기기
    ! n m 비교해서 n이 더 크면 나가기
sub1:
    ld [%fp-4], %o0
    ld [%fp-8], %o1
    call f_sub1
    nop
    mov %o0, %o1
    set fmt3, %o0
    call printf
    nop
    ba start
    nop

    ! 서브루틴2 n m 값 넘기기
sub2:
    ld [%fp-4], %o0
    ld [%fp-8], %o1
    call f_sub2
    nop
    set fmt3, %o0
    mov %o3, %o1
    call printf
    nop
    stop4:
    ba start
    nop

f_sub1:
    save %sp, -96, %sp
    ! for(int i = n ; i <= m; i++)
    ! sum += i*i
    clr %l0 ! %l0 (=sum)
    loop_sub1:
        cmp %i0, %i1 ! n 값이 m 보다 커지면 종료
        bg exit_sub1
        nop
        
        mov %i0, %o0 ! i * i
        mov %i0, %o1
        call .mul
        nop
        add %o0, %l0, %l0 ! sum += i*i
        inc %i0 ! i++
        ba loop_sub1
        nop

    exit_sub1:
        mov %l0, %i0 ! %i0 로 값 전달하고 나가기
        ret
        restore
 
f_sub2:
    clr %o3 ! %o3(=sum)
    ! for(int i = n; i <= m; i++)
    ! sum += i
    loop_sub2:
        cmp %o0, %o1
        bg exit_sub2
        nop
        
        add %o0, %o3, %o3 ! sum += i
        inc %o0
        ba loop_sub2
        nop
    exit_sub2:
        retl
        nop

exit:
    ret
    restore
