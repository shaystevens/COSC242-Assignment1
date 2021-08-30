#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>

typedef struct htablerec *htable;
/* collision resolution strategies linear probing or double hashing */
typedef enum hashing_e { LINEAR_P, DOUBLE_H } hashing_t;

extern void   htable_free(htable h);
extern int    htable_insert(htable h, char *str);
extern htable htable_new(int capacity, hashing_t hashing_type);
extern void   htable_print(htable h, void print_info(int freq, char *word));
extern void   htable_print_entire_table(htable h, FILE *stream);
extern void   htable_print_stats(htable h, FILE *stream, int num_stats);

#endif
