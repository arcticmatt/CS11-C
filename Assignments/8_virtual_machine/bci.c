/*
 * CS 11, C track, lab 8
 *
 * FILE: bci.c
 *       Implementation of the bytecode interpreter.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bci.h"


/* Define the virtual machine. */
vm_type vm;


/*
 * Does: Initializes the virtual machine.
 * Arguments: Void.
 * Returns: Void.
 */
void init_vm(void)
{
    int i;

    /*
     * Initialize the stack.  It grows to the right i.e.
     * to higher memory.
     */

    vm.sp = 0;

    for (i = 0; i < STACK_SIZE; i++)
    {
        vm.stack[i] = 0;
    }

    /*
     * Initialize the registers to all zeroes.
     */

    for (i = 0; i < NREGS; i++)
    {
        vm.reg[i] = 0;
    }

    /*
     * Initialize the instruction buffer to all zeroes.
     */

    for (i = 0; i < MAX_INSTS; i++)
    {
        vm.inst[i] = 0;
    }

    vm.ip = 0;
}


/*
 * Does: Helper function to read in integer values which take up varying
 * numbers of bytes from the instruction array 'vm.inst'.
 *
 * NOTES:
 * 1) This function moves 'vm.ip' past the integer's location
 *    in memory.
 * 2) This function assumes that integers take up 4 bytes and are
 *    arranged in a little-endian order (low-order bytes at the
 *    beginning).  This should hold for any pentium-based microprocessor.
 * 3) This function only works for n = 1, 2, or 4 bytes.
 * Arguments: The integer value (which takes up a varying number of bytes).
 * Returns: An integer representing a valid location to jump to or
 * value to push.
 */
int read_n_byte_integer(int n)
{
    int i;
    unsigned char *val_ptr;
    int val = 0;

    /* This only works for 1, 2, or 4 byte integers. */
    assert((n == 1) || (n == 2) || (n == 4));

    val_ptr = (unsigned char *)(&val);

    for (i = 0; i < n; i++)
    {
        *val_ptr = vm.inst[vm.ip];
        val_ptr++;
        vm.ip++;
    }

    return val;
}


/*
 * Machine operations.
 */

/*
 * Does: Pushes an element to the current stack pointer,
 * then increments the stack pointer.
 * Arguments:
 * -- n: The value to be pushed.
 * Returns: Void.
 */
void do_push(int n)
{
    if (vm.sp < STACK_SIZE - 1)
    {
        vm.stack[vm.sp] = n;
        vm.sp++;
    }
    else
    {
        fprintf(stderr, "Stack overflow");
        exit(1);
    }
}


/*
 * Does: Decrements the stack pointer.
 * Arguments: Void.
 * Returns: Void.
 */
void do_pop(void)
{
    if (vm.sp > 0)
    {
        vm.sp--;
    }
    else
    {
        fprintf(stderr, "Stack underflow");
        exit(1);
    }
}


/*
 * Does: Pushes an integer from the n-th index
 * of the register onto the stack.
 * Arguments:
 * -- n: The index of the register whose value
 * we will push.
 * Returns: Void.
 */
void do_load(int n)
{
    if (n >= 0 && n < NREGS)
    {
        do_push(vm.reg[n]);
    }
}


/*
 * Does: Stores and pops the element from the top of the stack
 * in the n-th index of the register.
 * Arguments:
 * -- n: The index of the register at which we will store the popped value.
 * Returns: Void.
 */
void do_store(int n)
{
    do_pop();
    if (n >= 0 && n < NREGS)
    {
        vm.reg[n] = vm.stack[vm.sp];
    }
}


/*
 * Does: Jumps to instruction number n.
 * Arguments:
 * -- n: The instruction number to jump to.
 * Returns: Void.
 */
void do_jmp(int n)
{
    if (n >= 0 && n < MAX_INSTS)
    {
        vm.ip = n;
    }
}


/*
 * Does: Jumps to instruction number n if the top of
 * the stack is zero.
 * Arguments:
 * -- n: The instruction number to jump to.
 * Returns: Void.
 */
void do_jz(int n)
{
    do_pop();
    if (vm.stack[vm.sp] == 0)
    {
        do_jmp(n);
    }
}


/*
 * Does: Jumps to instruction number n if the top of
 * the stack is not zero.
 * Arguments:
 * -- n: The instruction number to jump to.
 * Returns: Void.
 */
void do_jnz(int n)
{
    do_pop();
    if (vm.stack[vm.sp] != 0)
    {
        do_jmp(n);
    }
}


/*
 * Does: Adds the top two elements of the stack, pops them both,
 * and pushes the sum onto the stack.
 * Arguments: Void.
 * Returns: Void.
 */
void do_add(void)
{
    int sum = 0;
    do_pop();
    sum += vm.stack[vm.sp];
    do_pop();
    sum += vm.stack[vm.sp];
    do_push(sum);
}


/*
 * Does: Subtracts the top two elements of the stack (the element
 * on top of the stack is subtract from the element below
 * it), pops them both,
 * and pushes the difference onto the stack.
 * Arguments: Void.
 * Returns: Void.
 */
void do_sub(void)
{
    int diff = 0;
    do_pop();
    diff -= vm.stack[vm.sp];
    do_pop();
    diff += vm.stack[vm.sp];
    do_push(diff);
}


/*
 * Does: Multiplies the top two elements of the stack, pops them both,
 * and pushes the multiple onto the stack.
 * Arguments: Void.
 * Returns: Void.
 */
void do_mul(void)
{
    int mul = 1;
    do_pop();
    mul *= vm.stack[vm.sp];
    do_pop();
    mul *= vm.stack[vm.sp];
    do_push(mul);
}


/*
 * Does: Divides the top two elements of the stack (the element
 * on top of the stack is divided into the element below
 * it), pops them both,
 * and pushes the fraction onto the stack.
 * Arguments: Void.
 * Returns: Void.
 */
void do_div(void)
{
    int div = 1;
    do_pop();
    div /= vm.stack[vm.sp];
    do_pop();
    div *= vm.stack[vm.sp];
    do_push(div);
}


/*
 * Does: Prints the element on the top of the stack and pops it.
 * Arguments: Void.
 * Returns: Void.
 */
void do_print(void)
{
    do_pop();
    printf("%d\n", vm.stack[vm.sp]);
}


/*
 * Stored program execution.
 */

/*
 * Does: Loads the stored program into the VM.
 * Arguments:
 * -- fp: The name of the file to read.
 * Returns: Void.
 */
void load_program(FILE *fp)
{
    int nread;
    unsigned char *inst = vm.inst;

    do
    {
        /*
         * Read a single byte at a time and load it into the
         * 'vm.insts' array.  'fread' returns the number of bytes read,
         * or 0 if EOF is hit.
         */

        nread = fread(inst, 1, 1, fp);
        inst++;
    }
    while (nread > 0);
}


/*
 * Does: Executes the stored program in the VM.
 * Arguments: Void.
 * Returns: Void.
 */
void execute_program(void)
{
    int val;

    vm.ip = 0;
    vm.sp = 0;

    while (1)
    {
        /*
         * Read each instruction and select what to do based on the
         * instruction.  For each instruction you may also have to
         * read in some number of bytes as the arguments to the
         * instruction.
         */

        switch (vm.inst[vm.ip])
        {
        case NOP:
            /* Skip to the next instruction. */
            vm.ip++;
            break;

        case PUSH:
            vm.ip++;

            /* Read in the next 4 bytes. */
            val = read_n_byte_integer(4);
            do_push(val);
            break;

        case POP:
            vm.ip++;

            do_pop();
            break;

        case LOAD:
            vm.ip++;

            /* Read in the next byte. */
            val = read_n_byte_integer(1);
            do_load(val);
            break;

        case STORE:
            vm.ip++;

            /* Read in the next byte. */
            val = read_n_byte_integer(1);
            do_store(val);
            break;

        case JMP:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jmp(val);
            break;

        case JZ:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jz(val);
            break;

        case JNZ:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jnz(val);
            break;

        case ADD:
            vm.ip++;
            do_add();
            break;

        case SUB:
            vm.ip++;
            do_sub();
            break;

        case MUL:
            vm.ip++;
            do_mul();
            break;

        case DIV:
            vm.ip++;
            do_div();
            break;

        case PRINT:
            vm.ip++;
            do_print();
            break;

        case STOP:
            return;

        default:
            fprintf(stderr, "execute_program: invalid instruction: %x\n",
                    vm.inst[vm.ip]);
            fprintf(stderr, "\taborting program!\n");
            return;
        }
    }
}


/*
 * Does: Runs the program given the file name in which it's stored.
 * Arguments: The file name where the program is stored.
 * Returns: Void.
 */
void run_program(char *filename)
{
    FILE *fp;

    /* Open the file containing the bytecode. */
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "bci.c: run_program: "
               "error opening file %s; aborting.\n", filename);
        exit(1);
    }

    /* Initialize the virtual machine. */
    init_vm();

    /* Read the bytecode into the instruction buffer. */
    load_program(fp);

    /* Execute the program. */
    execute_program();

    /* Clean up. */
    fclose(fp);
}

