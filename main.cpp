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

/*
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
*/

void mat_print_double(CvMat * mat) 
{
	int i, j;
	
	for (i = 0; i < mat->rows; i++) {
		for (j = 0; j < mat->cols; j++) {
			printf("%4d: %6.3f\n", i * mat->width + j, 
					  ((double*) mat->data.ptr)[i * mat->cols + j]);
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
			printf("%4d ", ((unsigned char*) mat->data.ptr)[i * mat->cols + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}
void ipl_print_double(IplImage * ipl) 
{
	int i, j;
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			printf("%4d: %6.3f\n", i * ipl->width + j, 
					  ((double*) ipl->imageData)[i * ipl->width + j]);
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
			printf("%4d: %d\n", i * ipl->width + j, 
					  ((unsigned char*) ipl->imageData)[i * ipl->width + j]);
			fflush(stdout);
		}
		printf("\n");
	}
}



void swap_pixels(double * base_position, int upper_offset, int lower_offset)
{
	double buffer;

	buffer = base_position[upper_offset + 0];
	base_position[upper_offset + 0] = base_position[lower_offset + 0];
	base_position[lower_offset + 0] = buffer;

	buffer = base_position[upper_offset + 1];
	base_position[upper_offset + 1]= base_position[lower_offset + 1];
	base_position[lower_offset + 1] = buffer;

	/*
	buffer = base_position[upper_offset + 2];
	base_position[upper_offset + 2] = base_position[lower_offset + 2];
	base_position[lower_offset + 2] = buffer;

	buffer = base_position[upper_offset + 3];
	base_position[upper_offset + 3]= base_position[lower_offset + 3];
	base_position[lower_offset + 3] = buffer;*/
}

/*
 * Procedure that inverts the quadrants in diagonal form.
 * 
 * The quadrant 1 goes to 3 and 2 goes to 3.
 */
void shift_quadrants(CvMat * img)
{
	int full_col, full_row, half_col, half_row;
	int upper_offset, lower_offset, row, col;
	int DIM;
	
	printf("\nStarting the Shift.");
	
	DIM = 2;

	full_col = img->cols * DIM;
	full_row = img->height;

	half_col = full_col / 2;
	half_row = full_row / 2;

	// swap quadrants diagonally
	for (row = 0; row < half_row; row++) {
		
		for (col = 0; col < half_col; col+=DIM) {
			
			// Position of Second and First Quadrants.
			upper_offset = col + (full_col * row);
			
			// Position of Third and Fourth Quadrants
			lower_offset = upper_offset + // Current Position
					  half_col + // Jump to next Quadrants
					  (full_col * half_row); // bottom 			
			
			swap_pixels((double *) img->data.ptr, upper_offset, lower_offset);

			swap_pixels((double *) img->data.ptr, upper_offset + half_col, 
					  lower_offset - half_col);
		}
	}
}



void ipl_fill(IplImage * ipl, double v) 
{
	int i, j;
	
	printf("\nFilling the structure with %3.1f value.", v);
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			((double * ) ipl->imageData)[i * ipl->width + j] = v;
		}
	}
}


void ipl_copy_to_double(IplImage * src, IplImage * dst) 
{
	int i, j;
	
	printf("\nMaking a copy from uchar to double.");
	
	for (i = 0; i < src->height; i++) {
		for (j = 0; j < src->width; j++) {
			((double * ) dst->imageData)[i * dst->width + j] = 
					  ((unsigned char * ) src->imageData)[i * src->width + j];
		}
	}
}

void ipl_copy_to_char(IplImage * src, IplImage * dst) 
{
	int i, j;
	
	printf("\nMaking a copy from double to uchar.");
	
	for (i = 0; i < src->height; i++) {
		for (j = 0; j < src->width; j++) {
			((unsigned char * ) dst->imageData)[i * dst->width + j] = 
					  ((double * ) src->imageData)[i * src->width + j];
		}
	}
}

void ipl_divide_scalar(IplImage * ipl, double div) 
{
	int i, j;

	printf("\nDividing the image by %3.1f.", div);
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			((double * ) ipl->imageData)[i * ipl->width + j] = 
					  ((double * ) ipl->imageData)[i * ipl->width + j] / div;
		}
	}
}

/*
float scale_number(float t, float minOld, float maxOld, float minNew, float maxNew) {
	float r = 0;
	r = minNew + ((maxNew - minNew) / (maxOld - minOld)) * (t - minOld);
	return r;
}

void scale (IplImage * ipl, double minNew, double maxNew) {
	int i, j;
	float minOld, maxOld;
	
	
	printf("\nScaling the interval of numbers.");
	
	minOld = maxOld = ((double * ) ipl->imageData)[0];
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			if (maxOld < ((double * ) ipl->imageData)[i * ipl->width + j]) 
				maxOld = ((double * ) ipl->imageData)[i * ipl->width + j];
				
			if (minOld > ((double * ) ipl->imageData)[i * ipl->width + j]) 
				minOld = ((double * ) ipl->imageData)[i * ipl->width + j];
		}
	}
	
	for (i = 0; i < ipl->height; i++) {
		for (j = 0; j < ipl->width; j++) {
			((double * ) ipl->imageData)[i * ipl->width + j] = 
					  scale_number(
					  ((double * ) ipl->imageData)[i * ipl->width + j], 
								 minOld, maxOld, minNew, maxNew);
		}
	}
}
*/

void normalize_merge (CvMat * mat) {
	int i, j;
	
	
	printf("\nNormalizing the Merge, copying the first to others.");
	
	for (i = 0; i < mat->height; i+=4) {
		for (j = 0; j < mat->width; j+=4) {
			mat->data.ptr[i * mat->width + j + 1] = 
					  mat->data.ptr[i * mat->width + j + 0];
			mat->data.ptr[i * mat->width + j + 2] = 
					  mat->data.ptr[i * mat->width + j + 0];
			mat->data.ptr[i * mat->width + j + 3] = 
					  mat->data.ptr[i * mat->width + j + 0];
		}
	}
}

void process_image_show(CvArr * img, char isFORWARD) {
	
	printf("\nShowing and saving the image.");
	
	if (isFORWARD) {	
		cvNamedWindow("DFT", 0);
		cvShowImage("DFT", img);
		cvSaveImage("spectrum.png", img, 0);
	} else {
		cvNamedWindow("iDFT", 0);
		cvShowImage("iDFT", img);
		cvSaveImage("out.png", img, 0);
	}
}


void show_image(CvArr * in_complex, char isFORWARD) {
	
	IplImage * ipl_dft_real, *  ipl_dft_imaginary;
	double min = 0, max = 0;
	
	ipl_dft_real      = cvCreateImage(cvGetSize(in_complex), 
			  IPL_DEPTH_64F, 1);
	ipl_dft_imaginary = cvCreateImage(cvGetSize(in_complex), 
			  IPL_DEPTH_64F, 1);

	
	// Split Fourier in real and imaginary parts
	cvSplit( in_complex, ipl_dft_real, ipl_dft_imaginary, NULL, NULL);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	cvPow( ipl_dft_real, ipl_dft_real, 2.0);
	cvPow( ipl_dft_imaginary, ipl_dft_imaginary, 2.0);
	cvAdd( ipl_dft_real, ipl_dft_imaginary, ipl_dft_real, NULL);
	cvPow( ipl_dft_real, ipl_dft_real, 0.5 );

	// Compute log(1 + Mag)
	if (isFORWARD) {
		cvAddS( ipl_dft_real, cvScalarAll(1.0), ipl_dft_real, NULL ); // 1 + Mag
		cvLog( ipl_dft_real, ipl_dft_real ); // log(1 + Mag)
	}
	

	// Rearrange the quadrants of Fourier image so that the origin is at
	// the image center
	//cvShiftDFT( ipl_dft_real, ipl_dft_real );

	cvMinMaxLoc(ipl_dft_real, &min, &max, NULL, NULL, NULL);
	cvScale(ipl_dft_real, ipl_dft_real, 
			  1.0/(max-min), 1.0*(-min)/(max-min));
	
	cvMinMaxLoc(ipl_dft_real, &min, &max, NULL, NULL, NULL);
	
	process_image_show(ipl_dft_real, isFORWARD);
	
	cvReleaseImage(&ipl_dft_real);
	cvReleaseImage(&ipl_dft_imaginary);
}


/*
 * 
 */
int main(int argc, char** argv)
{
	// http://opencv-users.1802565.n2.nabble.com/faint-image-
	//after-Inverse-DFT-using-cvDFT-td2193072.html
	// https://stackoverflow.com/questions/21984413/different-
	//results-with-cvdft-and-dft-in-opencv-2-4-8
	
	const char * path = "lenna.png";

	int ROWS = 0, COLS = 0;
	
	IplImage * ipl_8_in_255 = 0,
			  * ipl_64_in_real = 0, * ipl_64_in_imaginary = 0, 
			  * ipl_64c2_in_complex = 0, * ipl_64_div = 0;
	CvMat * mat_64c2_plan;

	// Loads the image in 0..255
	ipl_8_in_255 = cvLoadImage(path, 
			  CV_LOAD_IMAGE_GRAYSCALE);
	
	// Verifies if the image was loaded correctly.
	if (ipl_8_in_255 == 0) {
		printf("\nERROR! Image not loaded. Check in main function\n");
		exit(-1);
	}
	
	// Gets the value of COLS and ROWS from origin.
	COLS = cvGetOptimalDFTSize(ipl_8_in_255->height - 1);
	ROWS = cvGetOptimalDFTSize(ipl_8_in_255->width  - 1);

	// Allocate the images
	ipl_64_in_real      = cvCreateImage(cvSize(COLS, ROWS), 
			  IPL_DEPTH_64F, 1);
	ipl_64_in_imaginary = cvCreateImage(cvSize(COLS, ROWS), 
			  IPL_DEPTH_64F, 1);
	//ipl_64_div          = cvCreateImage(cvSize(COLS, ROWS), 
	//		  IPL_DEPTH_64F, 1);
	
	ipl_64c2_in_complex = cvCreateImage(cvSize(COLS, ROWS), 
			  IPL_DEPTH_64F, 2);
	
	mat_64c2_plan       = cvCreateMat(ROWS, COLS, CV_64FC2);
	
	cvZero(ipl_64_in_real);
	cvZero(ipl_64_in_imaginary);
	cvZero(ipl_64_div);
	cvZero(ipl_64c2_in_complex);
	cvZero(mat_64c2_plan);
	
	// Copies the image to another space of memory with double format
	ipl_copy_to_double(ipl_8_in_255, ipl_64_in_real);
	
	// Copies the value 255 to all the matrix.
	// It will be used dividing the 0..255 by 255 making the scale to be 0.0..1.0 
	//ipl_fill(ipl_64_div, (double) 255);
	
	// Divides the 0..255 by 255 in each cell.
	ipl_divide_scalar(ipl_64_in_real, 255.0);
	
	// Creates the new structure with the real and imaginary numbers.
	// It will gets both structure (real and imaginary) and make a structure with two
	// channels each one.
	cvMerge(ipl_64_in_real, ipl_64_in_imaginary, NULL, NULL, 
			  ipl_64c2_in_complex);
	
	// Set the plan for operations on Fourier's transform
	// Gets the plan of the image and copy to cvMat structure.
	cvGetSubRect(ipl_64c2_in_complex, mat_64c2_plan, 
			  cvRect(0, 0, ipl_8_in_255->width, ipl_8_in_255->height));
	
	printf("\nDoing the DFT.");
	// Does the DFT
	cvDFT(mat_64c2_plan, mat_64c2_plan, (CV_DXT_FORWARD));  
	
	shift_quadrants(mat_64c2_plan);
	
	// Save the spectrum
	show_image(mat_64c2_plan, 1);
	
	laplaceOpenCV(mat_64c2_plan);
	
	//shift_quadrants(mat_64c2_plan);
	
	printf("\nDoing the iDFT.");
	// Does the Inverse
	cvDFT(mat_64c2_plan, mat_64c2_plan , CV_DXT_INVERSE);
	
	// Save the output image
	show_image(mat_64c2_plan, 0);
	
	cvWaitKey(0);
	
	
	// Freeing the memory
	cvReleaseImage(&ipl_8_in_255);
	cvReleaseImage(&ipl_64_in_real);
	cvReleaseImage(&ipl_64_in_imaginary);
	cvReleaseImage(&ipl_64_div);
	cvReleaseImage(&ipl_64c2_in_complex);
	return 0;
}