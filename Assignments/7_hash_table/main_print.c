/*
 * CS 11, C Track, week 7
 *
 * FILE: main.c
 *
 *       Tests of the hash table implementation.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "memcheck.h"

#define MAX_WORD_LENGTH 100

int isCompoundWord(hash_table *ht, char *key, int length, int num_chars, int orig_length);
int getStrLength(char *);
void findCompoundWords(hash_table *ht);

void usage(char *progname)
{
    fprintf(stderr, "usage: %s filename\n", progname);
}

void add_to_hash_table(hash_table *ht, char *key)
{
    int v = get_value(ht, key);
    set_value(ht, key, v + 1);
}


int main(int argc, char **argv)
{
    int   nwords;
    FILE *input_file;
    char  word[MAX_WORD_LENGTH];
    char  line[MAX_WORD_LENGTH];
    char *new_word;
    hash_table *ht;

    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }

    /* Make the hash table. */
    ht = create_hash_table();

    /*
     * Open the input file.  For simplicity, we specify that the
     * input file has to contain exactly one word per line.
     */
    input_file = fopen(argv[1], "r");

    if (input_file == NULL)  /* Open failed. */
    {
        fprintf(stderr, "Input file \"%s\" does not exist! "
                        "Terminating program.\n", argv[1]);
        return 1;
    }

    /* Add the words to the hash table until there are none left. */

    while (fgets(line, MAX_WORD_LENGTH, input_file) != NULL)
    {
        /* Clear the contents of 'word'. */
        word[0] = '\0';

        /* Convert the line to a word. */
        nwords = sscanf(line, "%s", word);

        if (nwords != 1)  /* Conversion failed, e.g. due to a blank line. */
        {
            continue;
        }
        else
        {
            /* Copy the word.  Add 1 for the zero byte at the end. */
            new_word = (char *)calloc(strlen(word) + 1, sizeof(char));

            if (new_word == NULL)
            {
                fprintf(stderr, "Error: memory allocation failed! "
                                "Terminating program.\n");
                return 1;
            }

            strcpy(new_word, word);

            /* Add it to the hash table. */
            add_to_hash_table(ht, new_word);
        }
    }

    /* Print out the hash table key/value pairs. */
    /*print_hash_table(ht);*/
    findCompoundWords(ht);

    /* Clean up. */
    free_hash_table(ht);
    fclose(input_file);

    /* Check for memory leaks. */
    print_memory_leaks();

    return 0;
}

void findCompoundWords(hash_table *ht)
{
    int i;
    node **slot = ht->slot;
    node *list;
    char *word;
    int compound_word;
    char *longest_compound;
    char *second_longest_compound;
    int longest = 0;
    int second_longest = 0;
    int compound_word_length;
    int num_compound_words = 0;

    for (i = 0; i < NSLOTS; i++)
    {
        list = slot[i];
        while (list != NULL)
        {
            /* TODO */
            word = list->key;
            printf("\n\n%s %d\n", word, i);
            compound_word_length = getStrLength(word);
            compound_word = isCompoundWord(ht, word, compound_word_length, 0, compound_word_length);
            if (compound_word)
            {
                if (compound_word_length > longest)
                {
                    second_longest = longest;
                    second_longest_compound = longest_compound;
                    longest = compound_word_length;
                    longest_compound = word;
                }
                else if (compound_word_length > second_longest)
                {
                    second_longest = compound_word_length;
                    second_longest_compound = word;
                }
                num_compound_words++;
            }
            list = list->next;
        }
    }
    printf("Longest compound word: %s\n", longest_compound);
    printf("Second longest compound word: %s\n", second_longest_compound);
    printf("Number of compound words: %d\n", num_compound_words);
}

int isCompoundWord(hash_table *ht, char *key, int length, int num_chars, int orig_length)
{
    int i;
    int *indices = (int *) calloc(length, sizeof(int));
    int num_indices = 0;
    char *substr;
    int index;
    int is_compound;
    printf("START isCompoundWord\n");

    if (num_chars == orig_length)
    {
        free(indices);
        printf("return TRUE\n");
        return 1;
    }
    for (i = 1; i <= length && i < orig_length; i++)
    {
        substr = (char *) calloc(i + 1, sizeof(char));
        memcpy(substr, &key[0], i);
        substr[i] = '\0';
        printf("Substring: %s\n", substr);
        if (get_value(ht, substr) != 0)
        {
            indices[num_indices] = i;
            printf("Index: %d\n", i);
            num_indices++;
        }
        free(substr);
    }
    i = 0;
    while (indices[i] != 0)
    {
        printf("Length: %d\n", length);
        index = indices[i];
        printf("Loop through indices, index %d is %d\n", i, index);
        printf("Index remainder length: %d\n", index % length);
        substr = (char *) calloc(length + 1 - (index % length), sizeof(char));
        memcpy(substr, &key[index % length], length - (index % length));
        substr[length - (index % length)] = '\0';
        printf("Substring in while: %s\n", substr);
        num_chars += index;
        printf("num chars: %d\n", num_chars);
        printf("Orig length: %d\n", orig_length);
        /*return isCompoundWord(ht, substr, length - index, num_chars, orig_length);*/
        is_compound = isCompoundWord(ht, substr, length - index, num_chars, orig_length);
        if (is_compound)
        {
            free(substr);
            free(indices);
            return is_compound;
        }
        i++;
        num_chars -= index;
        free(substr);
    }
    printf("return FALSE\n");
    free(indices);
    printf("freed indices\n");
    return 0;
}

int getStrLength(char *str)
{
    int i = 0;
    int length = 0;
    while (str[i] != 0)
    {
        length++;
        i++;
    }
    return length;
}


