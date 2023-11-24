Things we need to have for the next update:
1. firmware folder.
2. app src code.
3. baseline cartridge project, including Makefile, crt0.s, func stubs, header file.
4. Makefile for firmware at least, should be more than that.

What we need to talk about:
1. discussion 11-06, crt0.s and main.c. Newly added funcs and assembly code
2. multi thread funcs: need to implement how?
3. create Makefile to compile
4. create header files
5. baseline cartridge project: like the one Dedeboy has. a separate dir with all the standard stuff

System Call Stub Functions

The system call stub functions provide a high-level language interface to a function whose main job is to generate the software interrupt (trap) needed to get the kernel's attention. These functions are often called wrappers.

The stub functions on most operating systems do the same basic steps. While the details of implementation differ, they include the following:

set up the parameters,
trap to the kernel,
check the return value when the kernel returns, and
if no error: return immediately, else
if there is an error: set a global error number variable (called "errno") and return a value of -1.

6. What is "stub to invoke syscall" in "Baseline Catridge Requirements"?

This is the assembly code that will invoke the ECALL. If you look at the discussion code where we did this in the cartridge you will see for the GetTicks I believe that there is a setting of the a5 register and then the ECALL instruction. This is the stub to call the system call from the function.
