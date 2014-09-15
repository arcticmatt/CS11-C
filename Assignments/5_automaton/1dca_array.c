/*
 * This program simulates a simple one-dimensional cellular
 * automaton, only using array operations (as opposed to pointer operations).
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "memcheck.h"

void usage(char *program_name);
void make_next_gen(int *numbers, int *numbers2, int length);
void print_int_array(int *array, int length);

int main(int argc, char *argv[])
{
    /* The program name, to be passed to usage() */
    char *program_name = argv[0];
    int num_cells;   /* Number of cells in automaton */
    int num_gens;    /* Number of gens for automaton */
    int i;
    int zero_or_one; /* Temp int to fill array */
    int *numbers;    /* Array to be used for automaton */
    int *numbers2;   /* Array used to generate next gen */

    /* If not the correct number of elements, print usage and exit */
    if (argc != 3)
    {
        usage(program_name);
    }

    num_cells = atoi(argv[1]); /* Get num cells from args */
    num_gens = atoi(argv[2]);  /* Get num gens from args */
    /* Dynamically allocate memory for the arrays */
    numbers = (int *) calloc(num_cells, sizeof(int));
    numbers2 = (int *) calloc(num_cells, sizeof(int));
    /* Check to make sure the memory is correctly allocated */
    if (numbers == NULL || numbers2 == NULL)
    {
        fprintf(stderr, "Error! Memory allocation failed!\n");
        exit(1);
    }

    /* Seed the random generator */
    srand(time(NULL));
    /* Fill both arrays with random zeroes and ones */
    for (i = 0; i < num_cells; i++)
    {
        zero_or_one = rand() % 2;
        numbers[i] = zero_or_one;
        numbers2[i] = zero_or_one;
    }

    /* Print the initial array */
    print_int_array(numbers, num_cells);
    /* Loop that makes a new gen then prints it num_gens number of times */
    for (i = 0; i < num_gens; i++)
    {
        make_next_gen(numbers, numbers2, num_cells);
        print_int_array(numbers, num_cells);
    }

    /* Free the memory */
    free(numbers);
    free(numbers2);
    print_memory_leaks();
    return 0;
}

/*
 * Does: Makes the next generation array.
 * Arguments:
 * -- numbers: The main automaton array (to be printed).
 * -- numbers2: The temp array that is used to make the new gen.
 * -- length: The length of both arrays.
 * Returns: Void.
 */
void make_next_gen(int *numbers, int *numbers2, int length)
{
    int i;

    /* Loop through numbers1 and make the next gen in numbers2 */
    for (i = 0; i < length; i++)
    {
        if (i == 0)
        {
            if (numbers[i] == 0 && numbers[i + 1] == 1)
            {
                numbers2[i] = 1;
            }
            else
            {
                numbers2[i] = 0;
            }
        }
        else if (i > 0 && i < length - 1)
        {
            if (numbers[i] == 0 && ((numbers[i - 1] + numbers[i +1]) == 1))
            {
                numbers2[i] = 1;
            }
            else
            {
                numbers2[i] = 0;
            }
        }
        else
        {
            if (numbers[i] == 0 && numbers[i - 1] == 1)
            {
                numbers2[i] = 1;
            }
            else
            {
                numbers2[i] = 0;
            }
        }
    }

    /* Copy elements from numbers2 to numbers */
    for (i = 0; i < length; i++)
    {
        numbers[i] = numbers2[i];
    }
}

/*
 * Does: Prints an array.
 * Arguments:
 * -- array: The array to be printed.
 * -- length: The length of the array.
 * Returns: Void.
 */
void print_int_array(int *array, int length)
{
    int i;
    /* Print a '.' instead of zeroes and a '*' instead of 1. */
    for (i = 0; i < length; i++)
    {
        if (array[i] == 0)
        {
            printf(".");
        }
        else
        {
            printf("*");
        }
    }
    printf("\n");
}

/*
 * Does: Prints a usage message and exits.
 * Arguments: A string representing the program name.
 * Returns: Void.
 */
void usage(char *program_name)
{
    fprintf(stderr, "usage: %s number_cells, number_gens\n", program_name);
    exit(1);
}
