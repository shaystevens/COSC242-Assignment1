#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"

int main(int argc, char **argv) {
    int capacity;
    htable h;
    char word[256];
    const int default_capacity = 113;

    if (argc > 1){
        sscanf(argv[1], "%d", &capacity);
        h = htable_new(capacity);      
    } else {
        h = htable_new(default_capacity);
    }

    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }

    htable_print(h, stdout);
    htable_free(h);  
    
    return EXIT_SUCCESS;
}
