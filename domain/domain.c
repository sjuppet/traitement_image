#include <math.h>

#include <domain.h>

unsigned short bilinear_interpolation(float x, float y, pnm ims, int c) {
    int rows = pnm_get_height(ims);
    int cols = pnm_get_width(ims);
    int i = y;
    int j = x;
    float dx = x - j;
    float dy = y - i;
    unsigned short interpolation = (1-dx) * (1-dy) * pnm_get_component(ims, i, j, c);
    if (i + 1 >= rows) {
        interpolation = interpolation + dx * (1-dy) * pnm_get_component(ims, i, j+1, c);
    }
    if (j + 1 >= cols) {
        interpolation = interpolation + (1-dx) * dy * pnm_get_component(ims, i+1, j, c);
    }
    else {
        interpolation = interpolation + dx * (1-dy) * pnm_get_component(ims, i, j+1, c)
                                      + (1-dx) * dy * pnm_get_component(ims, i+1, j, c)
                                      + dx * dy * pnm_get_component(ims, i+1, j+1, c);
    }
    return interpolation;
}
