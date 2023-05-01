#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#include "all.h"

size_t min(size_t a, size_t b) {
  if (a < b)
    return a;
  return b;
}

string *tail(string *str) {
  if (str->len != 0)
    return &(string){.ptr = str->ptr + 1, .len = str->len - 1, .lim = str->lim};
  return &(string){.ptr = NULL, .len = 0, .lim = 0};
}

/// implementation of levenshtein distance between two strings
/// see: https://en.wikipedia.org/wiki/Levenshtein_distance
size_t lev(string *a, string *b) {
  if (a->len == 0)
    return b->len;
  if (b->len == 0)
    return a->len;
  if (a->ptr[0] == b->ptr[0])
    return lev(&(string){.ptr = a->ptr + 1, .len = a->len, .lim = a->lim},
               &(string){.ptr = b->ptr + 1, .len = b->len, .lim = b->lim});
  return 1 + min(min(lev(tail(a), b), lev(a, tail(b))), lev(tail(a), tail(b)));
}
