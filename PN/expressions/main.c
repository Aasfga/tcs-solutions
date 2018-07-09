#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "expr.h"

static char buffer[1<<20];

int main() {
    unsigned tests;
    for (assert(scanf("%u", &tests) == 1); tests; --tests) {
        unsigned rows, cols;
        assert(scanf("%u%u", &rows, &cols) == 2);
        assert(fgets(buffer, sizeof(buffer), stdin) != NULL);
        int * data = (int *) malloc(rows * cols * sizeof(int));
        for (unsigned i = 0; i < rows*cols; ++i)
            assert(scanf("%d", &data[i]) == 1);

        struct expr * expr = expr_create(buffer);
        int * result = expr_eval(expr, rows, cols, data);
        expr_destroy(expr);

        for (unsigned i = 0; i < rows; ++i)
            printf("%d\n", result[i]);
        free(data);
        free(result);
    }
    return 0;
}

