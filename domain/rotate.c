#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>



void process(int x, int y, float angle, char *ims_name, char *imd_name){
    pnm ims = pnm_load(ims_name);
    int rows = pnm_get_height(ims);
    int cols = pnm_get_width(ims);
    pnm imd = pnm_new(cols, rows, PnmRawPpm);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            float xp = cos(angle) * (j-x) + sin(angle) * (i-y) + x;
            float yp = -sin(anlge) * (j-x) + cos(angle) * (i-y) + y;
            for (size_t k = 0; k < 3; k++) {
                
            }
        }
    }
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
