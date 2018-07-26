.section ".data"
res: .single 0r0.0
con1: .single 0r1.0
con2: .single 0r999.0
x: .word 0
fmt1: .asciz "%g"
fmt2: .asciz "input = "
fmt3: .asciz "output = %f\n"

.section ".text"
.align 4
.global main, scanf, printf
main:
    save %sp, -120, %sp
    set res, %l0
    set con1, %l1
    set con2, %l2
    ld [%l0], %f0 ! 0.0
    ld [%l1], %f1 ! 1.0
    ld [%l2], %f2 ! 999.0
loop:
    set fmt2, %o0
    call printf
    nop

    set fmt1, %o0
    set x, %o1
    call scanf
    nop

    set x, %l3 ! x = %f3
    ld [%l3], %f3
    ld [%l2], %f2
    fcmps %f3, %f2 ! x == 999.0
    nop
    fbne add_
    nop
    ba pr_
    nop

add_:
    ld [%l3], %f3
    ld [%l1], %f1
    fcmps %f3, %f1 ! x < 1.0
    nop
    fbl loop
    nop
    
    ld [%l0], %f0
    fadds %f3, %f0, %f6 ! sum += x
    st %f6, [%l0]
    ba loop
    nop

pr_:
    set fmt3, %o0
    ld [%l0], %f0
    fstod %f0, %f2
    std %f2, [%fp-8]
    ldd [%fp-8], %o2
    mov %o2, %o1
    mov %o3, %o2
    call printf
    nop

    ret
    restore
