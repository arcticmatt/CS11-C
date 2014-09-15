/*
 * This program takes an array from the command
 * line and sorts it, using either the minimum
 * element sort or bubble sort. It then
 * prints out each element on a new line, in order.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define MAX_ARRAY_LENGTH 32

/* Function prototypes */
void min_elem_sort(int arr[], int nelems);
void bubble_sort(int arr[], int nelems);
void usage(char *program_name);

int main(int argc, char *argv[])
{
    int i;
    int j;
    int quiet = 0;  /* Value for the "-q" optional argument. */
    int bubble = 0; /* Value for the "-b" optional argument. */
    /* New array that holds all the number args of argv[] */
    int nums[MAX_ARRAY_LENGTH];
    /* Counts number of number elements in argv[] */
    int counter = 0;
    /* The program name, to be passed to usage() */
    char *program_name = argv[0];

    /* Loop through argv[] */
    for (i = 1; i < argc; i++)  /* Skip argv[0] (program name). */
    {
        /*
         * Use the 'strcmp' function to compare the argv values
         * to a string of your choice (here, it's the optional
         * argument "-q").  When strcmp returns 0, it means that the
         * two strings are identical.
         */
        if (strcmp(argv[i], "-q") == 0)  /* Process -q optional argument. */
        {
            quiet = 1;  /* This is used as a boolean value. */
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            /* Process -b optional argument. */
            bubble = 1;  /* This is used as a boolean value. */
        }
        else if (counter < MAX_ARRAY_LENGTH)
        {
            /*
             * If counter is less than MAX_ARRAY_LENGTH, add
             * to nums
             */
            nums[counter] = atoi(argv[i]);
            counter++;
        }
        else
        {
            /*
             * Else (if counter greater than MAX_ARRAY_LENGTH,
             * print usage message and exit
             */
            usage(program_name);
            exit(1);
        }
    }

    /*
     * If number arguments were entered, perform
     * right sort method
     */
    if (counter > 0)
    {
        if (bubble)
        {
            bubble_sort(nums, counter);
        }
        else
        {
            min_elem_sort(nums, counter);
        }
    }
    else
    {
        /*
         * Else (if no number arguments were entered)
         * print usage message and exit
         */
        usage(program_name);
    }

    /* If -q was not entered, print the sorted array */
    if (!quiet)
    {
        for (j = 0; j < counter; j++)
        {
            printf("%d\n", nums[j]);
        }
    }

    return 0;
}

/*
 * Does: Sorts an array with the minimum element
 * sort.
 * Arguments:
 * -- arr - The array to be sorted.
 * -- nelems - The number of elements in the array.
 * Returns: Void.
 */
void min_elem_sort(int arr[], int nelems)
{
    int i;
    int j;
    /* The starting index */
    int start = 0;
    /*
     * The index at which the smallest element not yet
     * moved occurs
     */
    int smallest;
    /* Int used for switching elements */
    int temp;
    /*
     * Loop through the sorting algorithm a
     * maximum of MAX_ARRAY_LENGTH times
     */
    for (i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        smallest = start;
        /*
         * Loop through the whole array and find
         * the index of the smallest element
         */
        for (j = start; j < nelems; j++)
        {
            if (arr[j] < arr[smallest])
            {
                smallest = j;
            }
        }
        /*
         * Switch the smallest element with the
         * current starting element
         */
        temp = arr[start];
        arr[start] = arr[smallest];
        arr[smallest] = temp;
        /* Increment start */
        start++;
        /*
         * If start is greater than or equal
         * to the number of elements,
         * break out of the loop (the array is now sorted)
         */
        if (start >= nelems)
        {
            break;
        }
    }
    /* Check that the array is sorted correctly. */
    for (i = 1; i < nelems; i++)
    {
        assert(arr[i] >= arr[i - 1]);
    }
}

/*
 * Does: Sorts an array using bubble sort.
 * Arguments:
 * -- arr: The array to be sorted.
 * -- nelems: The number of elements in the array.
 * Returns: Void.
 */
void bubble_sort(int arr[], int nelems)
{
    int i;
    /*
     * Boolean that indicates whether or not a
     * change has been made in the array
     */
    int has_changed = 1;
    /* Int used for switching elements */
    int temp;
    /*
     * While loop that executes while changes are
     * being made to the array
     */
    while (has_changed)
    {
        /* Reset has_changed to false */
        has_changed = 0;
        /*
         * Loop through the whole array, switching
         * adjacent elements if the current one
         * is previous than the next one
         */
        for (i = 0; i < nelems - 1; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                /*
                 * If elements are switched, set
                 * has_changed to true
                 */
                has_changed = 1;
            }
        }
    }
    /* Check that the array is sorted correctly. */
    for (i = 1; i < nelems; i++)
    {
        assert(arr[i] >= arr[i - 1]);
    }
}

/*
 * Does: Prints a usage message and exits.
 * Arguments: A string representing the program name.
 * Returns: Void.
 */
void usage(char *program_name)
{
    fprintf(stderr, "usage: %s [-b] [-q] number 1", program_name);
    fprintf(stderr, " [number2 ... ] (maximum 32 numbers)\n");
    exit(1);
}
