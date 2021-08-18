#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"

int main(void){
    htable h = htable_new(12);
    htable_free(h);
    printf("Htable Success");

    return EXIT_SUCCESS;
}
