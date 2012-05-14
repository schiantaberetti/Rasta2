#ifndef _match_h_
#define _match_h_ 1


#ifdef __cplusplus // if compiled with C++ compiler. Note that this is not
// standard, check your compiler's docs and other header files to find out
// the correct macro it uses for c++ versus C compilations.
extern "C" {
#endif 

extern int getTextCircledPosition( char* pdf_image_name,char* photo_name,int* tlx,int* tly,int* width,int* height);

#ifdef __cplusplus // if compiled with C++ compiler
} // end of extern "C" block
#endif

#endif
