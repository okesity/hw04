/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv = malloc(sizeof(svec));
    sv->data = malloc(2 * sizeof(char*));
    sv->size = 0;
    sv->capacity = 2;
    // TODO: correctly allocate and initialize data structure
    return sv;
}

void
free_svec(svec* sv)
{
    for(int i=0;i<sv->size;++i)
        free(sv->data[i]);
    free(sv->data);
    free(sv);
    return;
    // TODO: free all allocated data
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void
svec_put(svec* sv, int ii, char* item)
{
    assert(ii >= 0 && ii < sv->size);

    int size = sv->size;

    //push_back size-1
    svec_push_back(sv, sv->data[size-1]);
    // free(sv->data[size-1]);

    //copying from (data[ii], data[size-1]) to ii+1, size
    for(int i=ii+1; i<= size; ++i) {
        sv->data[i] = sv->data[i-1];
    }

    sv->data[ii] = strdup(item);
    return;

}

void
svec_push_back(svec* sv, char* item)
{
    int size = sv->size;

    if (size >= sv->capacity) {
        int capacity = sv->capacity;
        char** data = malloc(2 * capacity * sizeof(char*));
        sv->capacity *= 2;
        for(int i=0;i<size;++i) {
            data[i] = sv->data[i];
        }
        free(sv->data);
        sv->data = data;
    }

    sv->data[size] = strdup(item);
    sv->size++;
    return;
}

void
svec_swap(svec* sv, int ii, int jj)
{
    char* temp = sv->data[ii];
    sv->data[ii] = sv->data[jj];
    sv->data[jj] = temp;
    return;
}
