#include <math.h>

#include <domain.h>

unsigned short bilinear_interpolation(float x, float y, pnm ims, int c) {
    int rows = pnm_get_height(ims);
    int cols = pnm_get_width(ims);
    int i = y;
    int j = x;
    float dx = x - j;
    float dy = y - i;
    unsigned short interpolation;
    if ((i >= 0) && (j >= 0) && (i < rows) && (j < cols)) {
        interpolation = (1-dx) * (1-dy) * pnm_get_component(ims, i, j, c);
        if ((i + 1 < rows) && (j + 1 < cols)) {
            return interpolation + dx * (1-dy) * pnm_get_component(ims, i, j+1, c)
            + (1-dx) * dy * pnm_get_component(ims, i+1, j, c)
            + dx * dy * pnm_get_component(ims, i+1, j+1, c);
        }
        else if ((i + 1 < rows) && (j + 1 >= cols)) {
            return interpolation + (1-dx) * dy * pnm_get_component(ims, i+1, j, c);
        }
        else if ((i + 1 >= rows) && (j + 1 < cols)) {
            return interpolation + dx * (1-dy) * pnm_get_component(ims, i, j+1, c);
        }
    }
    else if ((i >= 0) && (j < 0) && (j + 1 >= 0) && (i < rows) && (j< cols)) {
        if (i + 1 < rows) {
            return dx * (1-dy) * pnm_get_component(ims, i, j+1, c)
            + (1-dx) * dy * pnm_get_component(ims, i+1, j, c)
            + dx * dy * pnm_get_component(ims, i+1, j+1, c);
        }
    }
    else if ((i < 0) && (i + 1 >= 0) && (j >= 0) && (i < rows) && (j < cols)) {
        if (j + 1 < cols) {
            return dx * (1-dy) * pnm_get_component(ims, i, j+1, c)
            + (1-dx) * dy * pnm_get_component(ims, i+1, j, c)
            + dx * dy * pnm_get_component(ims, i+1, j+1, c);
        }
    }
    else if ((i < 0) && (j < 0) && (i + 1 >= 0) && (j + 1 >= 0)) {
        return dx * dy * pnm_get_component(ims, i+1, j+1, c);
    }
    return 0;
}
