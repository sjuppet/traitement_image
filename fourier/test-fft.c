/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libgen.h>
#include <fft.h>

/**
 * @brief test the forward and backward functions
 * @param char* name, the input image file name
 */

void
test_forward_backward(char* name)
{
  fprintf(stderr, "test_forward_backward: ");

  pnm img = pnm_load(name);
  int rows = pnm_get_height(img);
  int cols = pnm_get_width(img);
  unsigned short * data_in = pnm_get_channel(img, NULL, 0);
  unsigned short * data_out;
  fftw_complex * transition;

  transition = forward(rows, cols, data_in);
  data_out = backward(rows, cols, transition);

  for (int k = 0; k < 3; k++) {
    pnm_set_channel(img, data_out, k);
  }

  char filename[50], FB_name[50];
  strcpy(filename, basename(name));
  strcpy(FB_name, "FB-");
  strcat(FB_name, filename);
  pnm_save(img, PnmRawPpm, FB_name);

  free(transition);
  free(data_out);
  free(data_in);
  pnm_free(img);

  fprintf(stderr, "OK\n");
}

/**
 * @brief test image reconstruction from of magnitude and phase spectrum
 * @param char *name: the input image file name
 */
void
test_reconstruction(char* name)
{
  fprintf(stderr, "test_reconstruction: ");

    pnm img = pnm_load(name);
    int rows = pnm_get_height(img);
    int cols = pnm_get_width(img);
    unsigned short * data_in = pnm_get_channel(img, NULL, 0);
    unsigned short * data_out;
    float * as = malloc(rows * cols * sizeof(float));
    float * ps = malloc(rows * cols * sizeof(float));
    fftw_complex * freq_repr_in;
    fftw_complex * freq_repr_out = malloc(cols * rows * sizeof(fftw_complex));

    freq_repr_in = forward(rows, cols, data_in);
    freq2spectra(rows, cols, freq_repr_in, as, ps);
    spectra2freq(rows, cols, as, ps, freq_repr_out);
    data_out = backward(rows, cols, freq_repr_out);

    for (int k = 0; k < 3; k++) {
      pnm_set_channel(img, data_out, k);
    }

    char filename[50], ASPS_name[50];
    strcpy(filename, basename(name));
    strcpy(ASPS_name, "ASPS-");
    strcat(ASPS_name, filename);
    pnm_save(img, PnmRawPpm, ASPS_name);

    free(freq_repr_out);
    free(freq_repr_in);
    free(ps);
    free(as);
    free(data_out);
    free(data_in);
    pnm_free(img);

  fprintf(stderr, "OK\n");
}

/**
 * @brief test construction of magnitude and phase images in ppm files
 * @param char* name, the input image file name
 */
void
test_display(char* name)
{
  fprintf(stderr, "test_display: ");

  pnm img = pnm_load(name);
  int rows = pnm_get_height(img);
  int cols = pnm_get_width(img);
  unsigned short * data_in = pnm_get_channel(img, NULL, 0);

  float * as = malloc(rows * cols * sizeof(float));
  float * ps = malloc(rows * cols * sizeof(float));
  fftw_complex * freq_repr_in;
  freq_repr_in = forward(rows, cols, data_in);
  freq2spectra(rows, cols, freq_repr_in, as, ps);

  unsigned short *channel_as = malloc(rows * cols * sizeof(unsigned short));
  unsigned short *channel_ps = malloc(rows * cols * sizeof(unsigned short));

  float max_as = as[0];
  float max_ps = ps[0];
  for (int i = 0; i < rows * cols; i++) {
    if (as[i] > max_as) {
      max_as = as[i];
    }
    if (ps[i] > max_ps) {
      max_ps = ps[i];
    }
  }

  for (int i = 0; i < rows * cols; i++) {
    as[i] = pnm_maxval * pow(as[i] / max_as, 0.1);
    ps[i] = pnm_maxval * pow(ps[i] / max_ps, 0.1);
    channel_as[i] = (unsigned short) as[i];
    channel_ps[i] = (unsigned short) ps[i];
  }

  pnm img_freq = pnm_dup(img);
  pnm img_amp = pnm_dup(img);

  for (int k = 0; k < 3; k++) {
    pnm_set_channel(img_amp, channel_as, k);
    pnm_set_channel(img_freq, channel_ps, k);
  }

  char filename[50], AS_name[50], PS_name[50];
  strcpy(filename, basename(name));

  strcpy(AS_name, "AS-");
  strcat(AS_name, filename);
  pnm_save(img_amp, PnmRawPpm, AS_name);

  strcpy(PS_name, "PS-");
  strcat(PS_name, filename);
  pnm_save(img_freq, PnmRawPpm, PS_name);

  pnm_free(img_amp);
  pnm_free(img_freq);
  free(channel_ps);
  free(channel_as);
  free(freq_repr_in);
  free(ps);
  free(as);
  free(data_in);
  pnm_free(img);

  fprintf(stderr, "OK\n");
}

/**
 * @brief test the modification of magnitude by adding a periodic functions
          on both vertical and horizontal axis, and
 *        construct output images
 * @param char* name, the input image file name
 */
void

test_add_frequencies(char* name)
{
  fprintf(stderr, "test_add_frequencies: ");

  pnm img = pnm_load(name);
  int rows = pnm_get_height(img);
  int cols = pnm_get_width(img);
  unsigned short * data_in = pnm_get_channel(img, NULL, 0);
  unsigned short * data_out;

  float * as = malloc(rows * cols * sizeof(float));
  float * ps = malloc(rows * cols * sizeof(float));
  fftw_complex * freq_repr_in;
  freq_repr_in = forward(rows, cols, data_in);
  fftw_complex * freq_repr_out = malloc(cols * rows * sizeof(fftw_complex));
  freq2spectra(rows, cols, freq_repr_in, as, ps);

  unsigned short *channel_as = malloc(rows * cols * sizeof(unsigned short));
  unsigned short *channel_ps = malloc(rows * cols * sizeof(unsigned short));

  float max = as[0];
  for (int i = 0; i < rows * cols; i++) {
    if(as[i] > max){
      max = as[i];
    }
  }

  as[ (rows / 2 - 8) * cols + cols /2 ] = 0.25 * max;
  as[ (rows / 2 + 8) * cols + cols /2 ] = 0.25 * max;
  as[ rows / 2 * cols + rows /2 + 8] = 0.25 * max;
  as[ rows / 2 * cols + rows /2 - 8] = 0.25 * max;

  spectra2freq(rows, cols, as, ps, freq_repr_out);
  data_out = backward(rows, cols, freq_repr_out);

  for (int i = 0; i < rows * cols; i++) {
    as[i] = pnm_maxval * pow(as[i] / max, 0.1);
    channel_as[i] = (unsigned short) as[i];
    channel_ps[i] = (unsigned short) ps[i];
  }

  pnm img_amp = pnm_dup(img);

  for (int k = 0; k < 3; k++) {
    pnm_set_channel(img_amp, channel_as, k);
    pnm_set_channel(img, data_out, k);
  }

  char filename[50], FAS_name[50], FREQ_name[50];
  strcpy(filename, basename(name));

  strcpy(FAS_name, "FAS-");
  strcat(FAS_name, filename);
  pnm_save(img_amp, PnmRawPpm, FAS_name);

  strcpy(FREQ_name, "FREQ-");
  strcat(FREQ_name, filename);
  pnm_save(img, PnmRawPpm, FREQ_name);

  pnm_free(img_amp);
  free(channel_ps);
  free(channel_as);
  free(freq_repr_out);
  free(freq_repr_in);
  free(ps);
  free(as);
  free(data_out);
  free(data_in);
  pnm_free(img);

  fprintf(stderr, "OK\n");
}

void
run(char* name)
{
  test_forward_backward(name);
  test_reconstruction(name);
  test_display(name);
  test_add_frequencies(name);
}

void
usage(const char *s)
{
  fprintf(stderr, "Usage: %s <ims> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 1
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1) usage(argv[0]);
  run(argv[1]);
  return EXIT_SUCCESS;
}
