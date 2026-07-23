#include <iostream>
#include "GOL_helpers.h"
using namespace std;

void initPixels(bool array[rows*cols]) {
    for (int i = 0; i < rows*cols; i++) {
        array[i] = false;
    }
}

void add(int index) {
    pixels[index] = true;
}

void remove(int index) {
    pixels[index] = false;
}

int countNbors(int index) {
    int result = 0;
    int startIndex = index - cols - 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (index % cols == 0 && j == 0) continue;
            if (index % cols == cols - 1 && j == 2) continue;
            int currIndex = startIndex + i*cols + j;
            if (currIndex < 0 || currIndex >= rows*cols || currIndex == index) continue;
            if (pixels[currIndex]) result++;
        }
    }
    return result;
}

void step() {
    for (int i = 0; i < rows*cols; i++) {
        int nbors = countNbors(i);

        if (pixels[i]) {
            copyPixels[i] = (nbors == 2 || nbors == 3);
        } else {
            copyPixels[i] = (nbors == 3);
        }
    }

    for (int i = 0; i < rows*cols; i++) {
        pixels[i] = copyPixels[i];
    }
}
