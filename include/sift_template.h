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
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix. It can return the number of match between templae and image*/

extern void perspectiveTrasformation(const CvMat* H,CvPoint* src);
/*Map the src point wrt the transformation matrix H*/

extern CvMat* getProjection(IplImage* template,IplImage *image);
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/

extern CvMat* getProjectionAndMatchFeatures(struct feature* feat_template,int n1,char* siftName,int* numberOfMatch);
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/

extern CvMat* getProjectionAndMatchText(char* templateSiftName,char* siftName,int* numberOfMatch);
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/
#endif
