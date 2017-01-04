#include <stdio.h>
#include <string.h>
#include <sil_ext.h>
#include <proto_prim.h>

typedef struct {
    size_t n;
    double x[0];
} vector;

const unsigned char vector_hash[21] = /*!hash!*/;

// convenience functions
// Note: this makes a copy of x
int sil_copyvector(sil_State *S, size_t n, const double *x) {
    size_t len = 8*n;
    vector *w = (vector *)malloc(sizeof(vector)+len);
    if(w == NULL)
        return 1;
    w->n = n;
    memcpy(w->x, x, len);
    if(sil_newuserdata(S, vector_hash, w)) {
        free(w);
        return 1;
    }
    return 0;
}

// This takes over responsibility for freeing v
int sil_pushvector(sil_State *S, vector *v) {
    return sil_newuserdata(S, vector_hash, v);
}
