#include <float.h>
#include <stdlib.h>
#include <math.h>

#include <fft.h>

#define PI 3.14159265359

void shift(int rows, int cols, float *buffer){
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if((i+j)%2!=0){
        buffer[i*cols + j] = - buffer[i*cols +j];
      }
    }
  }
}

fftw_complex * forward(int rows, int cols, unsigned short* g_img) {
  int size = rows * cols;
  float *float_g_img = malloc(size * sizeof(float));
  for (int i = 0; i < size; i++) {
    float_g_img[i] = g_img[i];
  }

  shift(rows, cols, float_g_img);

  fftw_complex * c_in = malloc(size * sizeof(fftw_complex));
  fftw_complex * c_out = malloc(size * sizeof(fftw_complex));

  for (int i = 0; i < size; i++) {
    c_in[i] = float_g_img[i] + I * 0;
  }

  fftw_plan plan = fftw_plan_dft_2d(rows, cols, c_in, c_out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);

  fftw_destroy_plan(plan);
  free(c_in);
  free(float_g_img);
  return c_out;
}

unsigned short * backward(int rows, int cols, fftw_complex * freq_repr) {
  int size = rows * cols;
  fftw_complex * c_out = malloc(size * sizeof(fftw_complex));
  float *float_img = malloc(size * sizeof(float));
  unsigned short * img = malloc(size * sizeof(unsigned short));

  fftw_plan plan = fftw_plan_dft_2d(rows, cols, freq_repr, c_out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(plan);

  for (int i = 0; i < size; i++) {
    float_img[i] = creal(c_out[i]) / size;
  }
  shift(rows, cols, float_img);

  for (int i = 0; i < size; i++) {
    if(float_img[i] > pnm_maxval){
      float_img[i] = pnm_maxval;
    }
    if(float_img[i] < 0){
      float_img[i] = 0;
    }
    img[i] = (unsigned short) float_img[i];
  }

  fftw_destroy_plan(plan);
  free(float_img);
  free(c_out);
  return img;
}

void freq2spectra(int rows, int cols, fftw_complex* freq_repr, float* as, float* ps) {
  float re_square;
  float im_square;
  float re;
  float im;

  for (int i = 0; i < cols * rows; i++) {
    re = creal(freq_repr[i]);
    im = cimag(freq_repr[i]);
    re_square = re * re;
    im_square = im * im;

    as[i] = sqrtf(re_square + im_square);
    ps[i] = atan2f(im, re);
  }
}


void
spectra2freq(int rows, int cols, float* as, float* ps, fftw_complex* freq_repr)
{
  for (int i = 0; i < rows * cols; i++) {
    freq_repr[i] = as[i] * cosf(ps[i]) + I * as[i] * sinf(ps[i]);
  }
}
