#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"

struct htablerec {
    int capacity;
    int num_keys;
    int *freqs;
    char **keys;
    int *stats;
    hashing_t method;
};

htable htable_new(int capacity, hashing_t hashing_type) {
    int i;
    htable h = emalloc(sizeof *h);
    h->capacity = capacity;
    h->num_keys = 0;
    h->freqs = emalloc(h->capacity * sizeof h->freqs[0]);
    h->keys = emalloc(h->capacity * sizeof h->keys[0]);
    h->stats = emalloc(h->capacity *sizeof h->stats[0]);
    for (i = 0; i < h->capacity; i++){
        h->freqs[i] = 0;
        h->keys[i] = NULL;
        h->stats[i] = 0;
    }

    h->method = hashing_type;
    
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
        h->freqs[indexed_string] = 1;
        h->stats[h->num_keys] = 0;
        h->num_keys++;
        return 1;

        /* key exists already */
    } else if (strcmp(h->keys[indexed_string], copied_str) == 0){
        h->freqs[indexed_string]++;
        return h->freqs[indexed_string];

        /* another key in first position */
    } else {
        int i = indexed_string;
        int collisions = 1;
        int dhash_step = htable_step(h, int_string);
        while (strcmp(h->keys[i], copied_str) != 0){
            if (h->method == LINEAR_P){
                i++;
                i = i % h->capacity;
            } else {
                i = (indexed_string + (collisions * dhash_step))%h->capacity;
            }

            /* when it goes back to the same position it started at */
            if (i == indexed_string){
                return 0;
                /* there is an empty spot where it should be so key doesn't exist */
            } else if (h->keys[i] == NULL){
                h->keys[i] = copied_str;
                h->freqs[i] = 1;
                h->stats[h->num_keys] = collisions;
                h->num_keys++;
                return 1;
            }
            collisions++;
        }
        h->freqs[i]++;
        return h->freqs[i];
    }
}

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
} /* WHAT TO DO WITH THIS???? */

void htable_print(htable h, FILE *stream) {
    int i = 0;
    for (i = 0; i < h->capacity; i++){
        if ((h->keys[i]) != NULL){
            fprintf(stream, "%d\t%s\n", h->freqs[i],
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
    free(h->freqs);
    free(h->stats);
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
        if (h->method == LINEAR_P){
            i++;
            i = i % h->capacity;
        } else {
            i = (indexed_string + (collisions * dhash_step))%h->capacity;
        }
        collisions++;
    }
    
    if (collisions == h->capacity){
        return 0;
    } else {
        return h->freqs[i];
    }
}

void htable_print_entire_table(htable h, FILE *stream) {
    int i = 0;
    /* int index = 1;*/
    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream, "----------------------------------------\n");
    for (i = 0; i < h->capacity; i++){
        /*if ((h->keys[i]) != NULL){*/      
        fprintf(stream, "%5d %5d %5d   %s\n", i, h->freqs[i], h->stats[i],
                h->keys[i] != NULL ? h->keys[i] : "");
            /*}*/
    }

    
    /* int i = 0;
    for (i = 0; i < h->capacity; i++){
        if ((h->keys[i]) != NULL){
            fprintf(stream, "\%5d \%5d \%5d   \%s\n", h->freqs[i],
               h->keys[i]);
        }
        }*/
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %11.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n", 
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current    Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries    At Home   Collisions   Collisions\n");
    fprintf(stream, "------------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "------------------------------------------------------\n\n");
}




