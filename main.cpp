/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools  |  Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: kiev
 *
 * Created on July 4, 2017, 1:38 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

#include "filter_procedure.h"


void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr )
{
    CvMat * tmp;
    CvMat q1stub, q2stub;
    CvMat q3stub, q4stub;
    CvMat d1stub, d2stub;
    CvMat d3stub, d4stub;
    CvMat * q1, * q2, * q3, * q4;
    CvMat * d1, * d2, * d3, * d4;

    CvSize size = cvGetSize(src_arr);
    CvSize dst_size = cvGetSize(dst_arr);
    int cx, cy;

    if(dst_size.width != size.width ||
       dst_size.height != size.height){
        cvError( CV_StsUnmatchedSizes, "cvShiftDFT", "Source and Destination arrays must have equal sizes", __FILE__, __LINE__ );  
    }

    if(src_arr==dst_arr){
        tmp = cvCreateMat(size.height/2, size.width/2,
cvGetElemType(src_arr));
    }
   
    cx = size.width/2;
    cy = size.height/2; // image center

    q1 = cvGetSubRect( src_arr, &q1stub, cvRect(0,0,cx, cy) );
    q2 = cvGetSubRect( src_arr, &q2stub, cvRect(cx,0,cx,cy) );
    q3 = cvGetSubRect( src_arr, &q3stub, cvRect(cx,cy,cx,cy) );
    q4 = cvGetSubRect( src_arr, &q4stub, cvRect(0,cy,cx,cy) );
    d1 = cvGetSubRect( src_arr, &d1stub, cvRect(0,0,cx,cy) );
    d2 = cvGetSubRect( src_arr, &d2stub, cvRect(cx,0,cx,cy) );
    d3 = cvGetSubRect( src_arr, &d3stub, cvRect(cx,cy,cx,cy) );
    d4 = cvGetSubRect( src_arr, &d4stub, cvRect(0,cy,cx,cy) );

    if(src_arr!=dst_arr){
        if( !CV_ARE_TYPES_EQ( q1, d1 )){
            cvError( CV_StsUnmatchedFormats, "cvShiftDFT", "Source and Destination arrays must have the same format", __FILE__, __LINE__ );
        }
        cvCopy(q3, d1, 0);
        cvCopy(q4, d2, 0);
        cvCopy(q1, d3, 0);
        cvCopy(q2, d4, 0);
    }
    else{
        cvCopy(q3, tmp, 0);
        cvCopy(q1, q3, 0);
        cvCopy(tmp, q1, 0);
        cvCopy(q4, tmp, 0);
        cvCopy(q2, q4, 0);
        cvCopy(tmp, q2, 0);
    }
}


void mat_print(CvMat * mat) 
{
	int i, j;
	
	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			printf("%6.3f ", ((float*) mat->data.ptr)[i * mat->cols + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}

void mat_print_char(CvMat * mat) 
{
	int i, j;
	
	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			printf("%d ", ((unsigned char*) mat->data.ptr)[i * mat->cols + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}
void ipl_print(IplImage * ipl) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			printf("%6.3f ", ((float*) ipl->imageData)[i * ipl->width + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}

void ipl_print_char(IplImage * ipl) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			printf("%d ", ((unsigned char*) ipl->imageData)[i * ipl->width + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}

void ipl_fill(IplImage * ipl, float v) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			((float * ) ipl->imageData)[i * ipl->width + j] = v;
		}
	}
}


void ipl_copy_float(IplImage * src, IplImage * dst) 
{
	int i, j;
	
	for (i = 0; i < src->height; i++) {
		for (j = 0; j < src->width; j++) {
			((float * ) dst->imageData)[i * dst->width + j] = ((unsigned char * ) src->imageData)[i * src->width + j];
		}
	}
}

void ipl_copy_char(IplImage * src, IplImage * dst) 
{
	int i, j;
	
	for (i = 0; i < src->height; i++) {
		for (j = 0; j < src->width; j++) {
			((unsigned char * ) dst->imageData)[i * dst->width + j] = ((float * ) src->imageData)[i * src->width + j];
		}
	}
}

void ipl_divide_scalar(IplImage * ipl, float div) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			((float * ) ipl->imageData)[i * ipl->width + j] = ((float * ) ipl->imageData)[i * ipl->width + j] / div;
		}
	}
}


float scale_number(float t, float minOld, float maxOld, float minNew, float maxNew) {
	float r = 0;
	r = minNew + ((maxNew - minNew) / (maxOld - minOld)) * (t - minOld);
	return r;
}

void scale (IplImage * ipl, /*float minOld, float maxOld,*/ float minNew, float maxNew) {
	
	int i, j;
	float minOld, maxOld;
	
	minOld = maxOld = ((float * ) ipl->imageData)[0];
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			if (maxOld < ((float * ) ipl->imageData)[i * ipl->width + j]) maxOld = ((float * ) ipl->imageData)[i * ipl->width + j];
			if (minOld > ((float * ) ipl->imageData)[i * ipl->width + j]) minOld = ((float * ) ipl->imageData)[i * ipl->width + j];
		}
	}
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			((float * ) ipl->imageData)[i * ipl->width + j] = scale_number(
					  ((float * ) ipl->imageData)[i * ipl->width + j], minOld, maxOld, minNew, maxNew);
		}
	}
}


void normalize_merge (CvArr * arr, /*float minOld, float maxOld,*/ float minNew, float maxNew) {
	
	int i, j;
	
	for (i = 0; i < ipl->height; i+=4) {
		for (j = 0; j < ipl->width; j+=4) {
			for (k = )
		}
	}
}

void show_image(CvArr * ipl_in_complex) {
	
	IplImage * ipl_dft_real, *  ipl_dft_imaginary, * ipl_image;
	double min = 0, max = 0;
	
	ipl_dft_real      = cvCreateImage(cvGetSize(ipl_in_complex), IPL_DEPTH_64F, 1);
	ipl_dft_imaginary = cvCreateImage(cvGetSize(ipl_in_complex), IPL_DEPTH_64F, 1);
	ipl_image         = cvCreateImage(cvGetSize(ipl_in_complex), IPL_DEPTH_8U,  1);

	
	// Split Fourier in real and imaginary parts
	cvSplit( ipl_in_complex, ipl_dft_real, ipl_dft_imaginary, NULL, NULL);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvPow( ipl_dft_real, ipl_dft_real, 2.0);
	cvPow( ipl_dft_imaginary, ipl_dft_imaginary, 2.0);
	cvAdd( ipl_dft_real, ipl_dft_imaginary, ipl_dft_real, NULL);
	cvPow( ipl_dft_real, ipl_dft_real, 0.5 );

	// Compute log(1 + Mag)
	cvAddS( ipl_dft_real, cvScalarAll(1.0), ipl_dft_real, NULL ); // 1 + Mag
	cvLog( ipl_dft_real, ipl_dft_real ); // log(1 + Mag)
	

	// Rearrange the quadrants of Fourier image so that the origin is at
	// the image center
	//cvShiftDFT( ipl_dft_real, ipl_dft_real );

	cvMinMaxLoc(ipl_dft_real, &min, &max, NULL, NULL, NULL);
	printf("Min:%f Max:%f\n", min, max);
	cvScale(ipl_dft_real, ipl_dft_real, 1.0/(max-min), 1.0*(-min)/(max-min));
	
	cvMinMaxLoc(ipl_dft_real, &min, &max, NULL, NULL, NULL);
	printf("Min:%f Max:%f\n", min, max);
	//ipl_print_char(ipl_dft_real);
	
	//ipl_copy_char(ipl_dft_real, ipl_image);
	
	cvNamedWindow("DFT", 0);
	cvShowImage("DFT", ipl_dft_real);
	cvWaitKey(0);
}


/*
 * 
 */
int main(int argc, char** argv)
{
	// http://opencv-users.1802565.n2.nabble.com/faint-image-after-Inverse-DFT-using-cvDFT-td2193072.html
	// https://stackoverflow.com/questions/21984413/different-results-with-cvdft-and-dft-in-opencv-2-4-8
	
	//int WIDTH, HEIGHT, DIM;

	//const char * path = "bw/gray.jpeg";
	//const char * path = "grad.png";
	//const char * path = "color/lenna.jpg";
	const char * path = "lenna.png";
	//const char * path = "xadrez.png";
	//const char * path = "tel.jpg";
	//const char * path = "bw/telb.png";
	//const char * path = "collor.jpg";
	//const char * path = "bw/tinyb.jpg";

	int ROWS = 0, COLS = 0;
	IplImage * ipl_in_255 = 0, * ipl_out = 0,
			  * ipl_in_real = 0, * ipl_in_imaginary = 0, 
			  * ipl_in_complex = 0, * div = 0;
	CvMat * plan, temp;

	ipl_in_255 = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
	
	if (ipl_in_255 == 0) {
		printf("\nERROR! Image not loaded. Check in main function\n");
		exit(-1);
	}
	
	COLS = cvGetOptimalDFTSize(ipl_in_255->height - 1);
	ROWS = cvGetOptimalDFTSize(ipl_in_255->width  - 1);

	// Allocate the images and set the parameters values
	ipl_in_real      = cvCreateImage(cvSize(COLS, ROWS), IPL_DEPTH_64F, 1);
	
	ipl_in_imaginary = cvCreateImage(cvSize(COLS, ROWS), IPL_DEPTH_64F, 1);
	ipl_in_complex   = cvCreateImage(cvSize(COLS, ROWS), IPL_DEPTH_64F, 2);
	div              = cvCreateImage(cvSize(COLS, ROWS), IPL_DEPTH_64F, 1);
	
	plan             = cvCreateMat(ROWS, COLS, CV_64FC2);
	
	
	int i = 0, j = 0;
	
	for (i = 0; i < ipl_in_255->height; i++) {
		for (j = 0; j < ipl_in_255->width; j++) {
			ipl_in_real->imageData[i * ipl_in_255->width + j]      = 0;
			ipl_in_imaginary->imageData[i * ipl_in_255->width + j] = 0;
			ipl_in_complex->imageData[i * ipl_in_255->width + j]   = 0;
			div->imageData[i * ipl_in_255->width + j]              = 0;
			plan->data.ptr[i * ipl_in_255->width + j]              = 0;
		}
	}
	
	
	ipl_copy_float(ipl_in_255, ipl_in_real);
	//ipl_print(ipl_in_real);
	
	ipl_fill(div, (float) 255);
	
	ipl_divide_scalar(ipl_in_real, 255.0);
	//ipl_print(ipl_in_real);
	
	cvMerge(ipl_in_real, ipl_in_imaginary, NULL, NULL, ipl_in_complex);
	
	//ipl_print(ipl_in_complex);
	
	CvRect window = cvRect(0, 0, ipl_in_255->width, ipl_in_255->height);
	
	
	// Set the plan for operations on Fourier's transform
	cvGetSubRect(ipl_in_complex, plan, window);
	//cvCopy(&temp, plan, NULL);
	
	
	/*if (plan->cols > ipl_in_255->width) {
		cvGetSubRect(plan, &temp, cvRect(ipl_in_255->width, 0, 
			  plan->cols - ipl_in_255->width, ipl_in_255->height));
		cvZero(&temp);
	}*/
	

	
	cvDFT(plan, plan, (CV_DXT_FORWARD));  
	
	//mat_print(plan);
	
	//show_image(ipl_in_complex, 1);
	
	//cvDFT(ipl_in_complex, ipl_in_complex, CV_DXT_FORWARD, ipl_in_complex->height);

	
	
	
	 /*
	 = laplaceFFTW(, WIDTH, HEIGHT);
	


    cvDFT(src, dst, DFT_REAL_OUTPUT, 0);


	printf("Finishing the program\n");
	
	printf("\tClearing the plans\n");
	// free memory
	fftw_destroy_plan(plan);

	
	printf("\tCleaning the fftw_complex\n");
	fftw_free(complex_fft);
	fftw_free(changed_fft);

	printf("\tShowing the images\n");
	cvShowImage("IN",  image_in);
	cvShowImage("OUT", image_out);
	/*cvSaveImage("In.png", image_in, 0);
	cvSaveImage("Out.png", image_out, 0);*/
	
    /*
	cvShowImage("iplimage_dft(): mag", image_mag);
	cvShowImage("iplimage_dft(): phase", image_phase);
	//cvShowImage("iplimage_dft(): spectrum", image_spectrum);
	cvSaveImage("iplimage_dft_mag.png", image_mag, 0);
	cvSaveImage("iplimage_dft_phase.png", image_phase, 0);
	cvWaitKey(0);
	 * 
	 *

	// Free memory in the end
	fftw_destroy_plan(plan_forward);
	fftw_free(in_complex);
	fftw_free(dft_complex);
	cvReleaseImage(&image_in);
	cvReleaseImage(&image_spectrum);
	 */
	
	
	
	
	cvDFT(plan, plan , CV_DXT_INVERSE);
	
	mat_print_char(plan);
	
	//ipl_print(ipl_in_complex);
	
	show_image(plan);
	
	
	printf("\tCleaning the images\n");
	//cvReleaseImage(&image_in);
	//cvReleaseImage(&image_out);
	/*cvReleaseImage(&image_mag);
	cvReleaseImage(&image_phase);
	cvReleaseImage(&image_spectrum);*/

	return 0;
}