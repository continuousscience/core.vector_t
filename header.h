#include <stdio.h>
#include <string.h>
#include <sil_ext.h>
#include <proto_prim.h>

typedef struct {
    size_t n;
    double x[0];
} vector;

static const unsigned char vector_hash[HASH_SIZE+1] = /*!hash!*/;

// convenience functions
// Note: this makes a copy of x
static inline vector *sil_copyvector(sil_State *S, size_t n, const double *x) {
    size_t len = 8*n;
    vector *w = (vector *)malloc(sizeof(vector)+len);
    if(w == NULL)
        return w;
    w->n = n;
    memcpy(w->x, x, len);
    return w;
}

// This takes over responsibility for freeing v
static inline int sil_pushvector(sil_State *S, vector *v) {
    return sil_newuserdata(S, vector_hash, v);
}
