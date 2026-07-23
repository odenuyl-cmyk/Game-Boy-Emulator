#ifndef HELPERS_H
#define HELPERS_H

#endif

constexpr int rows = 50;
constexpr int cols = 50;
inline bool pixels[rows*cols];
inline bool copyPixels[rows*cols];

void initPixels(bool array[rows*cols]);
void add(int index);
void remove(int index);
int countNbors(int index);
void step();