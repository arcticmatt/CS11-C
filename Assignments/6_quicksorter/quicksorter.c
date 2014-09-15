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
    int quiet = 0;   /* Value for the "-q" optional argument. */
    int counter = 0; /* Counts number of number args */
    /* The program name, to be passed to usage() */
    char *program_name = argv[0];
    node *list = NULL;
    node *sorted_list;

    /* Loop through argv[] */
    for (i = 1; i < argc; i++)  /* Skip argv[0] (program name). */
    {
        if (strcmp(argv[i], "-q") == 0)  /* Process -q optional argument. */
        {
            quiet = 1;  /* This is used as a boolean value. */
        }
        else /* Process number args */
        {
            /*
             * Add node to list. If the first
             * node, make its next field point
             * to NULL.
             */
            data = atoi(argv[i]);
            list = create_node(data, list);
            counter++;
        }
    }

    /* If number arguments were entered, quicksort. */
    if (counter > 0)
    {
        sorted_list = quicksort(list);
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

    /* If -q was not entered, print the sorted list */
    if (!quiet)
    {
        print_list(list);

    }

    free_list(list);
    print_memory_leaks();
    return 0;
}

/*
 * Does: Sorts a linked list using the quicksort algorithm.
 * Arguments:
 * -- list: A pointer to the first node of the linked list to
 *          be sorted.
 * Returns: A pointer to the first node of the sorted linked list.
 */
node *
quicksort(node *list)
{
    node *first_node;
    node *left_half = NULL;
    node *sorted_left_half = NULL;
    node *right_half = NULL;
    node *sorted_right_half = NULL;
    node *head;
    node *sorted_list;
    node *sorted_list2;
    int pivot;
    int head_data;

    /* If a null list or single node, return a copy of it */
    if (list == NULL || list->next == NULL)
    {
        return copy_list(list);
    }
    else /* Else, partition the list into left and right lists */
    {
        first_node = copy_list(list);
        free_list(first_node->next);
        first_node->next = NULL;

        /* Value with which data fields are compared against */
        pivot = first_node->data;
        head = list->next;
        while (head != NULL)
        {
            head_data = head->data;
            if (head_data < pivot)
            {
                /* Add nodes to left_half, creating the left_half list */
                left_half = create_node(head_data, left_half);
            }
            else
            {
                /* Add nodes to right_half, creating the right_half list */
                right_half = create_node(head_data, right_half);
            }
            head = head->next;
        }
        /* Sort the left_half recursively using quicksort */
        sorted_left_half = quicksort(left_half);
        free_list(left_half);
        left_half = sorted_left_half;

        /* Sort the right_half recursively using quicksort */
        sorted_right_half = quicksort(right_half);
        free_list(right_half);
        right_half = sorted_right_half;

        /*
         * Append the left and right halves with the first node in the
         * middle to create a sorted list.
         */
        sorted_list = append_lists(left_half, first_node);
        sorted_list2 = append_lists(sorted_list, right_half);

        /* Free memory */
        free_list(sorted_list);
        free_list(first_node);
        free_list(left_half);
        free_list(right_half);

        assert(is_sorted(sorted_list2));
        return sorted_list2;
    }
}

/*
 * Does: Prints a usage message and exits.
 * Arguments:
 * -- program_name: A string representing the program name.
 * Returns: Void.
 */
void usage(char *program_name)
{
    fprintf(stderr, "usage: %s [-q] number 1 [number2 ... ] \
            (as many numbers as you want)\n", program_name);
    exit(1);
}
