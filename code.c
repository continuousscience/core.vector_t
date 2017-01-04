char *show(const vector *v) {
    char *out;
    //asprintf(&out, "vector with %d elems", v->n);
    if(v->n == 0) {
        asprintf(&out, "[]");
    } else if(v->n < 10) {
        char *cur;
        int i, len;
        out = (char *)malloc(v->n*14 + 2);
        len = snprintf(out, 15, "[ %g", v->x[0]);
        cur = out + len;
        for(i=1; i<v->n; i++,cur+=len) {
            len = snprintf(cur, 15, ", %g", v->x[i]);
        }
        snprintf(cur, 2, "]");
    } else {
        asprintf(&out, "[ %g, %g, ..., %g, %g] (%lu elems)",
                 v->x[0],v->x[1],v->x[v->n-2],v->x[v->n-1], v->n);
    }
    return out;
}

size_t size(const vector *v) {
    return size_uint32(v->n)+v->n*8;
}

void serialize(SWriter *s, const vector *v) {
    write_uint32(s, v->n);
    s->write(s->stream, v->x, 8*v->n);
}

void parse(sil_State *S, const uint8_t *buf, size_t len) {
    uint32_t n;
    unsigned k = read_uint32(&n, buf, len);
    buf += k;
    len -= k;
    if(len != 8*n) {
        sil_err(S, "Vector claims %u elems, "
                "but contains %d bytes.\n", n, len);
        return;
    }
    if(sil_copyvector(S, n, (double *)buf)) {
        sil_err(S, "Error parsing vector.");
    }
}

void handler(vector *v) {
    printf("Called handler on %p.\n", v->x);
}

void copy(sil_State *S) {
    size_t len; // Always assume len is wrong!
    vector *v = (vector *)sil_getST(S, &len);
    if(v == NULL) {
        sil_err(S, "Can't copy - no vector present.");
        return;
    }
    len = sizeof(vector)+8*v->n;
    vector *w = (vector *)malloc(len);
    if(w == NULL) {
        sil_err(S, "Memory error.");
        return;
    }
    memcpy(w, v, len);
    sil_setST(S, w, len);
}
