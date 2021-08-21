#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"

struct htablerec {
    int capacity;
    int num_keys;
    int *frequencies;
    char **keys;
};

htable htable_new(int capacity) {
    int i;
    htable h = emalloc(sizeof *h);
    h->capacity = capacity;
    h->num_keys = 0;
    h->frequencies = emalloc(h->capacity * sizeof h->frequencies[0]);
    h->keys = emalloc(h->capacity * sizeof h->keys[0]);
    for (i = 0; i < h->capacity; i++){
        h->frequencies[i] = 0;
        h->keys[i] = NULL;
    }
    return h;
    
}

static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;

    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;
}

static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

int htable_insert(htable h, char *str){
    unsigned int int_string;
    int indexed_string;
    char *copied_str;

    copied_str =  emalloc(256 * sizeof copied_str[0]);
    strcpy(copied_str, str);

    int_string = htable_word_to_int(copied_str);
    indexed_string = int_string % h->capacity;

    /* key doesn't exist in table */
    if (h->keys[indexed_string] == NULL){
        h->keys[indexed_string] = copied_str;
        h->frequencies[indexed_string] = 1;
        h->num_keys++;
        return 1;

        /* key exists already */
    } else if (strcmp(h->keys[indexed_string], copied_str) == 0){
        h->frequencies[indexed_string]++;
        return h->frequencies[indexed_string];

        /* another key in first position */
    } else {
        int i = indexed_string;
        int collisions = 1;
        int dhash_step = htable_step(h, int_string);
        while (strcmp(h->keys[i], copied_str) != 0){
            i = (indexed_string + (collisions * dhash_step))%h->capacity; 

            /* when it goes back to the same position it started at */
            if (i == indexed_string){
                return 0;
            /* there is an empty spot where it should be so key doesn't exist */
            } else if (h->keys[i] == NULL){
                h->keys[i] = copied_str;
                h->frequencies[i] = 1;
                h->num_keys++;
                return 1;
            }
            collisions++;
        }
        h->frequencies[i]++;
        return h->frequencies[i];
    }
}

void htable_print(htable h, FILE *stream) {
    int i = 0;
    for (i = 0; i < h->capacity; i++){
        if ((h->keys[i]) != NULL){
            fprintf(stream, "%d\t%s\n", h->frequencies[i],
                   h->keys[i]);
        }
    }
}

void htable_free(htable h) {
    int i = 0;
    for (i = 0; i < h->capacity; i++){
        if (h->keys[i] != NULL){
            free(h->keys[i]);
        }
    }
    
    free(h->keys);
    free(h->frequencies);
    free(h);
}

int htable_search(htable h, char *str){
    int collisions = 0;
    int i;
    unsigned int int_string;
    int indexed_string;
    char *copied_str;
    int dhash_step;

    copied_str =  emalloc(256 * sizeof copied_str[0]);
    strcpy(copied_str, str);

    int_string = htable_word_to_int(copied_str);
    indexed_string = int_string % h->capacity;

    i = indexed_string;
    dhash_step = htable_step(h, int_string);

    while ((h->keys[i] != NULL) && (strcmp(h->keys[i], copied_str) != 0)
           && (collisions != h->capacity)){
        i = (indexed_string + (collisions * dhash_step))%h->capacity;
        collisions++;
    }
    
    if (collisions == h->capacity){
        return 0;
    } else {
        return h->frequencies[i];
    }
}


