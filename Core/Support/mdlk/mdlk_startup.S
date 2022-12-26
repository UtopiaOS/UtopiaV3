# After the kernel is done setting up the stack, this is how it looks
# [Top of the stack]
#                   Utopia pointers
#                         |
#             Environment variable pointers
#                         |
#                   Argv pointers
#                         |
#                        Argc
#                         |
#                    Mach header
#
.text
.globl start
.align 2,0x90
start:
    popq %rdi # Mach Header of the executable
    pushq $0
    movq %rsp,%rbp # Pointer to the base kernel frame
    andq $-16,%rsp # Force SSE aligment
    subq $16,%rsp # Room for local variables

    movl 8(%rbp),%esi # The low 32 bits of the rsi register are for argc
    leaq 16(%rbp),%rdx # Move the address of argv[0] into rdx (Third argument)

    movl %esi,%ecx # Copy argc (address) into the low 32 bits of %rcx

    addl $1,%ecx # argc + 1 Move one the right
    sall $3,%ecx # * sizeof (char*) Move the size of a char pointer
    addq %rdx,%rcx # Copy the env [0] <- this should be the env pointer
    call _mdlk_actual_start # Call the actual start function
    hlt # NEVER RETURN