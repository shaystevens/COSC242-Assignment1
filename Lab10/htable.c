#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"

struct htablerec {
    int capacity;
    int num_keys;
    int *frequency;
    char **keys;
};

htable htable_new(int capacity){
    int i;
    htable result = emalloc(sizeof *result);
    result->capacity = capacity;
    result->num_keys = 0;
    result->keys = emalloc(sizeof(result->keys[0]) * capacity);
    result->frequency =  emalloc(sizeof(int) * capacity);

    for(i=0; i<capacity; i++){
        result->keys[i] = NULL;
        result->frequency[i] = 0;
    }
    return result;
}

void htable_free(htable h){
    int i;
    for(i=0; i< h->capacity; i++){
        free(h->keys[i]);
    }
    free(h->keys);
    free(h->frequency);
    free(h);
}

static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;
}

void htable_print(htable h, FILE *stream){
    int i;
    for(i = 0; i < h->capacity; i++ ){
        fprintf(stream, "%d %d  %s\n", i, h->frequency[i] , h->keys[i] == NULL ? "" : h->keys[i]);
    }
}

int htable_insert(htable h, char *str){
    unsigned int index;
    int count;

    index = htable_word_to_int(str) % h->capacity;

    count = 0;
    while(count < h->capacity){
        if (h->keys[index] == NULL){
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0][0]);
            strcpy(h->keys[index], str);
            h->num_keys++;
            h->frequency[index] = 1;
            return 1;
        } else if (strcmp(h->keys[index], str) == 0){
            h->frequency[index] = h->frequency[index] + 1;
            return h->frequency[index];
        } else {
            index = index + 1;
            if((int)index == h->capacity){
                index = 0;
            }
            count++;
        }
    }
    return 0;
}

int htable_search(htable h, char *str){
    int collisions = 0;
    unsigned int index;

    index = htable_word_to_int(str) % h->capacity;

    while((h->keys[index] != NULL) && (strcmp(h->keys[index], str) != 0) && (collisions < h->capacity)){
        index = index +1;
        if((int) index == h->capacity){
            index = 0;
        }
        collisions++;
    }
    if(collisions == h->capacity){
        return 0;
    }else{
        return h->frequency[index];
    }
}

