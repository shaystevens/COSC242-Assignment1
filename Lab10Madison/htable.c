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

int htable_insert(htable h, char *str){
    unsigned int int_string;
    int indexed_string;
    char *copied_str;

    copied_str =  emalloc(256 * sizeof copied_str[0]);
    strcpy(copied_str, str);

    int_string = htable_word_to_int(copied_str);
    indexed_string = int_string % h->capacity;

    if (h->keys[indexed_string] == NULL){
        h->keys[indexed_string] = copied_str;
        h->frequencies[indexed_string] = 1;
        h->num_keys++;
        return 1;
    } else if (strcmp(h->keys[indexed_string], copied_str) == 0){
        h->frequencies[indexed_string]++;
        return h->frequencies[indexed_string];
    } else {
        int i = indexed_string;
        while (strcmp(h->keys[i], copied_str) != 0){
            i++;
            i = i % h->capacity;
            if (i == indexed_string){
                return 0;
            } else if (h->keys[i] == NULL){
                h->keys[i] = copied_str;
                h->frequencies[i] = 1;
                h->num_keys++;
                return 1;
            }
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

    copied_str =  emalloc(256 * sizeof copied_str[0]);
    strcpy(copied_str, str);

    int_string = htable_word_to_int(copied_str);
    indexed_string = int_string % h->capacity;

    i = indexed_string;

    while ((h->keys[i] != NULL) && (strcmp(h->keys[i], copied_str) != 0)
           && (collisions != h->capacity)){
        i++;
        i = i % h->capacity;
        collisions++;
    }

    if (collisions == h->capacity){
        return 0;
    } else {
        return h->frequencies[i];
    }
}
