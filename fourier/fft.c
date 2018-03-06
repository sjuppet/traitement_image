#include <float.h>
#include <stdlib.h>
#include <math.h>

#include <fft.h>

#define PI 3.14159265359

fftw_complex * forward(int rows, int cols, unsigned short* g_img) {
  int size = rows * cols;
  fftw_complex * c_in = malloc(size * sizeof(fftw_complex));
  fftw_complex * c_out = malloc(size * sizeof(fftw_complex));

  for (int i = 0; i < size; i++) {
    c_in[i] = g_img[i] + I * 0;
  }

  fftw_plan plan = fftw_plan_dft_2d(rows, cols, c_in, c_out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);

  fftw_destroy_plan(plan);
  free(c_in);
  return c_out;
}

unsigned short * backward(int rows, int cols, fftw_complex * freq_repr) {
  int size = rows * cols;
  fftw_complex * c_out = malloc(size * sizeof(fftw_complex));
  unsigned short * img = malloc(size * sizeof(unsigned short));

  fftw_plan plan = fftw_plan_dft_2d(rows, cols, freq_repr, c_out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(plan);

  for (int i = 0; i < size; i++) {
    img[i] = creal(c_out[i]) / size;
  }

  fftw_destroy_plan(plan);
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
