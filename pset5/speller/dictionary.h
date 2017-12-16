/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define LETTERS 27
#define CHAR_TO_INT_LOW(c) ((int)(c) - 'a')
#define CHAR_TO_INT_UP(c) ((int)(c) - 'A')

typedef struct node
{
    bool is_word;
    struct node *children[LETTERS];
}
node;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);


node *getNode();

void insert(node *root, char *word);

void freeNode(node *currentnode);

#endif // DICTIONARY_H
