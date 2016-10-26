#ifndef COLORMAP_H
#define COLORMAP_H

//color
#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define blue RGB(0,0,255)
#define yellow RGB(255,255,0)
#define cyan RGB(0,255,255)
#define magenta RGB(255,0,255)
#define white RGB(255,255,255)

void hsv2rgb(float hsv[3], float rgb[3]);
void genColor(float rgbcolor[4], float colorv);
COLORREF genColor(float colorv);
float genColorvFromIndex(unsigned long idx);

#endif