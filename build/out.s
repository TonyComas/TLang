.global main
main:
    push %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    mov $10, %rax
    mov %rax, -8(%rbp)
    mov $7, %rax
    mov %rax, -16(%rbp)
    mov -8(%rbp), %rax
    push %rax
    mov $5, %rax
    pop %rcx
    cmp %rax, %rcx
    setl %al
    movzb %al, %rax
    cmp $0, %rax
    je .Lend0
    mov -16(%rbp), %rax
    leave
    ret
.Lend0:
    mov $2, %rax
    leave
    ret
    mov $0, %rax
    leave
    ret
