.text
.globl start
.align 2
start:
    pushq $0 # Start at 0
    movq %rsp,%rbp # Pointer to the base kernel frame
    andq $-16,%rsp # Force SSE aligment
    movq 8(%rbp),%rdi # Lets load argc
    leaq 16(%rbp),%rsi # Address of arg[0], into rsi
    movl %edi,%edx
    addl $1,%edx # argc + 1
    sall $3,%edx # * sizeof(char *)
    addq %rsi,%rdx # Copy the env
    call _mdlk_actual_start # Call the actual start function
    hlt # NEVER RETURN