#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"

int main(int argc, char **argv) {
    /*int item;
    htable my_htable = htable_new(11);

    htable_free(my_htable);*/

    int capacity;
    htable h;
    char word[256];
    /*char words;*/

    if (argc > 1){
        /* capacity = (int)argv[1]; FIX!!!!! at number 2*/
        /*printf("%s", argv[1]);*/
        sscanf(argv[1], "%d", &capacity);
        /* printf("%d\n", capacity);*/
    

        h = htable_new(capacity);

        while (getword(word, sizeof word, stdin) != EOF) {
            htable_insert(h, word);
        }

        htable_print(h, stdout);
        htable_free(h);

        
    }

    return EXIT_SUCCESS;
}
