#ifndef VECTOR
#define VECTOR

typedef struct Vector {
    double x;
    double y;
} Vector;

void initVector(Vector *v, double x, double y);
#endif
