#ifndef _template_extractor_h_
#define _template_extractor_h_ 1

#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "stdio.h"

/*NOTICE*/
/*the "bn" prefix referred to functions or variables is used to indicate operation on gray images. */

#define VIDEO_WIDTH 1024
#define VIDEO_HEIGHT 600
#define HSV_HUE_RED 0
#define HSV_HUE_GREEN 120
#define HSV_HUE_BLUE 240
#define HUE_EPS	4
#define SAT_THRES 100
#define LAB_A_THRES 140
#define LAB_B_THRES 100


extern IplImage* selectHue(const IplImage* elabMask,IplImage* bitMask,int hue,int eps,int sat);
/*elabMask is a HSV image and bitMask is the computed gray image of pixel with hue=hue+-eps and with sat as threshold of saturation*/
 
extern IplImage* getHuePixelsMap(const IplImage* img,int hue,CvSize output_size,int eps,int sat);
/*Given an arbitrary image (img) this function returns a mask of pixel of hue=hue *\
 (according to the parameters of eps and sat) of dimension "output_size".             
\* Typical values for eps and sat are (4,100).                                    */

extern void bn_get_containing_box_coordinates(const IplImage* gray_img,CvPoint* topLeft,CvPoint* bottomRight);
/*Given a gray image (img) overwrite the topleft and the bottom right corner of the white pixels. *\
\* I.e. get the describing points of the minimum rectangle containing all the white pixels.       */

extern void show_scaled_image_and_stop(const IplImage*img,int width,int height);
/*Simply show an image and stop until key pressing. Useful for debugging.*/

extern IplImage* get_black_pixels_without_mask(const IplImage* img,const IplImage* mask);
/*Return a gray image computed deleting the mask component     */

extern void bn_reverse(IplImage* img);

extern IplImage* getDarkerPixelsMap(const IplImage* img,int threshold);

extern void std_show_image(const IplImage* img,char* name ,  int width , int height);

extern IplImage* img_crop(IplImage* img,CvRect rect);
/*Return the cropped image i.e. the portion of image inside rect*/

extern void bn_closure(IplImage* img,int n);

extern IplImage* pattern_matching(const IplImage* img,const IplImage* pattern);
/*Return the probability map of the pattern in the img*/

extern IplImage* getABPixelsMap(const IplImage* img,int a_thres,int b_thres);
/*Given an arbitrary image (img) this function returns a mask of pixel for the a,b coordinates
 * in the Lab color space          
\* Typical values for a and b are (140,100).       */

extern void getRedAreaCoords(const IplImage* img,CvPoint *tl,CvPoint *br);

extern IplImage* getCentredROI(const IplImage *img,int width,int height,CvPoint *offset);
/*Crop the center of the image of dimension width*height. */

extern IplImage* cleanUpRedComponent(IplImage* img);

extern IplImage* remove_mask(const IplImage* img,const IplImage* mask);
/*Return the image computed deleting the mask component     */

#endif
