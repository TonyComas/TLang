.global main
main:
    push %rbp
    mov %rsp, %rbp
    sub $32, %rsp
    mov $10, %rax
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    push %rax
    mov $2, %rax
    pop %rcx
    add %rcx, %rax
    mov %rax, -16(%rbp)
    mov -8(%rbp), %rax
    push %rax
    mov $8, %rax
    pop %rcx
    sub %rax, %rcx
    mov %rcx, %rax
    mov %rax, -24(%rbp)
    mov $20, %rax
    mov %rax, -8(%rbp)
    mov -24(%rbp), %rax
    push %rax
    mov -16(%rbp), %rax
    pop %rcx
    add %rcx, %rax
    push %rax
    mov -8(%rbp), %rax
    pop %rcx
    add %rcx, %rax
    leave
    ret
    mov $0, %rax
    leave
    ret
