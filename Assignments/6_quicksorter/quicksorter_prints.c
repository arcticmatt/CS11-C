/*
 * This program takes ints from the command
 * line, puts them into a linked list,
 * and sorts it using quicksort. It then
 * prints out each element on a new line, in order.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "linked_list.h"
#include "memcheck.h"

/* Function prototypes */
node * quicksort(node *list);
void usage(char *program_name);

int main(int argc, char *argv[])
{
    int i;
    int data;
    int quiet = 0;  /* Value for the "-q" optional argument. */
    int counter = 0;
    /* The program name, to be passed to usage() */
    char *program_name = argv[0];
    node *list;
    node *sorted_list;
    fprintf(stderr, "started");

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
        else
        {
            data = atoi(argv[i]);
            if (counter == 0)
            {
                list = create_node(data, NULL);
            }
            else
            {
                list = create_node(data, list);
            }
            counter++;
        }
    }

    /*
     * If number arguments were entered, perform
     * right sort method
     */
    if (counter > 0)
    {
        print_list(list);
        sorted_list = quicksort(list);
        printf("list sorted\n");
        free_list(list);
        list = sorted_list;
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
        printf("ACTUALLY PRINTING\n");
        print_list(list);

    }

    free_list(list);
    print_memory_leaks();
    return 0;
}

node *
quicksort(node *list)
{
    node *first_node = NULL;
    node *left_half = NULL;
    node *sorted_left_half = NULL;
    node *right_half = NULL;
    node *sorted_right_half = NULL;
    node *head = NULL;
    node *sorted_list = NULL;
    node *sorted_list2 = NULL;
    int pivot;
    int head_data;
    int left_counter = 0;
    int right_counter = 0;
    printf("Quicksort\n");
    if (list == NULL || list->next == NULL)
    {
        return copy_list(list);
    }
    else
    {
        first_node = copy_list(list);
        free_list(first_node->next);
        first_node->next = NULL;

        pivot = first_node->data;
        head = list->next;
        while (head != NULL)
        {
            head_data = head->data;
            if (head_data < pivot)
            {
                if (left_counter == 0)
                {
                    left_half = create_node(head_data, NULL);
                }
                else
                {
                    left_half = create_node(head_data, left_half);
                }
                left_counter++;
                printf("Left half\n");
                print_list(left_half);
            }
            else
            {
                if (right_counter == 0)
                {
                    right_half = create_node(head_data, NULL);
                }
                else
                {
                    right_half = create_node(head_data, right_half);
                }
                right_counter++;
                printf("Right half\n");
                print_list(right_half);
            }
            head = head->next;
        }
        printf("NULL head\n");

        printf("Quicksort left half\n");
        sorted_left_half = quicksort(left_half);
        free_list(left_half);
        left_half = sorted_left_half;

        printf("Quicksort right half\n");
        sorted_right_half = quicksort(right_half);
        free_list(right_half);
        right_half = sorted_right_half;

        sorted_list = append_lists(left_half, first_node);
        sorted_list2 = append_lists(sorted_list, right_half);
        printf("Printing list\n");
        print_list(sorted_list);
        assert(is_sorted(sorted_list));
        /*free_list(list);*/
        free_list(first_node);
        free_list(left_half);
        free_list(right_half);
        free_list(sorted_list);
        printf("END quicksort\n");
        return sorted_list2;
    }
}

/*
 * Does: Prints a usage message and exits.
 * Arguments: A string representing the program name.
 * Returns: Void.
 */
void usage(char *program_name)
{
    fprintf(stderr, "usage: %s [-q] number 1 [number2 ... ] \
            (as many numbers as you want)\n", program_name);
    exit(1);
}
