#ifndef LOREG_H
#define LOREG_H

void zeromatrix( float** mx, long rs, long re, long cs, long ce);

void loregf(float *x, long ci, long h, long d, float *a);

void loreg(float *x, long l, long h, long d, float *xs);

#endif