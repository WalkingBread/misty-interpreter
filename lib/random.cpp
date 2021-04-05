#include <time.h>
#include <stdlib.h>

int randint(int min, int max) {
    srand(time(NULL));
    int random = rand() % (max - min) + min;

    return random;
}