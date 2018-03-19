#include <stdlib.h>
#include <stdio.h>
#include <bcl.h>
#include <math.h>
#include <string.h>

#define D 3

unsigned short box(float x) {
  if ((x < 0.5) && (x >= -0.5)) {
    return 1;
  }
  return 0;
}

unsigned short tent(float x) {
  if (fabs(x) <= 1) {
    return 1 - fabs(x);
  }
  return 0;
}

unsigned short bell(float x) {
  if (fabs(x) <= 0.5) {
    return 0.75 - x * x;
  }
  if ((fabs(x) > 0.5) && (fabs(x) <= 1.5)) {
    return (fabs(x) - 1.5) * (fabs(x) - 1.5) * 0.5;
  }
  return 0;
}

unsigned short mitch(float x) {
  if (fabs(x) <= 1) {
    return 1.166666667*powf(fabs(x), 3) - 2*x*x + 0.888888889;
  }
  if ((fabs(x) >= 1) && (fabs(x) <= 2)) {
    return -0.388888889*powf(fabs(x), 3) + 2*x*x - 3.333333333*fabs(x) + 1.777777778;
  }
  return 0;
}

pnm apply_filter(int factor, int WF, pnm ims, int rows, int cols) {
  int fcols = factor * cols;
  pnm imd = pnm_new(fcols, rows, PnmRawPpm);

  int left, right;
  float j;
  unsigned short S;
  for (int i = 0; i < rows; i++) {
    for (int j_imd = 0; j_imd < fcols; j_imd++) {
      j = j_imd / factor;
      left = j - WF;
      right = j + WF;
      S = 0;
      if (left < 0) {
        left = 0;
      }
      if (right >= cols) {
        right = cols - 1;
      }
      for (int k = left; k <= right; k++) {
        switch (WF) {
          case 1:
          S += pnm_get_component(ims, i, k, 0) * box(k - j);
          break;
          case 2:
          S += pnm_get_component(ims, i, k, 0) * tent(k - j);
          break;
          case 3:
          S += pnm_get_component(ims, i, k, 0) * bell(k - j);
          break;
          case 4:
          S += pnm_get_component(ims, i, k, 0) * mitch(k - j);
          break;
          default:
          break;
        }
      }
      for (int channel = 0; channel < D; channel++) {
        pnm_set_component(imd, i, j_imd, channel, S);
      }
    }
  }
return imd;
}

pnm flip(pnm ims){
  int cols, rows;

  rows = pnm_get_height(ims);
  cols = pnm_get_width(ims);
  pnm imd = pnm_new(rows, cols, PnmRawPpm);


  unsigned short *data_s = pnm_get_image(ims);
  unsigned short *data_d = pnm_get_image(imd);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int new_i = j;
      int new_j = i;
      int p = pnm_offset(ims, i, j);
      int q = pnm_offset(imd, new_i, new_j);
      for (size_t k = 0; k < 3; k++) {
        data_d[q+k] = data_s[p+k];
      }
    }

  }
  return imd;
}

void filter(int factor, char * filter_name, char * ims_name, char * imd_name) {
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  int WF;
  if (strcmp(filter_name, "box") == 0) {
    WF = 1;
  }
  else if (strcmp(filter_name, "tent") == 0) {
    WF = 2;
  }
  else if (strcmp(filter_name, "bell") == 0) {
    WF = 3;
  }
  else if (strcmp(filter_name, "mitch") == 0) {
    WF = 4;
  }
  else {
    fprintf(stderr, "Error : %s not a filter\n", filter_name);
    exit(EXIT_FAILURE);
  }

  pnm imt1 = apply_filter(factor, WF, ims, rows, cols);
  pnm imt2 = flip(imt1);

  rows = pnm_get_height(imt2);
  cols = pnm_get_width(imt2);
  pnm imt3 = apply_filter(factor, WF, imt2, rows, cols);
  pnm imd = flip(imt3);

  pnm_save(imd, PnmRawPpm, imd_name);

  pnm_free(imd);
  pnm_free(imt3);
  pnm_free(imt2);
  pnm_free(imt1);
  pnm_free(ims);
}


void usage(const char *s) {
  fprintf(stderr, "Usage: %s <factor> <filter-name> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[]) {
  if (argc != PARAM+1) usage(argv[0]);
  int factor = atoi(argv[1]);
  filter(factor, argv[2], argv[3], argv[4]);
  printf("ok\n");
  return EXIT_SUCCESS;
}
