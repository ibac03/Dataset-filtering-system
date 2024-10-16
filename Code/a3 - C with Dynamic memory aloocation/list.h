/** @file listy.h
 *  @brief Function prototypes for the linked list.
 */
#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define MAX_WORD_LEN 50
#include <stdbool.h>

typedef struct{
    char* data;
    char* filter;
    char* value;
    char* order_by;
    char* order;
    int limit;
} args;


typedef struct {
    char *track_name;
    char *artist_name;
    int year;
    int month;
    int day;
    int spotify;
    long streams;
    int apple;
} song_data;


typedef struct node_t
{
    song_data *song;
    struct node_t *next;
} node_t;

/**
 * Function protypes associated with a linked list.
 */
node_t *new_node(song_data *song);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *add_inorder(node_t *, node_t *, char*);
node_t *add_rev_order(node_t *, node_t *, char*);
bool is_less_than_reverse_order(song_data* first, song_data* second, char* order_by);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void apply(node_t *, void (*fn)(node_t *, void *), void *arg);

#endif
