.global main
main:
    mov $2, %rax
    push %rax
    mov $3, %rax
    push %rax
    mov $4, %rax
    pop %rcx
    imul %rcx, %rax
    pop %rcx
    add %rcx, %rax
    ret
