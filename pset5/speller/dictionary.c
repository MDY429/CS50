/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

node *root = NULL;
unsigned int totalword = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *ptr = root;

    for(int i = 0, length = strlen(word), index; i < length; i++)
    {
        if(word[i] >= 'a' && word[i] <= 'z')
            index = (CHAR_TO_INT_LOW(word[i]));
        else if(word[i] >= 'A' && word[i] <= 'Z')
            index = (CHAR_TO_INT_UP(word[i]));
        else
            index = 26;

        //If NULL, word is misspeeled.
        if(ptr->children[index] == NULL)
            return false;

        //move to next letter
        ptr = ptr->children[index];

        //Check the end of letter whether is_word is true.
        if(i == length - 1 && ptr->is_word == false)
            return false;
    }

    return true;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    char *word = NULL;

    FILE *fp = fopen(dictionary, "r");
    if(fp == NULL)
    {
        return false;
    }

    word = malloc(256 * sizeof(char));
    root = getNode();

    if(root == NULL)
    {
        printf("Cannot allocate the memory.\n");
        return false;
    }

    while(fscanf(fp, "%s", word) != EOF)
    {
        insert(root, word);
        totalword++;
    }

    free(word);
    fclose(fp);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //retuen the total of words.
    return totalword;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int i = 0; i < LETTERS; i++)
    {
        if(root->children[i] != 0)
        {
            //Try to free all the childern pointers of root.
            freeNode(root->children[i]);
        }
    }

    //free the root node.
    free(root);

    return true;
}


/**
 * Let the word insert to root node.
 */
void insert(node *root, char *word)
{
    node *ptr = root;

    for(int i = 0, length = strlen(word), index; i < length; i++)
    {
        index = (word[i] >= 'a' && word[i] <= 'z') ? (CHAR_TO_INT_LOW(word[i])) : 26;

        //If next children is NULL, should be allocate new node to save next letter.
        if(ptr->children[index] == NULL)
            ptr->children[index] = getNode();

        //Let ptr point to next children
        ptr = ptr->children[index];
    }

    //End of word, let is_word be true.
    ptr->is_word = true;
}

/**
 * Allocate memory for new Node.
 */
node *getNode()
{
    node *pNode = NULL;

    pNode = malloc(sizeof(node));

    //If malloc success, it should be initial.
    if (pNode != NULL)
    {
        pNode->is_word = false;

        for(int i = 0; i < LETTERS; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

/**
 * Free the node.
 */
void freeNode(node *currentnode)
{
    for(int i = 0; i < LETTERS; i++)
    {
        //Travel to lowest possiable node, free all pointers in children.
        if(currentnode->children[i] != NULL)
        {
            freeNode(currentnode->children[i]);
            currentnode->children[i] = NULL;
        }
    }

    //free the current node.
	free(currentnode);

	return;
}

