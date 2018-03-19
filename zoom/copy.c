#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>

#define D 3

void zoom_copy(int factor, char * ims_name, char * imd_name) {
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  int fcols = factor * cols;
  int frows = factor * rows;
  pnm imd = pnm_new(fcols, frows, PnmRawPpm);

  unsigned short component;
  int i_imd, j_imd;
  for (int i_ims = 0; i_ims < rows; i_ims++) {
    for (int j_ims = 0; j_ims < cols; j_ims++) {
      for (int k = 0; k < D; k++) {
        component = pnm_get_component(ims, i_ims, j_ims, k);
        for (int i_add = 0; i_add < factor; i_add++) {
          for (int j_add = 0; j_add < factor; j_add++) {
            i_imd = factor * i_ims + i_add;
            j_imd = factor * j_ims + j_add;
            pnm_set_component(imd, i_imd, j_imd, k, component);
          }
        }
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);

  pnm_free(imd);
  pnm_free(ims);
}

void usage(const char *s) {
  fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[]) {
  if (argc != PARAM+1) usage(argv[0]);
  int factor = atoi(argv[1]);
  zoom_copy(factor, argv[2], argv[3]);
  return EXIT_SUCCESS;
}
