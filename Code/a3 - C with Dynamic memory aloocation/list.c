/** @file list.c
 *  @brief Implementation of a linked list.
 *
 * Based on the implementation approach described in "The Practice
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "list.h"
#include <stdbool.h>

/**
 * Function:  new_node
 * -------------------
 * @brief  Allows to dynamically allocate memory for a new node to be added to the linked list.
 *
 * This function should confirm that the argument being passed is not NULL (i.e., using the assert library). Then,
 * It dynamically allocates memory for the new node using emalloc(), and assign values to attributes associated with the node (i.e., val and next).
 *
 * @param val The value to be associated with the node.
 *
 * @return node_t* A pointer to the node created.
 *
 */


node_t *new_node(song_data *song)
{
    assert(song != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    temp->song = song;
    temp->next = NULL;

    return temp;
}

/**
 * Function:  add_front
 * --------------------
 * @brief  Allows to add a node at the front of the list.
 *
 * @param list The list where the node will be added (i.e., a pointer to the first element in the list).
 * @param new The node to be added to the list.
 *
 * @return node_t* A pointer to the new head of the list.
 *
 */
node_t *add_front(node_t *list, node_t *new)
{
    new->next = list;
    return new;
}

/**
 * Function:  add_end
 * ------------------
 * @brief  Allows to add a node at the end of the list.
 *
 * @param list The list where the node will be added (i.e., a pointer to the first element in the list).
 * @param new The node to be added to the list.
 *
 * @return node_t* A pointer to the head of the list.
 *
 */
node_t *add_end(node_t *list, node_t *new)
{
    node_t *curr;

    if (list == NULL)
    {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next)
        ;
    curr->next = new;
    new->next = NULL;
    return list;
}

/**
 * Function:  add_inorder
 * ----------------------
 * @brief  Allows to add a new node to the list respecting an order.
 *
 * @param list The list where the node will be added (i.e., a pointer to the first element in the list).
 * @param new The node to be added to the list.
 *
 * @return node_t* A pointer to the node created.
 *
 */
node_t *add_inorder(node_t *list, node_t *new, char* order_by)
{
    node_t *prev = NULL;
    node_t *curr = NULL;

    if (list == NULL)
    {
        return new;
    }

    for (curr = list; curr != NULL; curr = curr->next)
    {
        if (strcmp(order_by, "STREAMS") == 0){
            if (new->song->streams > curr->song->streams){
                prev = curr;
            }
            else{
                break;
            }
        }
        else if (strcmp(order_by, "NO_SPOTIFY_PLAYLISTS") == 0)
        {
            if (new->song->spotify > curr->song->spotify){
                prev = curr;
            }
            else{
                break;
            }
        }
        else{
            if (new->song->apple > curr->song->apple){
                prev = curr;
            }
            else{
                break;
            }
        }
    }

    new->next = curr;

    if (prev == NULL)
    {
        return (new);
    }
    else
    {
        prev->next = new;
        return list;
    }
}


// Adds a new node in reverse order based on the specified ordering criteria
node_t *add_rev_order(node_t *list, node_t *new, char* order_by) {
    node_t *prev = NULL; // Initialize a pointer to track the previous node
    node_t *curr = list; // Start with the current node as the head of the list

    // Iterate through the list until the correct position for the new node is found
    while (curr != NULL && is_less_than_reverse_order(new->song, curr->song, order_by)) {
        prev = curr; // Update prev to current node before moving to next
        curr = curr->next; // Move to the next node in the list
    }

    new->next = curr; // Insert new node before curr (maintaining reverse order)

    if (prev == NULL) {
        // If prev is still NULL, new node should be the new head of the list
        return new;
    } else {
        // Otherwise, insert the new node after prev and return the unchanged head of the list
        prev->next = new;
        return list;
    }
}

// Determines if the first song should come before the second song in reverse order based on the specified criteria
bool is_less_than_reverse_order(song_data* first, song_data* second, char* order_by) {
    // Compare based on stream counts if "STREAMS" is the specified order
    if (strcmp(order_by, "STREAMS") == 0) {
        return first->streams < second->streams;
    } else if (strcmp(order_by, "NO_SPOTIFY_PLAYLISTS") == 0) {
        // Compare based on Spotify playlist counts if "NO_SPOTIFY_PLAYLISTS" is specified
        return first->spotify < second->spotify;
    } else {
        // Default to comparing based on Apple playlist counts for any other order_by value
        return first->apple < second->apple;
    }
}


/**
 * Function:  peek_front
 * ---------------------
 * @brief  Allows to get the head node of the list.
 *
 * @param list The list to get the node from.
 *
 * @return node_t* A pointer to the head of the list.
 *
 */
node_t *peek_front(node_t *list)
{
    return list;
}

/**
 * Function:  remove_front
 * -----------------------
 * @brief  Allows removing the head node of the list.
 *
 * @param list The list to remove the node from.
 *
 * @return node_t* A pointer to the head of the list.
 *
 */
node_t *remove_front(node_t *list)
{
    if (list == NULL)
    {
        return NULL;
    }

    return list->next;
}

/**
 * Function: apply
 * --------------
 * @brief  Allows to apply a function to the list.
 *
 * @param list The list (i.e., pointer to head node) where the function will be applied.
 * @param fn The pointer of the function to be applied.
 * @param arg The arguments to be applied.
 *
 */
void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for (; list != NULL; list = list->next)
    {
        (*fn)(list, arg);
    }
}
