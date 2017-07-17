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

void print_ipl(IplImage * ipl) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			printf("%6.3f ", (double) ipl->imageData[i * ipl->width + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}

void fill_ipl(IplImage * ipl, double v) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			ipl->imageData[i * ipl->width + j] = v;
		}
	}
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

	double max = 0, min = 0;
	int PLAN_ROWS = 0, PLAN_COLS = 0;
	IplImage * ipl_in_255 = 0, * ipl_out = 0,
			  * ipl_in_real = 0, * ipl_in_imaginary = 0, 
			  * ipl_in_complex = 0;
	IplImage * ipl_dft_real, *  ipl_dft_imaginary, * div;
	CvMat * plan, temp;

	ipl_in_255 = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);

	if (ipl_in_255 == 0) {
		printf("\nERROR! Image not loaded. Check in main function\n");
		exit(-1);
	}

	// Allocate the images and set the parameters values
	ipl_in_real      = cvCreateImage(cvGetSize(ipl_in_255), IPL_DEPTH_64F, 1);
	
	ipl_in_imaginary = cvCreateImage(cvGetSize(ipl_in_255), IPL_DEPTH_64F, 1);
	ipl_in_complex   = cvCreateImage(cvGetSize(ipl_in_255), IPL_DEPTH_64F, 2);
	div              = cvCreateImage(cvGetSize(ipl_in_255), IPL_DEPTH_64F, 1);
	
	plan              = cvCreateMat(PLAN_ROWS, PLAN_COLS, CV_64FC2);
	
	ipl_dft_real      = cvCreateImage(cvGetSize(ipl_in_255), IPL_DEPTH_64F, 1);
	ipl_dft_imaginary = cvCreateImage(cvGetSize(ipl_in_255), IPL_DEPTH_64F, 1);
	
	PLAN_COLS = cvGetOptimalDFTSize(ipl_in_255->height - 1);
	PLAN_ROWS = cvGetOptimalDFTSize(ipl_in_255->width  - 1);
	
	/*
	// Defines the real and imaginary spaces
	// real: sets the new image im 1.0--0.0 scale.
	// imaginary: fill the space with zeros
	cvSet( div, cvScalar(255.0), NULL);
	cvConvert(ipl_in_255, ipl_in_real);
	cvDiv(ipl_in_real, div, ipl_in_real, 0);
	
	cvZero(ipl_in_imaginary);
	
	
	// Group the real and imaginary in a structure.
	// Will be real + imaginary
	cvMerge(ipl_in_real, ipl_in_imaginary, NULL, NULL, ipl_in_complex);
	
	// Set the plan for operations on Fourier's transform
	cvGetSubRect(ipl_in_complex, &temp, cvRect(0, 0, ipl_in_255->width, ipl_in_255->height));
	cvCopy(&temp, ipl_in_complex, NULL);
	
	/*if (plan->cols > ipl_in_255->width) {
		cvGetSubRect(plan, &temp, cvRect(ipl_in_255->width, 0, 
			  plan->cols - ipl_in_255->width, ipl_in_255->height));
		cvZero(&temp);
	}*/
	
	//cvSet( div, cvScalar(120.0), NULL);
	
	fill_ipl(div, 255.0);
	
	cvConvert(ipl_in_255, ipl_in_real);
	
	print_ipl(ipl_in_real);
	
	cvDiv(ipl_in_real, div, ipl_in_real, 0);
	
	cvZero(ipl_in_imaginary);
	cvMerge(ipl_in_real, ipl_in_imaginary, NULL, NULL, ipl_in_complex);
	
	cvDFT(ipl_in_complex, ipl_in_complex, (CV_DXT_FORWARD | CV_DXT_ROWS));  
	
	
	
	//cvDFT(ipl_in_complex, ipl_in_complex, CV_DXT_FORWARD, ipl_in_complex->height);





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
	cvScale(ipl_dft_real, ipl_dft_real, 1.0/(max-min), 1.0*(-min)/(max-min));
	
	cvNamedWindow("DFT", 0);
	cvShowImage("DFT", ipl_dft_real);

	
	
	
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
	
	cvWaitKey(0);
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
	
	
	
	printf("\tCleaning the images\n");
	//cvReleaseImage(&image_in);
	//cvReleaseImage(&image_out);
	/*cvReleaseImage(&image_mag);
	cvReleaseImage(&image_phase);
	cvReleaseImage(&image_spectrum);*/

	return 0;
}