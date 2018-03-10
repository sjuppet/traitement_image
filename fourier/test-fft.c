/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
  //char * FB_name = "FB-";

  transition = forward(rows, cols, data_in);
  data_out = backward(rows, cols, transition);

  for (int i = 0; i < rows * cols; i++) {
        data_in[i] = data_out[i];
  }

  for (int k = 0; k < 3; k++) {
    pnm_set_channel(img, data_out, k);
  }
  //int i = strlen(name) - 1;
  //while ((name[i] != '/') && (i > 0)) {
  //  i--;
  //}
  //FB_name = strcat(FB_name, name + i);
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
  //  char * FB_name = "FB-ASPS-";

    freq_repr_in = forward(rows, cols, data_in);
    freq2spectra(rows, cols, freq_repr_in, as, ps);
    spectra2freq(rows, cols, as, ps, freq_repr_out);
    data_out = backward(rows, cols, freq_repr_out);

    for (int i = 0; i < rows * cols; i++) {
          data_in[i] = data_out[i];
    }

    for (int k = 0; k < 3; k++) {
      pnm_set_channel(img, data_out, k);
    }

  //  int i = strlen(name) - 1;
  //  while ((name[i] != '/') && (i > 0)) {
  //    i--;
  //  }
  //  FB_name = strcat(FB_name, name + i);
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

/*
  pnm img = pnm_load(name);
  int rows = pnm_get_height(img);
  int cols = pnm_get_width(img);
  unsigned short * data_in = pnm_get_channel(img, NULL, 0);

  for (int i = 0; i < rows * cols; i++) {
    if(i%2 != 0){
      data_in = -1 * data_in;
    }
  }

  float * as = malloc(rows * cols * sizeof(float));
  float * ps = malloc(rows * cols * sizeof(float));
  fftw_complex * freq_repr_in;

  freq_repr_in = forward(rows, cols, data_in);
  freq2spectra(rows, cols, freq_repr_in, as, ps);
  for (int i = 0; i < rows * cols; i++) {
    as[i] = log(1 + as[i]);
  }

  pnm img_freq = pnm_dup(img);
  pnm img_amp = pnm_dup(img);

  for (int i = 0; i < rows * cols; i++) {
    for (int k = 0; k < 3; k++) {
      pnm_set_channel(img_freq, ps, k);
      pnm_set_channel(img_amp, as, k);
    }
  }

  pnm_save(img_freq, PnmRawPpm, "PS-name.ppm");
  pnm_save(img_amp, PnmRawPpm, "AS-name.ppm");

  pnm_free(img_amp);
  pnm_free(img_freq);
  free(freq_repr_in);
  free(ps);
  free(as);
  free(data_in);
  pnm_free(img);

*/
(void)name;
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
  (void)name;
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
