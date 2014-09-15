#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#define MAX_WORD_LENGTH 100

int isCompoundWord(hash_table *ht, char *key, int length, int num_chars, int orig_length);
int getStrLength(char *);
void findCompoundWords(hash_table *ht);
void usage(char *progname);
void add_to_hash_table(hash_table *ht, char *key);


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

    /* Open input file (one word on each line) */
    input_file = fopen(argv[1], "r");

    if (input_file == NULL)
    {
        fprintf(stderr, "Input file \"%s\" does not exist! "
                        "Terminating program.\n", argv[1]);
        return 1;
    }

    /* Add words to hash table */
    while (fgets(line, MAX_WORD_LENGTH, input_file) != NULL)
    {
        /* Reset 'word' */
        word[0] = '\0';

        /* Convert line to a word. */
        nwords = sscanf(line, "%s", word);

        if (nwords != 1)  /* Conversion failed */
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

    /*
     * Find the two longest compound words and the
     * number of compound words in the hash table.
     */
    findCompoundWords(ht);

    /* Clean up. */
    free_hash_table(ht);
    fclose(input_file);

    return 0;
}


/*
 * Does: Counts number of compound words in a hash table,
 * and prints the two longest ones. A compound word here is defined as
 * a word that can be made by concatenating copies of shorter words that are
 * also in the hash table.
 * Arguments:
 * -- ht: The hash table to search.
 * Returns: Void.
 */
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

    /*
     * Loop through entire hash table, checking each word to
     * see if it is a compound word, and if it is, comparing its length
     * against the first and second longest compound words.
     */
    for (i = 0; i < NSLOTS; i++)
    {
        list = slot[i];
        /* Iterate through the linked list */
        while (list != NULL)
        {
            /* Get the word */
            word = list->key;
            compound_word_length = getStrLength(word);
            compound_word = isCompoundWord(ht, word, compound_word_length, 0, compound_word_length);
            /* If word is a compound word, compare lengths */
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
            /* Move to next node in linked list */
            list = list->next;
        }
    }
    printf("Longest compound word: %s\n", longest_compound);
    printf("Second longest compound word: %s\n", second_longest_compound);
    printf("Number of compound words: %d\n", num_compound_words);
}


/*
 * Does: A recursive function that
 * determines if a word in a hash table is a compound word (can be
 * made by concatenating shorter words in the hash table).
 * Arguments:
 * -- ht: The hash table to be searched.
 * -- key: The word we will consider (to see if it is compound)
 * -- length: The length of key.
 * -- num_chars: The number of characters of the word that have been
 *    transversed.
 * -- orig_length: The original length of the key.
 * Returns: An int representing a boolean, determining if the word is compound.
 */
int isCompoundWord(hash_table *ht, char *key, int length, int num_chars, int orig_length)
{
    int i;
    /*
     * This will hold the indices of key that represent the end
     * of a shorter word (e.g. in dogcat, the index 3 will represent
     * g, the end of the word 'dog.'
     */
    int *indices = (int *) calloc(length, sizeof(int));
    int num_indices = 0;
    char *substr;
    int index;
    int is_compound;

    /*
     * Base Case: If the entire key has been transversed,
     * return true.
     */
    if (num_chars == orig_length)
    {
        free(indices);
        return 1;
    }
    /*
     * Find all possible shorter words (that exist in the hash table)
     * that can be made from key, and put them into indices.
     * Note that these words must be made
     * by starting at the beginning of key.
     */
    for (i = 1; i <= length && i < orig_length; i++)
    {
        substr = (char *) calloc(i + 1, sizeof(char));
        memcpy(substr, &key[0], i);
        substr[i] = '\0';
        if (get_value(ht, substr) != 0)
        {
            indices[num_indices] = i;
            num_indices++;
        }
        free(substr);
    }

    i = 0;
    /*
     * Loop through indices and recursively call this function on each
     * substring of shorter words.
     */
    while (indices[i] != 0)
    {
        index = indices[i];

        /* Get the substring that occurs after the index */
        substr = (char *) calloc(length + 1 - (index % length), sizeof(char));
        memcpy(substr, &key[index % length], length - (index % length));
        substr[length - (index % length)] = '\0';

        num_chars += index;
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
    free(indices);
    return 0;
}


/*
 * Does: Gets length of a char array.
 * Arguments:
 * -- str: The char array whose length is wanted.
 * Returns: The length.
 */
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


/*
 * Does: Prints usage message.
 * Arguments:
 * -- progname: The name of the program.
 * Returns: Void.
 */
void usage(char *progname)
{
    fprintf(stderr, "usage: %s filename\n", progname);
}


/*
 * Does: Adds a key to a hash table.
 * Arguments:
 * -- ht: The hash table to add to.
 * -- key: The key to add.
 * Returns: Void.
 */
void add_to_hash_table(hash_table *ht, char *key)
{
    int v = get_value(ht, key);
    set_value(ht, key, v + 1);
}
