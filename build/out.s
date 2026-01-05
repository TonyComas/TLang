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
    cmp $0, %rax
    mov $0, %rax
    sete %al
    push %rax
    mov -16(%rbp), %rax
    pop %rcx
    cmp %rax, %rcx
    setl %al
    movzb %al, %rax
    cmp $0, %rax
    jne .L0
    mov -8(%rbp), %rax
    push %rax
    mov $10, %rax
    pop %rcx
    cmp %rax, %rcx
    setne %al
    movzb %al, %rax
    cmp $0, %rax
    jne .L0
    mov $0, %rax
    jmp .L1
.L0:
    mov $1, %rax
.L1:
    leave
    ret
    mov $0, %rax
    leave
    ret
