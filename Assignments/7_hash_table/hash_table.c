#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"


/*** Hash function. ***/
/*
 * Does: Calculates a hash value for a char array using
 * DBJ hash algorithm.
 * Arguments:
 * -- s: The char array to be hashed.
 *  Returns: An integer between 0 and 127 inclusive,
 *  obtained by summing all the characters in the array
 *  and taking their modulus 128.
 */
unsigned long hash(unsigned char *s)
{
    unsigned long hash = 5381;
    int c;

    while (c = *s++)
        hash = ((hash << 5) + hash) + c;

    return hash % NSLOTS;
}


/*** Linked list utilities. ***/
/*
 * Does: Creates a single node with a next value of NULL.
 * Arguments:
 * -- key: The key of the new node.
 * -- value: The value of the new node.
 * Returns: The new node.
 */
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


/*
 * Does: Frees all the nodes of a linked list.
 * Arguments:
 * -- list: The list to free.
 * Returns: Void.
 */
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
/*
 * Does: Creates a new hash table.
 * Arguments: None.
 * Returns: The new hash table.
 */
hash_table *create_hash_table()
{
    /* Initialize the hash table */
    hash_table * ht = (hash_table *) malloc(sizeof(hash_table));
    /* Initialize the slot of the hash table */
    ht->slot = (node **) calloc(NSLOTS + 1, sizeof(node));
    return ht;
}


/* Does: Frees all the allocated memory that was
 * created by a hash table.
 * Arguments:
 * -- ht: The hash table whose memory will be freed.
 * Returns: Void.
 */
void free_hash_table(hash_table *ht)
{
    int i;
    node **slot = ht->slot;
    /* Free each linked list of nodes in the hash table */
    for (i = 0; i < NSLOTS; i++)
    {
        free_list(slot[i]);
    }
    /* Free the slot and hash table themselves */
    free(slot);
    free(ht);
}


/*
 * Does: Gets the value of a key in the hash table.
 * Arguments:
 * -- ht: The hash table to be searched.
 * -- key: The key whose value is to be retrieved.
 * Returns: The value of the key, or 0 if not found.
 */
int get_value(hash_table *ht, char *key)
{
    node **slot = ht->slot;
    int key_val = hash(key);
    node *list = slot[key_val];
    /* Loop through the desired list to look for the desired key */
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
 * Does: Sets the value stored at a key. If the key is not in the table,
 * create a new node and set the value to 'value'. Note that this
 * function alters the hash table that was passed to it.
 * Arguments:
 * -- ht: The hash table to be changed.
 * -- key: The key whose value will be set.
 * -- value: The value to be set.
 * Returns: Void.
 */
void set_value(hash_table *ht, char *key, int value)
{
    node **slot = ht->slot;
    int key_val = hash(key);
    node *list = slot[key_val];
    node *orig_list = list;
    node *new_node = NULL;
    if (list == NULL)
    {
        list = create_node(key, value);
        slot[key_val] = list;
    }
    else
    {
        /*
        * Search for existing node with a key matching
         * that of the key in the args. If found,
         * set the value of that node to the new value.
         */
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

        /*
         * If no existing node was found (list will equal NULL)
         * make a new node with the desired key and value,
         * and set its next value to the original list. Then
         * update the slot of the hash table to respect these changes.
         */
        if (list == NULL)
        {
            new_node = create_node(key, value);
            new_node->next = orig_list;
            slot[key_val] = new_node;
        }
    }
}


/*
 * Does: Prints out the contents of the hash table as key/value pairs.
 * Arguments:
 * -- ht: The hash table to be printed.
 * Returns: Void.
 */
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
            printf("%s %d\n", list->key, i);
            list = list->next;
        }
    }
}
