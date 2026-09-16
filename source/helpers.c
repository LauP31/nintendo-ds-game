#include "helpers.h"
#include <stdlib.h>

int randomRange(int min, int max) {
    return min + rand() % (max - min + 1);
}