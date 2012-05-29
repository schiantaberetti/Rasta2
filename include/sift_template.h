#ifndef _sift_template_h_
#define _sift_template_h_ 1
#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include "template_extractor.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <stdio.h>

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49

extern CvMat* getProjectionAndMatch(IplImage* template,char* imageName,int* numberOfMatch);
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/

extern char* getImageFile(char* dirName,char* fileName);
/*concatenate the name of the image and the fileName and returns it. If the file isn't in format .jpg
it returns NULL*/

extern CvMat* bestFitForTemplate(IplImage* template,char* dirName);
/*Calculate the best match for template, searching in the directory specified by dirName. It returns
the affine matrix H calculated wrt the best match.*/
/*TO BE IMPROVED (PASS THE FEAT VECTOR OF TEMPLATE IF IT WORKS?)*/

extern void perspectiveTrasformation(const CvMat* H,CvPoint* src);
/*Map the src point wrt the transformation matrix H*/

extern CvMat* getProjection(IplImage* template,IplImage *image);
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/

extern IplImage* getTemplateProjection(IplImage* template,IplImage *image);
/*It calculate the affine trasfomartion of template over image wrt sift matching.*/

extern void getTemplatePositionFromImage(IplImage* img1,IplImage *img2,CvPoint* topLeft,CvPoint* bottomRight);
/*calculate the upper left corner and the bottom right corner of the area in img2 that relates to img1.*/
#endif
