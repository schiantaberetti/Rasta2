
#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include "template_extractor.h"

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include "sift_template.h"

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49


int getSiftMatches(char* templateSiftName,char* siftName){
/*It calculate the affine trasfomartion of template over image wrt sift matching .*/
  //IplImage * image;
  struct feature* feat_image, *feat_template,* feat;
  struct feature** nbrs;
  struct kd_node* kd_root;
  CvPoint pt1, pt2;
  double d0, d1;
  int n2,n1, k, i, m = 0;

  //image=cvLoadImage(imageName,1);
  //printf("\nCarico features dell'immagine");
  //n2 = sift_features( image, &feat_image );// extract feature from image img2
  n2=import_features( siftName,FEATURE_LOWE, &feat_image );
  n1=import_features( templateSiftName,FEATURE_LOWE, &feat_template );
  kd_root = kdtree_build( feat_image, n2 ); //create a kdtree (i.e. a multiscale analysis of image img2)

  for( i = 0; i < n1; i++ )
    {
      feat = feat_template + i;
      k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS ); // finds an image feat'approximate k nearest neighbors in a kd tree using best bin first search.
      if( k == 2 )
	{
	  d0 = descr_dist_sq( feat, nbrs[0] );//calculate distance between feature and a nearest sift calculate previously
	  d1 = descr_dist_sq( feat, nbrs[1] );// calculate the other
	  if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
	    {
	      m++;
	      feat_template[i].fwd_match = nbrs[0];
	    }
	}
      free( nbrs );
    }

  kdtree_release( kd_root );
  free( feat_image );
  free(feat_template);
  return m;

}

//BRUTTURA
CvMat* getProjectionAndMatchText(char* templateSiftName,char* siftName,int* numberOfMatch){
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/
  //IplImage * image;
  struct feature* feat_image, *feat_template,* feat;
  struct feature** nbrs;
  struct kd_node* kd_root;
  CvPoint pt1, pt2;
  double d0, d1;
  int n2,n1, k, i, m = 0;

  //image=cvLoadImage(imageName,1);
  //printf("\nCarico features dell'immagine");
  //n2 = sift_features( image, &feat_image );// extract feature from image img2
  n2=import_features( siftName,FEATURE_LOWE, &feat_image );
  n1=import_features( templateSiftName,FEATURE_LOWE, &feat_template );
  kd_root = kdtree_build( feat_image, n2 ); //create a kdtree (i.e. a multiscale analysis of image img2)

  for( i = 0; i < n1; i++ )
    {
      feat = feat_template + i;
      k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS ); // finds an image feat'approximate k nearest neighbors in a kd tree using best bin first search.
      if( k == 2 )
	{
	  d0 = descr_dist_sq( feat, nbrs[0] );//calculate distance between feature and a nearest sift calculate previously
	  d1 = descr_dist_sq( feat, nbrs[1] );// calculate the other
	  if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
	    {
	      m++;
	      feat_template[i].fwd_match = nbrs[0];
	    }
	}
      free( nbrs );
    }

	*numberOfMatch=m;


    CvMat* H;
    H = ransac_xform( feat_template, n1, FEATURE_FWD_MATCH, lsq_homog, 4, 0.01,
		      homog_xfer_err, 3.0, NULL, NULL ); //calculate the geometric transformate with respect to sift

  kdtree_release( kd_root );
  free( feat_image );
  free(feat_template);
  //cvReleaseImage(&image);
  return H;

}








CvMat* getProjectionAndMatchFeatures(struct feature* feat_template,int n1,char* siftName,int* numberOfMatch){
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/
  //IplImage * image;
  struct feature* feat_image, * feat;
  struct feature** nbrs;
  struct kd_node* kd_root;
  CvPoint pt1, pt2;
  double d0, d1;
  int n2, k, i, m = 0;

  //image=cvLoadImage(imageName,1);
  printf("\nCarico features dell'immagine");
  //n2 = sift_features( image, &feat_image );// extract feature from image img2
  n2=import_features( siftName,FEATURE_LOWE, &feat_image );
  kd_root = kdtree_build( feat_image, n2 ); //create a kdtree (i.e. a multiscale analysis of image img2)

  for( i = 0; i < n1; i++ )
    {
      feat = feat_template + i;
      k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS ); // finds an image feat'approximate k nearest neighbors in a kd tree using best bin first search.
      if( k == 2 )
	{
	  d0 = descr_dist_sq( feat, nbrs[0] );//calculate distance between feature and a nearest sift calculate previously
	  d1 = descr_dist_sq( feat, nbrs[1] );// calculate the other
	  if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
	    {
	      m++;
	      feat_template[i].fwd_match = nbrs[0];
	    }
	}
      free( nbrs );
    }

	*numberOfMatch=m;


    CvMat* H;
    H = ransac_xform( feat_template, n1, FEATURE_FWD_MATCH, lsq_homog, 4, 0.01,
		      homog_xfer_err, 3.0, NULL, NULL ); //calculate the geometric transformate with respect to sift

  kdtree_release( kd_root );
  free( feat_image );
  //free(feat_template);
  //cvReleaseImage(&image);
  return H;

}


CvMat* getProjectionAndMatch(IplImage* template,char* imageName,int* numberOfMatch){
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/
  IplImage * image;
  struct feature* feat_image, * feat_template, * feat;
  struct feature** nbrs;
  struct kd_node* kd_root;
  CvPoint pt1, pt2;
  double d0, d1;
  int n1, n2, k, i, m = 0;

  image=cvLoadImage(imageName,1);
  printf("Cerco features nel template\n");
  n1 = sift_features( template, &feat_template );// extract feature from template img1
  printf("Cerco features nell'immagine\n");
  n2 = sift_features( image, &feat_image );// extract feature from image img2
  kd_root = kdtree_build( feat_image, n2 ); //create a kdtree (i.e. a multiscale analysis of image img2)
  for( i = 0; i < n1; i++ )
    {
      feat = feat_template + i;
      k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS ); // finds an image feat'approximate k nearest neighbors in a kd tree using best bin first search.
      if( k == 2 )
	{
	  d0 = descr_dist_sq( feat, nbrs[0] );//calculate distance between feature and a nearest sift calculate previously
	  d1 = descr_dist_sq( feat, nbrs[1] );// calculate the other
	  if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
	    {
	      m++;
	      feat_template[i].fwd_match = nbrs[0];
	    }
	}
      free( nbrs );
    }

	*numberOfMatch=m;


    CvMat* H;
    H = ransac_xform( feat_template, n1, FEATURE_FWD_MATCH, lsq_homog, 4, 0.01,
		      homog_xfer_err, 3.0, NULL, NULL ); //calculate the geometric transformate with respect to sift
// DEBUG
/*	if( H )
      {
	IplImage* xformed = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 3 );
	cvWarpPerspective( template, xformed, H, //apply the geometric transformate with respect to sift
			   CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS,
			   cvScalarAll( 0 ) );
	
	cvNamedWindow( "Xformed", 1 );
	cvShowImage( "Xformed", xformed );
	cvWaitKey( 0 );
	
      }	*/
// END DEBUG
  kdtree_release( kd_root );
  free( feat_template );
  free( feat_image );
  cvReleaseImage(&image);
  return H;

}

void perspectiveTrasformation(const CvMat* H,CvPoint* src)
/*Map the src point wrt the transformation matrix H*/
{
	int x,y;
	double den=(double)(cvGetReal2D(H,2,0)*(src->x)+cvGetReal2D(H,2,1)*(src->y)+cvGetReal2D(H,2,2));
	x=(int)((cvGetReal2D(H,0,0)*(src->x)+cvGetReal2D(H,0,1)*(src->y)+cvGetReal2D(H,0,2))/den);
	y=(int)((cvGetReal2D(H,1,0)*(src->x)+cvGetReal2D(H,1,1)*(src->y)+cvGetReal2D(H,1,2))/den);
	src->x=x;
	src->y=y;
}
CvMat* getProjection(IplImage* template,IplImage *image){
/*It calculate the affine trasfomartion of template over image wrt sift matching and returns the transformation matrix.*/
 // IplImage * stacked;
  struct feature* feat_image, * feat_template, * feat;
  struct feature** nbrs;
  struct kd_node* kd_root;
  CvPoint pt1, pt2;
  double d0, d1;
  int n1, n2, k, i, m = 0;


  printf("Cerco features nel template\n");
  n1 = sift_features( template, &feat_template );// extract feature from template img1
  printf("Cerco features nell'immagine\n");
  n2 = sift_features( image, &feat_image );// extract feature from image img2
  kd_root = kdtree_build( feat_image, n2 ); //create a kdtree (i.e. a multiscale analysis of image img2)
  for( i = 0; i < n1; i++ )
    {
      feat = feat_template + i;
      k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS ); // finds an image feat'approximate k nearest neighbors in a kd tree using best bin first search.
      if( k == 2 )
	{
	  d0 = descr_dist_sq( feat, nbrs[0] );//calculate distance between feature and a nearest sift calculate previously
	  d1 = descr_dist_sq( feat, nbrs[1] );// calculate the other
	  if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
	    {

	      m++;
	      feat_template[i].fwd_match = nbrs[0];
	    }
	}
      free( nbrs );
    }

  fprintf( stderr, "Found %d total matches\n", m );


    CvMat* H;
    H = ransac_xform( feat_template, n1, FEATURE_FWD_MATCH, lsq_homog, 4, 0.01,
		      homog_xfer_err, 3.0, NULL, NULL ); //calculate the geometric transformate with respect to sift
// DEBUG
/*	if( H )
      {
	IplImage* xformed = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 3 );
	cvWarpPerspective( template, xformed, H, //apply the geometric transformate with respect to sift
			   CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS,
			   cvScalarAll( 0 ) );
	
	cvNamedWindow( "Xformed", 1 );
	cvShowImage( "Xformed", xformed );
	cvWaitKey( 0 );
	
      }	*/
// END DEBUG
  kdtree_release( kd_root );
  free( feat_template );
  free( feat_image );
  return H;

}


