/**
* @file color-transfert
* @brief transfert color from source image to target image.
*        Method from Reinhard et al. :
*        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley,
*        'Color Transfer between Images', IEEE CGA special issue on
*        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
*/

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <bcl.h>

#define D 3

static float RGB2LMS[D][D] = {
    {0.3811, 0.5783, 0.0402},
    {0.1967, 0.7244, 0.0782},
    {0.0241, 0.1288, 0.8444}
};

static float LMS2RGB[D][D] = {
    {4.4679, -3.5873, 0.1193},
    {-1.2186, 2.3809, -0.1624},
    {0.0497, -0.2439, 1.2045}
};

static float LMS2RAB[D][D] = {
    {1/sqrtf(3), 1/sqrtf(3), 1/sqrtf(3)},
    {1/sqrtf(6), 1/sqrtf(6), -2/sqrtf(6)},
    {1/sqrtf(2), -1/sqrtf(2), 0}
};

static float RAB2LMS[D][D] = {
    {sqrtf(3)/3, sqrtf(6)/6, sqrtf(2)/2},
    {sqrtf(3)/3, sqrtf(6)/6, -sqrtf(2)/2},
    {sqrtf(3)/3, -sqrtf(6)/3, 0}
};

static void mean(pnm img, float res[D]) {
    int rows = pnm_get_height(img);
    int cols = pnm_get_width(img);

    unsigned short sum[D];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 3; k++) {
                sum[k] += pnm_get_component(img, i, j, k);
            }
        }
    }

    for (size_t i = 0; i < D; i++) {
        res[D] = sum[D] / (rows * cols);
    }
}

static void standard_deviation(pnm img, float res[D]) {
    float m[D];
    mean(img, m);

    float square[D];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 3; k++) {
                square[k] += pnm_get_component(img, i, j, k) * pnm_get_component(img, i, j, k);
            }
        }
    }

    for (size_t i = 0; i < D; i++) {
        res[D] = sqrtf((square[D] / (rows * cols)) - (m[D] * m[D]));
    }
}


/**
 * produit matrice vecteur dans res
 */
static void prod_mat_vect(float mat[D][D], float vect[D], float res[D]){
    for (size_t i = 0; i < D; i++) {
        res[i] = 0;
        for (size_t j = 0; j < D; j++) {
            res[i] += mat[i][j] * vect[i];
        }
    }
}

static void process(char * ims_name, char * imt_name, char * imd_name){
    pnm ims = pnm_load(ims_name);
    pnm imt = pnm_load(imt_name);
    pnm imd = pnm_new(pnm_get_width(imt), pnm_get_height(imt), PnmRawPpm);

    /*
    step 1 : RGB --> ραβ
    step 2 : stats
    step 3 : ραβ -> RGB
    */


    pnm_save(imd, PnmRawPpm, imd_name);

    pnm_free(ims);
    pnm_free(imt);
    pnm_free(imd);
}

void usage (char *s){
    fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
    exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[]){
    if (argc != PARAM+1) usage(argv[0]);
    process(argv[1], argv[2], argv[3]);
    return EXIT_SUCCESS;
}
