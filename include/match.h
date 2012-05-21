#ifndef _match_h_
#define _match_h_ 1
#include "jhead.h"

extern int getTextCircledPosition( char* pdf_image_name,char* photo_name,int* tlx,int* tly,int* width,int* height);
extern void getImageDate(char* name,char* time);
extern void parseText(char* text);

#endif
