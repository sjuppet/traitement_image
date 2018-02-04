#include <math.h>

#include <domain.h>

unsigned short bilinear_interpolation(float x, float y, pnm ims, int c){
    int i = y;
    int j = x;
    float dx = x - j;
    float dy = y - i;
    return
        (1-dx) * (1-dy) * pnm_get_component(ims, i, j, c)
        + dx * (1-dy) * pnm_get_component(ims, i, j+1, c)
        + (1-dx) * dy * pnm_get_component(ims, i+1, j, c)
        + dx * dy * pnm_get_component(ims, i+1, j+1, c);
}
