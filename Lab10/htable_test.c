#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"

int main(int argc, char **argv) {
    int capacity;
    htable h;
    char word[256];

    capacity = atoi(argv[1]);
    h = htable_new(capacity);
    
    while (getword(word, sizeof word, stdin) != EOF) {
      htable_insert(h, word);
    }

    htable_print(h, stdout);
    htable_free(h);
    
    return EXIT_SUCCESS;
}
