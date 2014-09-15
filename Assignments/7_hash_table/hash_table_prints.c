/*
 * CS 11, C Track, lab 7
 *
 * FILE: hash_table.c
 *
 *       Implementation of the hash table functionality.
 *
 */

/*
 * Include the declaration of the hash table data structures
 * and the function prototypes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "memcheck.h"


/*** Hash function. ***/

int hash(char *s)
{
    int i = 0;
    int sum = 0;
    while (s[i] != 0)
    {
        sum += s[i];
        i++;
    }
    return sum % 128;
}


/*** Linked list utilities. ***/

/* Create a single node. */
node *create_node(char *key, int value)
{
    node *result = (node *) malloc(sizeof(node));

    if (result == NULL)
    {
        fprintf(stderr, "Fatal error: out of memory. "
                "Terminating program.\n");
        exit(1);
    }

    result->key = key;
    result->value = value;
    result->next = NULL;

    return result;
}


/* Free all the nodes of a linked list. */
void free_list(node *list)
{
    node *temp;
    char *key;
    while (list != NULL)
    {
        temp = list;
        key = temp->key;
        list = list->next;
        if (key != NULL)
        {
            free(key);
        }
        free(temp);
    }
}


/*** Hash table utilities. ***/

/* Create a new hash table. */
hash_table *create_hash_table()
{
    hash_table * ht = (hash_table *) malloc(sizeof(hash_table));
    ht->slot = (node **) calloc(NSLOTS + 1, sizeof(node));
    return ht;
}


/* Free a hash table. */
void free_hash_table(hash_table *ht)
{
    int i;
    node **slot = ht->slot;
    for (i = 0; i < NSLOTS; i++)
    {
        free_list(slot[i]);
    }
    free(slot);
    free(ht);
}


/*
 * Look for a key in the hash table.  Return 0 if not found.
 * If it is found return the associated value.
 */
int get_value(hash_table *ht, char *key)
{
    node **slot = ht->slot;
    int key_val = hash(key);
    fprintf(stderr, "%d\n", key_val);
    node *list = slot[key_val];
    while (list != NULL)
    {
        if (strcmp(key, list->key) == 0)
        {
            return list->value;
        }
        list = list->next;
    }
    return 0;
}


/*
 * Set the value stored at a key.  If the key is not in the table,
 * create a new node and set the value to 'value'.  Note that this
 * function alters the hash table that was passed to it.
 */
void set_value(hash_table *ht, char *key, int value)
{
    fprintf(stderr, "Setting value\n");
    node **slot = ht->slot;
    int key_val = hash(key);
    node *list = slot[key_val];
    node *orig_list = list;
    node *new_node = NULL;
    if (list == NULL)
    {
        fprintf(stderr, "Creating new node\n");
        list = create_node(key, value);
        slot[key_val] = list;
    }
    else
    {
        fprintf(stderr, "DIFFERENT\n");
        while (list != NULL)
        {
            if (strcmp(key, list->key) == 0)
            {
                list->value = value;
                free(key);
                break;
            }
            list = list->next;
        }

        if (list == NULL)
        {
            new_node = create_node(key, value);
            new_node->next = orig_list;
            slot[key_val] = new_node;
            fprintf(stderr, "DFASSSSSSSSSS\n");
        }
    }
    /*fprintf(stderr, "Start free\n");*/
    /*free_list(list);*/
    /*free_list(orig_list);*/
    /*free_list(new_node);*/
    /*fprintf(stderr, "End free\n");*/
}


/* Print out the contents of the hash table as key/value pairs. */
void print_hash_table(hash_table *ht)
{
    int i;
    node **slot = ht->slot;
    node *list;
    for (i = 0; i < NSLOTS; i++)
    {
        list = slot[i];
        while (list != NULL)
        {
            printf("%s %d\n", list->key, list->value);
            list = list->next;
        }
    }
}

