
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: sudo apt install libbsd-dev
// This provides strlcpy
// See "man strlcpy"
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"


int
hash(char* key)
{
    // TODO: Produce an appropriate hash value.
    int res = 0;
    for (int i=0;key[i] != '\0'; i++) {
        res += 67*(int) key[i];
    }
    return res;
}

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = malloc(sizeof(hashmap));
    hh->size = 0;
    hh->capacity = nn;
    hh->data = calloc(nn, sizeof(hashmap_pair));
    // TODO: Allocate and initialize a hashmap with capacity 'nn'.
    // Double check "man calloc" to see what that function does.
    return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    free(hh);
}

int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    int hash_val = hash(kk) % hh->capacity;

    hashmap_pair cur_pair = hh->data[hash_val];
    while(cur_pair.used && !cur_pair.tomb) {
        if(strcmp(cur_pair.key, kk) == 0) {
            return cur_pair.val;
        }
        hash_val = (hash_val+1>=hh->capacity) ? 0 : hash_val+1;
        cur_pair = hh->data[hash_val];
    }
    return -1;
    // TODO: Return the value associated with the
    // key kk.
    // Note: return -1 for key not found.
}

void
hashmap_put(hashmap* hh, char* kk, int vv)
{
    // TODO: Insert the value 'vv' into the hashmap
    // for the key 'kk', replacing any existing value
    // for that key.
    if (hh->size + 1 >= hh->capacity) {
        // printf("expanding: \n");
        hashmap* new_map = make_hashmap_presize(2 * hh->capacity);
        for(int i=0;i<hh->capacity;i++) {
            hashmap_pair pair = hashmap_get_pair(hh, i);
            if(pair.used && !pair.tomb) {
                hashmap_put(new_map, pair.key, pair.val);
            }
        }
        hashmap_pair* data = new_map->data;       
        free(new_map);
        free(hh->data);
        hh->capacity *= 2;
        hh->data = data;
    }

    int hash_val = hash(kk) % hh->capacity;
    // printf("put: %s\n, hash %d\n", kk, hash_val);

    hashmap_pair cur_pair = hashmap_get_pair(hh, hash_val);
    while(cur_pair.used && !cur_pair.tomb) {
        if(strcmp(cur_pair.key, kk) == 0){
            // printf("found key %s at %d, val: %d\n", cur_pair.key, hash_val, vv);
            hh->data[hash_val].val = vv;
            return;
        }
        hash_val = (hash_val+1>=hh->capacity) ? 0 : hash_val+1;
        cur_pair = hashmap_get_pair(hh, hash_val);
    }

    // printf("add new key: %d\n", hash_val);
    hh->data[hash_val].used = true;
    hh->data[hash_val].val = vv;
    strcpy(hh->data[hash_val].key, kk);
    // printf("copying %s to %s\n\n", kk, hh->data[hash_val].key);

    hh->size++;
}

void
hashmap_del(hashmap* hh, char* kk)
{
    // TODO: Remove any value associated with
    // this key in the map.
    int hash_val = hash(kk) % hh->capacity;

    hashmap_pair cur_pair = hashmap_get_pair(hh, hash_val);
    while(cur_pair.used && !cur_pair.tomb) {
        if(strcmp(cur_pair.key, kk) == 0){
            printf("deleting key %s at %d\n", cur_pair.key, hash_val);
            cur_pair.tomb = true;
            return;
        }
        hash_val = (hash_val+1>=hh->capacity) ? 0 : hash_val+1;
        cur_pair = hashmap_get_pair(hh, hash_val);
    }
}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    // TODO: Get the {k,v} pair stored in index 'ii'.
    // assert(ii >= 0 && ii < hh->size);
    return hh->data[ii];
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    for(int i=0;i<hh->capacity; i++) {
        hashmap_pair cur = hashmap_get_pair(hh, i);
        printf("{%s, %d, tomb: %d, used: %d}\n", cur.key, cur.val, cur.tomb, cur.used);
        // printf("{%s, %d}\n", cur->key, cur->val);
    }
    // TODO: Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
}
