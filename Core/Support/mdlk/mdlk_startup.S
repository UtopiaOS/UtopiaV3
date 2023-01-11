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

.data
.align 3
_mdlk_start_static:
    .quad start # The address of the start function (Unslided)

.text
.globl start
.align 2,0x90
start:
    popq %rdi # Mach Header oxf the executable
    pushq $0
    movq %rsp,%rbp # Pointer to the base kernel frame
    andq $-16,%rsp # Force SSE aligment
    subq $16,%rsp # Room for local variables

    movl 8(%rbp),%esi # The low 32 bits of the rsi register are for argc
    leaq 16(%rbp),%rdx # Move the address of argv[0] into rdx (Third argument)

    # The magic of handling the sliding starts here
    # We were slided some bytes we don't currently know
    # but what we do know, is the current instruction address
    # we will take ourselves relative to the current instructions address
    # to obtain our "slide"
    movq _mdlk_start_static(%rip), %r8 # _mdlk_start_static contains an static address to our "start function"
    leaq start(%rip), %rcx # Calculate our current address relative to the rip
    subq %r8, %rcx # Get the slide, we subtract our "static address" from our actual address right now, this goes into arg 4, which is our "slide"

    leaq ___dso_handle(%rip), %r8 # Our own header
    call _mdlk_bootstrap_self

    #movl %esi,%ecx # Copy argc (address) into the low 32 bits of %rcx

    #addl $1,%ecx # argc + 1 Move one the right
    #sall $3,%ecx # * sizeof (char*) Move the size of a char pointer
    #addq %rdx,%rcx # Copy the env [0] <- this should be the env pointer
    #call _mdlk_actual_start # Call the actual start function
    #hlt # NEVER RETURN