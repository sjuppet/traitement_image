#include <float.h>
#include <stdlib.h>
#include <math.h>

#include <fft.h>

void padding(int factor, char * ims_name, char * imd_name){
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  int f_rows = factor * rows;
  int f_cols = factor * cols;
  pnm f_ims = pnm_new(f_cols, f_rows, PnmRawPpm);
  unsigned short * data_in;
  unsigned short * f_data_in;
  fftw_complex * freq_repr_in  = malloc(rows * cols * sizeof(fftw_complex));
  unsigned short * data_out;
  fftw_complex * f_freq_repr_out;

  //initialisation de l'image de taille finale en noir
  for (int i = 0; i < f_rows; i++) {
    for (int j = 0; j < f_cols; j++) {
      for (int k = 0; k < 3; k++) {
        pnm_set_component(f_ims, i, j, k, 0);
      }
    }
  }

//fft appliquée sur chaque channel de l'image
  for (int k = 0; k < 3; k++) {
    //fft sur l'image initiale
    data_in = pnm_get_channel(ims, NULL, k);
    freq_repr_in = forward(rows, cols, data_in);

    //centralisation de l'image initiale sur l'image de taille finale
    f_data_in = pnm_get_channel(f_ims, NULL, k);
    f_freq_repr_out  = malloc(f_rows * f_cols * sizeof(fftw_complex));

    for (int i = 0; i < f_rows * f_cols; i++) {
      f_freq_repr_out[i] = f_data_in[i] + I * 0;
    }
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        f_freq_repr_out[i * f_cols + j + (factor - 1) * cols * (1 + rows*factor)/ 2 ] = freq_repr_in[i * cols +j];
      }
    }

    //fft inverse sur la fusion des images
    data_out = backward(f_rows, f_cols, f_freq_repr_out, factor);

    pnm_set_channel(f_ims, data_out, k);

  }

  pnm_save(f_ims, PnmRawPpm, imd_name);

  free(f_freq_repr_out);
  free(data_out);
  free(freq_repr_in);
  free(f_data_in);
  free(data_in);
  pnm_free(f_ims);
  pnm_free(ims);
}

void usage (char *s){
    fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
    exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[]){
    if (argc != PARAM+1) usage(argv[0]);
    int factor = atoi(argv[1]);
    char * ims_name = argv[2];
    char * imd_name = argv[3];

    padding(factor, ims_name, imd_name);
    return EXIT_SUCCESS;
}
