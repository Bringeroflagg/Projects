#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define LIST_MAXLEN_TEXT 50
#define MAX_LINE_LEN 100

// struct for sentences lines and exception words
typedef struct node_t {
    char *text;
    struct node_t  *next;
} node_t;

// struct for words and line number
typedef struct node_w {
    char *word;
    int line_number;
    struct node_w   *next;
} node_w;

// added new_w and add_inorder
node_w *new_w(char *, int );
node_w *add_inorder(node_w *, node_w *);
node_t *new_node(char *);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void    apply(node_t *, void(*fn)(node_t *, void *), void *arg);

#endif
