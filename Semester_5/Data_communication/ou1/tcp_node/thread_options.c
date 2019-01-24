#include <stdio.h>
#include <stdlib.h>
#include "thread_options.h"

void terror(char *err) {
    perror(err);
    exit(1);
}