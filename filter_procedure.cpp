/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   filter_procedure.cpp
 * Author: kiev
 * 
 * Created on July 16, 2017, 1:06 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "filter_procedure.h"

/*

double distance(int col, int row, int WIDTH, int HEIGHT) {
	double d;
	double col_center = WIDTH / 2, row_center = HEIGHT / 2; 
	
	d = sqrt( 
			  pow( col - col_center, 2)
			  +
			  pow( row - row_center, 2)
			  );
	
	return d;
}

void gaussian(fftw_complex * fft, double cut_frequence, int WIDTH, int HEIGHT) 
{
	fftw_complex * fftw_out;
	double distance_center, bandwidth = 0;
	int i, j, current;
	
	create_fftw_complex(& fftw_out, WIDTH, HEIGHT);
	
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			current = j + WIDTH * i;
			
			bandwidth = fft[current][0];
					  
			fftw_out[current][0] = 1 - exp(
					  - pow( 
								(	
									pow(distance(j, i, WIDTH, HEIGHT), 2) 
										- 
									pow(cut_frequence, 2)
								)
							/
								(distance(j, i, WIDTH, HEIGHT) * bandwidth)
					  , 2));
		}
	}	
}

/*
 * 
*
%meshFourier
u = 0:lin-1;
v = 0:col-1;

u = u - floor(lin/2);
v = v - floor(col/2);
[U, V] = meshgrid(u, v);

H = -(U.^2 + V.^2);

% Dominio de frequencia
novaFourier = H .* fourier;
 *
fftw_complex * laplace(fftw_complex * fft, int WIDTH, int HEIGHT) {
	fftw_complex * fftw_out = 0;
	int row = 0, col = 0, row_mesh = 0, col_mesh = 0, current = 0;
	double power_result = 0;
	
	printf("Doing the laplace detector of edges\n");
	
	printf("\tCreating the news fftw_complex for processing\n");
	create_fftw_complex(& fftw_out, WIDTH, HEIGHT);
	
	row_mesh = HEIGHT / 2;
	col_mesh = WIDTH  / 2;
	
	printf("\tProcessing\n");
	
	for (row = 0; row < HEIGHT; row++) {
		
		for (col = 0; col < WIDTH; col++) {
			
			current = row * WIDTH + col;
			
			power_result = - ( 
					  (col - col_mesh) * (col - col_mesh)  +  
					  (row - row_mesh) * (row - row_mesh) 
					  );
			
			fftw_out[current][0] = fft[current][0] * power_result; 
			fftw_out[current][1] = fft[current][1] * power_result;
		}
	}
	
	printf("\tDone\n");
	
	return fftw_out;
}*/