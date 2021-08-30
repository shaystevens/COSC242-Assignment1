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
    int *stats; /* holds number of collisions for each word from insertion */
    hashing_t method; /* either linear probing or double hashing */
};

/**
 * Function that creates a new htable and allocates necessary memory and
 * initialises everything to default.
 *
 * @param capacity - int, size of the hash table.
 * @param hashing_type - hashing_t, collision resolution strategy (linear
 * probing or double hashing).
 *
 * @return h - htable, the new hash table.
 */
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

/**
 * Function that turns a specified word into an int.
 *
 * @param word - char*, the word we are converting into an int.
 *
 * @return result - unsigned int, the converted word in int form.
 */
static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;

    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;
}

/**
 * Function that calculates the steps when using the double hashing collision
 * resolutiom strategy.
 *
 * @param h - htable, the hash table we are obtaining the size of.
 * @param i_key - unsigned int, used for step calculation.
 *
 * @return unsigned int, how big the step is.
 */
static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

/**
 * Insert a given string into a hash table using either linear probing
 * or double hashing as a collision resolution strategy.
 *
 * @param h - htable, a hash table which we are inserting into.
 * @param str - char *, string to be inserted into hash table.
 *
 * @return int, 1 if key didn't exist in hash table, key frequency if it did
 * exist in the hash table, or 0 if hash table is full and we cannot insert it.
 */
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

    /* key exists already at its first position */
    } else if (strcmp(h->keys[indexed_string], copied_str) == 0){
        h->freqs[indexed_string]++;
        return h->freqs[indexed_string];

    /* another key in first position */
    } else {
        int i = indexed_string;
        int collisions = 1;
        int dhash_step = htable_step(h, int_string);
        while (strcmp(h->keys[i], copied_str) != 0){
            /* updates i based on whether linear probing or double hashing
               is used */
            if (h->method == LINEAR_P){
                i++;
                i = i % h->capacity;
            } else {
                i = (indexed_string + (collisions * dhash_step))%h->capacity;
            }

            /* when it goes back to the same position it started at, meaning
             hash table is full */
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
        /* key exists but not in first position */
        h->freqs[i]++;

        return h->freqs[i];
    }
}

/**
 * Prints the contents of a hash table, frequency and words.
 *
 * @param h - htable, the hash table we are printing.
 */
void htable_print(htable h, void print_info(int freq, char *word)) {
    int i = 0;
    for (i = 0; i < h->capacity; i++){
        if ((h->keys[i]) != NULL){
            print_info(h->freqs[i], h->keys[i]);
        }
    }
}

/**
 * Frees the memory associated with the hash table.
 *
 * @param h - htable, the hash table which we are freeing.
 */
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

/**
 * Prints the entire hash table, position frequency, collision stats, and word.
 *
 * @param h - htable, the hash table. 
 * @param stream - FILE *, location where the table will be printed.
 */
void htable_print_entire_table(htable h, FILE *stream) {
    int i = 0;
    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream, "----------------------------------------\n");
    for (i = 0; i < h->capacity; i++){     
        fprintf(stream, "%5d %5d %5d   %s\n", i, h->freqs[i], h->stats[i],
                h->keys[i] != NULL ? h->keys[i] : "");
    }
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - htable, the hash table.
 * @param stream - FILE *, a stream to print the data to.
 * @param percent_full - int, the point at which to show the data from.
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
 * @param h - htable, the hashtable to print statistics summary from.
 * @param stream - FILE *, the stream to send output to.
 * @param num_stats - int, the maximum number of statistical snapshots to print.
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
