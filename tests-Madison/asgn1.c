#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

static void help_message(){
    printf("Usage: ./asgn [OPTION]... <STDIN>\n\n");

    printf("Perform various operations using a hash table.  By default, words are\n" 
           "read from stdin and added to the hash table, before being printed out\n"
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
    
static int isPrime(int i){
    int x;
    for(x=2; x <= i/2; x++){
        if(i % x == 0){
            return 0;
        }
    }
    return 1;
}

static int prime_greater(int capacity){
    while (isPrime(capacity) == 0){
        capacity++;
        
        /* for (i = 2; i < capacity/2; i++){
           if (capacity%i == 0){
           break;
           }
           }
           capacity++;*/
    }
    return capacity;
}

int main(int argc, char **argv) {
    int capacity = 113;
    htable h;
    char word[256];
    const int default_capacity = 113;
    const char *optstring = "deps:t:h";
    char option;
    /* int is_Double_H = 0; 1 for yes, 0 for no*/
    int options[5] = {0, 0, 0, 10, 0};
    /* {0 = linear 1 = double, e = 1 option run, p = 1 option run, s = how many snapshots, 1 = tablesize set*/
    
    if(argc > 0){
        while ((option = getopt(argc, argv, optstring)) != EOF) {
            switch (option) {
                case 'd':
                    /* do something */
                    options[0] = 1;
                    break;
                
                    /* int i = 0;
                       if (argc == 2){
                       h = htable_new(default_capacity, DOUBLE_H);
                       } else {
                       for (i = 1; i < argc; i++){
                       if (getopt(arc, argv, optstring) == )
                       }
                       }*/
                
                case 'e':
                    /* the argument after the -b is available
                       in the global variable ’optarg’ */
                    options[1] = 1;
                    break;
                
                case 'p':
                    /* do something else */
                    options[2] = 1;
                    break;

                case 's':
                    options[3] = atoi(optarg);
                    break;

                case 't':
                    sscanf(optarg, "%d", &capacity);
                    capacity = prime_greater(capacity);
                    options[4] = 1;
                    break;
                    /* h = htable_new(prime_greater(capacity), DOUBLE_H); */
                case 'h':
               
                    help_message();
                    /*"Perform various operations using a hash table.  By default, \
                      words are read from stdin and added to the hash table, before being printed out\
                      alongside their frequencies to stdout.

                      -d           Use double hashing (linear probing is the default)\
                      -e           Display entire contents of hash table on stderr\
                      -p           Print stats info instead of frequencies & words\
                      -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\
                      -t TABLESIZE Use the first prime >= TABLESIZE as htable size\

                      -h           Display this message");*/
                    return EXIT_SUCCESS;
                
                default:
                    /* if an unknown option is given */
                    help_message(); /*"Perform various operations using a hash table.  By default, words are\n read from stdin and added to the hash table, before being printed out\n
                                                                   alongside their frequencies to stdout.\n\n

                                                                   -d           Use double hashing (linear probing is the default)\n
                                                                   -e           Display entire contents of hash table on stderr\n
                                                                   -p           Print stats info instead of frequencies & words\n
                                                                   -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n
                                                                   -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n

                                                                   -h           Display this message\n");*/
                    
            }
        }
    }

    if (options[0] == 0 && options[4] == 0){
        h = htable_new(default_capacity, LINEAR_P);
    } else if (options[0] == 1 && options[4] == 0){
        h = htable_new(default_capacity, DOUBLE_H);
    } else if (options[0] == 0 && options[4] == 1){
        h = htable_new(capacity, LINEAR_P);
    } else {
        h = htable_new(capacity, DOUBLE_H);
    }

    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }

    if (options[1] == 1){
        htable_print_entire_table(h, stderr);
    }

    if(options[3] <= 0){
        options[3] = 10;
    }

    if (options[2] == 1){
        htable_print_stats(h, stdout, options[3]);

        /*must link p and s, divide 100 percent into increments */
    } else {
        htable_print(h, stdout);
    }

    /*  if (argc > 1){
        sscanf(argv[1], "%d", &capacity);
        h = htable_new(capacity, DOUBLE_H);      
        } else {
        h = htable_new(default_capacity, DOUBLE_H);
        }

        while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
        }

        htable_print(h, stdout);

    */
    htable_free(h);  
    
    return EXIT_SUCCESS;
}


