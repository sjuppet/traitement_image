#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>
#include <math.h>
#include <domain.h>



void process(int x_center, int y_center, float angle, char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name);
    int rows = pnm_get_height(ims);
    int cols = pnm_get_width(ims);
    pnm imd = pnm_new(cols, rows, PnmRawPpm);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float x = cos(angle) * (j-x_center) - sin(angle) * (i-y_center) + x_center;
            float y = sin(angle) * (j-x_center) + cos(angle) * (i-y_center) + y_center;
            // printf("x : %5f | y : %5f\n",x, y );
            for (int c = 0; c < 3; c++) {
                pnm_set_component(imd, i, j, c, bilinear_interpolation(x, y, ims, c));
            }
        }
    }

    pnm_save(imd, PnmRawPpm, imd_name);
}

void usage(char* s) {
    fprintf(stderr,"%s <dir> <ims> <imd>\n",s);
    exit(EXIT_FAILURE);
}

#define PARAM 5
int main(int argc, char* argv[]) {
    if(argc != PARAM+1)
    usage(argv[0]);

    process(atoi(argv[1]), atoi(argv[2]), atof(argv[3]), argv[4], argv[5]);

    return EXIT_SUCCESS;
}
