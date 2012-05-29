#ifndef _template_extractor_h_
#define _template_extractor_h_ 1

#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "stdio.h"

//#include "opencv/cv.h"
//#include "opencv/cxcore.h" 
#define VIDEO_WIDTH 1024
#define VIDEO_HEIGHT 600
#define HSV_HUE_RED 0
#define HSV_HUE_GREEN 120
#define HSV_HUE_BLUE 240
#define HUE_EPS	4
#define SAT_THRES 100
#define LAB_A_THRES 140
#define LAB_B_THRES 100

#define for_each_pixel(pixel,img_ptr) \
	int i,j; \
	unsigned char *data=((uchar*)img_ptr->imageData); \
	int step=img_ptr->widthStep; \
	for(i=0;i<img_ptr->height;i++) { \
			for(j=0;j<(img_ptr->width) * (img_ptr->nChannels);j+=img_ptr->nChannels) { \
				pixel=cvGet2D(mask,i,j/filtered->nChannels); } \
			data+=step; 



extern IplImage* selectHue(const IplImage* elabMask,IplImage* bitMask,int hue,int eps,int sat);
extern IplImage* getHuePixelsMap(const IplImage* img,int hue,CvSize output_size,int eps,int sat);
extern void bn_get_containing_box_coordinates(const IplImage* gray_img,CvPoint* topLeft,CvPoint* bottomRight);
extern void show_scaled_image_and_stop(const IplImage*img,int width,int height);
extern IplImage* get_black_pixels_without_mask(const IplImage* img,const IplImage* mask);
extern void bn_reverse(IplImage* img);
extern IplImage* getDarkerPixelsMap(const IplImage* img,int threshold);
extern void std_show_image(const IplImage* img,char* name ,  int width , int height);
extern IplImage* img_crop(IplImage* img,CvRect rect);
extern void bn_closure(IplImage* img,int n);
extern IplImage* pattern_matching(const IplImage* img,const IplImage* pattern);
extern IplImage* getABPixelsMap(const IplImage* img,int a_thres,int b_thres);
extern void getRedAreaCoords(const IplImage* img,CvPoint *tl,CvPoint *br);
extern IplImage* getCentredROI(const IplImage *img,int width,int height,CvPoint *offset);
extern IplImage* cleanUpRedComponent(IplImage* img);
extern IplImage* remove_mask(const IplImage* img,const IplImage* mask);

#endif
