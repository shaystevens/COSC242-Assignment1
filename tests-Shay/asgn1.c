#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

/**
 * Prints out a helpful message on what the program does, showing what arguments
 * you can enter in the command line and what they do.
 */
static void help_message(){
    printf("Usage: ./asgn1 [OPTION]... <STDIN>\n\n");

    printf("Perform various operations using a hash table."
           "  By default, words are\n" 
           "read from stdin and added to the hash table,"
           " before being printed out\n"
           "alongside their frequencies to stdout.\n\n");

    printf(" -d           Use double hashing (linear probing is the default)\n"
           " -e           Display entire contents of hash table on stderr\n"
           " -p           Print stats info instead of frequencies & words\n"
           " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n"
           " -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n"
           "\n"
           " -h           Display this message\n\n");

    exit(EXIT_SUCCESS);
           
}

/**
 * Function that determines if the parameter entered is a prime number.
 *
 * @param i - the integer that will be tested.
 *
 * @return int 1 (true) or 0 (false).
 */
static int isPrime(int i){
    int x;
    if(i <= 1){
        return 0;
    }
    
    for(x=2; x <= i/2; x++){
        if(i % x == 0){
            return 0;
        }
    }
    return 1;
}

/**
 * Function that finds and returns the next prime number if the parameter
 * is not prime if the parameter is prime it returns the parameter.
 *
 * @param capacity - htable capacity size
 *
 * @return int either entered capacity if it is prime or next prime.
 */
static int prime_greater(int capacity){
    while (isPrime(capacity) == 0){
        capacity++;
    }
    return capacity;
}

/**
 * Main part of program which reads inputs from the command line and changes the
 * program output the default is table size 113 and linear probing as collosion
 * resolution.
 *
 * @param argc number or arguments.
 * @param argv the arguments.
 *
 * @return int EXIT_SUCESS.
 */
int main(int argc, char **argv) {
    int capacity = 113; /* Default capacity */
    htable h;
    char word[256];
    const int default_capacity = 113;
    const char *optstring = "deps:t:h"; /* Allowed command line entries */
    char option;
    int options[5] = {0, 0, 0, 10, 0};
    /* {0 = linear 1 = double, e = 1 option run, p = 1 option run,
       s = how many snapshots, 1 = tablesize set*/
    
    if(argc > 0){
        while ((option = getopt(argc, argv, optstring)) != EOF) {
            switch (option) {
                case 'd':
                    /*set options[0] to 1 (true) */
                    options[0] = 1;
                    break;
                
                case 'e':
                    /* set options[1] to 1 (true) */
                    options[1] = 1;
                    break;
                
                case 'p':
                    /* set options[2] to 1 (true) */
                    options[2] = 1;
                    break;

                case 's':
                    /* set options[3] to entered digit */
                    options[3] = atoi(optarg);
                    break;

                case 't':
                    /* scan for integer and update capacity and options[4]*/
                    sscanf(optarg, "%d", &capacity);
                    capacity = prime_greater(capacity);
                    options[4] = 1;
                    break;
       
                case 'h':
                    /* Display help message */
                    help_message();
                    return EXIT_SUCCESS;
                
                default:
                    /* if an unknown option is given */
                    help_message();
                    
            }
        }
    }

    if (options[0] == 0 && options[4] == 0){
        /* use deafult capacity and linear probing */
        h = htable_new(default_capacity, LINEAR_P);
        
    } else if (options[0] == 1 && options[4] == 0){
        /* use default capacity and double hashing */
        h = htable_new(default_capacity, DOUBLE_H);
        
    } else if (options[0] == 0 && options[4] == 1){
        /* use new capacity and linear probing */
        h = htable_new(capacity, LINEAR_P);
        
    } else {
        /* use new capacity and double hashing */
        h = htable_new(capacity, DOUBLE_H);
    }

    /* insert words into hash table */
    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }

    if (options[1] == 1){
        /* print entire htable */
        htable_print_entire_table(h, stderr);
    }

    if(options[3] <= 0){
        /* if an invalid snapshot number is given set it back to default */
        options[3] = 10;
    }

    if (options[2] == 1){
        /* print htable stats */
        htable_print_stats(h, stdout, options[3]);

    } else {
        htable_print(h);
    }

    htable_free(h);  
    
    return EXIT_SUCCESS;
}


