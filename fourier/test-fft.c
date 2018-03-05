/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  unsigned short * data_in = pnm_get_image(img);
  //char * FB_name = "";

  fftw_complex * transition = forward(rows, cols, data_in);
  unsigned short * data_out = backward(rows, cols, transition);

  for (int i = 0; i < rows * cols; i++) {
        data_in[i] = data_out[i];
  }
/*
  int i = strlen(name) - 1;
  char * a = "";
  while (name[i] != '/') {
    strcat(a, FB_name);
  }
  strcat("FB-", FB_name);
*/
  pnm_save(img, PnmRawPpm, "FB_name.ppm");

  free(transition);
  free(data_out);
  free(img);
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
  (void)name;
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
