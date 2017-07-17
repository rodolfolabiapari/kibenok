/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: kiev
 *
 * Created on July 1, 2017, 2:35 PM
 */

#include <stdio.h>
#include <stdlib.h>


void test()
{
  int i;
  double *in;
  double *in2;
  int j;
  int nx = 8;
  int ny = 10;
  int nyh;
  fftw_complex *out;
  fftw_plan plan_backward;
  fftw_plan plan_forward;
  unsigned int seed = 123456789;

  printf ( "\n" );
  printf ( "TEST04\n" );
  printf ( "  Demonstrate FFTW3 on a %d by %d array of real data.\n",
    nx, ny );
  printf ( "\n" );
  printf ( "  Transform data to FFT coefficients.\n" );
  printf ( "  Backtransform FFT coefficients to recover data.\n" );
  printf ( "  Compare recovered data to original data.\n" );
/*
  Create the input array, an NX by NY array of doubles.
*/
  in = ( double * ) malloc ( sizeof ( double ) * nx * ny );

  srand ( seed );

  for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < ny; j++ )
    {
      in[i*ny+j] = rand ( );
    }
  }

  printf ( "\n" );
  printf ( "  Input Data:\n" );
  printf ( "\n" );

  for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < ny; j++ )
    {
      printf ( "  %4d  %4d  %12f\n", i, j, in[i*ny+j] );
    }
  }
/*
  Create the output array OUT, which is of type FFTW_COMPLEX,
  and of a size NX * NYH that is roughly half the dimension of the input data
  (ignoring the fact that the input data is real, and the FFT
  coefficients are complex).
*/
  nyh = ( ny / 2 ) + 1;

  out = fftw_malloc ( sizeof ( fftw_complex ) * nx * nyh );

  plan_forward = fftw_plan_dft_r2c_2d ( nx, ny, in, out, FFTW_ESTIMATE );

  fftw_execute ( plan_forward );

  printf ( "\n" );
  printf ( "  Output FFT Coefficients:\n" );
  printf ( "\n" );

  for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < nyh; j++ )
    {
      printf ( "  %4d  %4d  %12f  %12f\n", 
      i, j, out[i*nyh+j][0], out[i*nyh+j][1] );
    }
  }
/*
  Recreate the input array.
*/
  in2 = ( double * ) malloc ( sizeof ( double ) * nx * ny );

  plan_backward = fftw_plan_dft_c2r_2d ( nx, ny, out, in2, FFTW_ESTIMATE );

  fftw_execute ( plan_backward );

  printf ( "\n" );
  printf ( "  Recovered input data divided by NX * NY:\n" );
  printf ( "\n" );

  for ( i = 0; i < nx; i++ )
  {
    for ( j = 0; j < ny; j++ )
    {
      printf ( "  %4d  %4d  %12f\n",  
        i, j, in2[i*ny+j] / ( double ) ( nx * ny ) );
    }
  }
/*
  Free up the allocated memory.
*/
  fftw_destroy_plan ( plan_forward );
  fftw_destroy_plan ( plan_backward );

  free ( in );
  free ( in2 );
  fftw_free ( out );

  return;
}
//*****************************************************************************/

double frand ( )

//*****************************************************************************/
/*
  Purpose:

    FRAND returns random values between 0 and 1.

  Discussion:

    The random seed can be set by a call to SRAND ( unsigned int ).

    Note that Kernighan and Ritchie suggest using

      ( ( double ) rand ( ) / ( RAND_MAX + 1 ) )

    but this seems to result in integer overflow for RAND_MAX + 1,
    resulting in negative values for the random numbers.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    23 October 2005

  Author:

    John Burkardt

  Reference:

    Brian Kernighan, Dennis Ritchie,
    The C Programming Language,
    Prentice Hall, 1988.

  Parameters:

    Output, double FRAND, a random value between 0 and 1.
*/
{
  double value;

  value = ( ( double ) rand ( ) / ( RAND_MAX ) );

  return value;
}
//*****************************************************************************/

void timestamp ( )

/******************************************************************************/
/*
  Purpose:

    TIMESTAMP prints the current YMDHMS date as a time stamp.

  Example:

    31 May 2001 09:45:54 AM

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    24 September 2003

  Author:

    John Burkardt

  Parameters:

    None
*/
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  printf ( "%s\n", time_buffer );

  return;
# undef TIME_SIZE
}




/*
 * Main
 */
int main(int argc, char** argv) {

	return (EXIT_SUCCESS);
}

