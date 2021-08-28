#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

/**
 * Prints out a helpful message on what the program does, showing what
 * arguments you can enter in the command line and what they do.
 *
 * @param x - int, to determine if help message called due to -h option given
 * or because invalid option given.
 */
static void help_message(int x){
    /* print to stdout if given the -h option, print to stderr if invalid
       option given */
    if (x == 0){
        fprintf(stdout, "Usage: ./asgn1 [OPTION]... <STDIN>\n\n");

        fprintf(stdout, "Perform various operations using a hash table."
                "  By default, words are\n" 
                "read from stdin and added to the hash table,"
                " before being printed out\n"
                "alongside their frequencies to stdout.\n\n");

        fprintf(stdout, " -d           Use double hashing (linear probing is"
                " the default)\n"
                " -e           Display entire contents of hash table on stderr"
                "\n"
                " -p           Print stats info instead of frequencies & words"
                "\n"
                " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)"
                "\n"
                " -t TABLESIZE Use the first prime >= TABLESIZE as htable size"
                "\n\n"
                " -h           Display this message\n\n");
    } else {
        fprintf(stderr, "Usage: ./asgn1 [OPTION]... <STDIN>\n\n");

        fprintf(stderr, "Perform various operations using a hash table."
                "  By default, words are\n" 
                "read from stdin and added to the hash table,"
                " before being printed out\n"
                "alongside their frequencies to stdout.\n\n");

        fprintf(stderr, " -d           Use double hashing (linear probing is"
                " the default)\n"
                " -e           Display entire contents of hash table on stderr"
                "\n"
                " -p           Print stats info instead of frequencies & words"
                "\n"
                " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)"
                "\n"
                " -t TABLESIZE Use the first prime >= TABLESIZE as htable size"
                "\n\n"
                " -h           Display this message\n\n");
    }
}

/**
 * Function that determines if the parameter entered is a prime number.
 *
 * @param i - int, the integer that will be tested.
 *
 * @return int, 1 (true) or 0 (false).
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
 * is not prime, if the parameter is prime it returns the parameter.
 *
 * @param capacity - int, htable capacity size.
 *
 * @return capacity - int, the entered capacity if it is prime, or next prime.
 */
static int prime_greater(int capacity){
    while (isPrime(capacity) == 0){
        capacity++;
    }

    return capacity;
}

/**
 * Function that prints out words and their frequencies to stdout.
 *
 * @param freq - int, the amount of times the word is in the table.
 * @param word - char *, the word in the table.
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/**
 * Main part of the program which reads inputs from stdin and changes the
 * program output. The default is table size 113 and linear probing as
 * collision resolution.
 *
 * @param argc - int, number of arguments.
 * @param argv - char **, the arguments.
 *
 * @return EXIT_SUCCESS - int, program was a success.
 */
int main(int argc, char **argv) {
    int capacity = 113; /* default capacity 113 */
    htable h;
    char word[256];
    const char *optstring = "deps:t:h"; /* Allowed command line entries */
    char option;
    /* options[0] = collision resolution, 0 for linear probing, 1 for double
       hashing
       options[1] = e option, 1 if run, 0 if not
       options[2] = p option, 1 if run, 0 if not
       options[3] = s option, number of stats snapshots, 10 if not set/default
       options[4] = t option, equal to size of table, 0 if not set/default*/
    int options[5] = {0, 0, 0, 10, 0};
       
    if(argc > 0){
        while ((option = getopt(argc, argv, optstring)) != EOF) {
            switch (option) {
                case 'd':
                    /* set for double hashing collision resolution */
                    options[0] = 1;
                    break;
                
                case 'e':
                    options[1] = 1;
                    break;
                
                case 'p':
                    options[2] = 1;
                    break;

                case 's':
                    /* read int argument after s option, put into options[3] */
                    sscanf(optarg, "%d", &options[3]);
                    break;

                case 't':
                    /* read int argument after t option and put into options[4],
                       update capacity to obtain the prime equal or greater to
                       this inputted table size rather than default capacity */
                    sscanf(optarg, "%d", &options[4]);
                    capacity = prime_greater(options[4]);
                    break;
       
                case 'h':
                    /* display help message */
                    help_message(0);
                    return EXIT_SUCCESS;
                
                default:
                    /* if an invalid option is given */
                    help_message(1);
                    return EXIT_SUCCESS;
                    
            }
        }
    }

    if (options[0] == 0){
        /* use linear probing */
        h = htable_new(capacity, LINEAR_P); 
    } else {
        /* use double hashing */
        h = htable_new(capacity, DOUBLE_H);
    }

    /* insert words into hash table */
    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }

    if (options[1] == 1){
        /* print entire htable to stderr */
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
        htable_print(h, print_info);
    }

    htable_free(h);  
    
    return EXIT_SUCCESS;
}


