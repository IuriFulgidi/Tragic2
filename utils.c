#include "gamelib.h"
#include <stdio.h>

void clear_screen() {
    printf("\033[H\033[J");
    fflush(stdout);
}
