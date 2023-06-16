#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "all.h"

// helper function for levenshtein
// returns the tail as in the rest of the characters after the first one
static inline string *tail(string *str) {
  if (str->len != 0)
    return &(string){.ptr = str->ptr + 1, .len = str->len - 1, .lim = str->lim};
  return &(string){.ptr = NULL, .len = 0, .lim = 0};
}

/// implementation of levenshtein distance between two strings
/// see: https://en.wikipedia.org/wiki/Levenshtein_distance
size_t lev_dist(string *a, string *b) {
  if (a->len == 0)
    return b->len;
  if (b->len == 0)
    return a->len;
  if (a->ptr[0] == b->ptr[0])
    return lev_dist(&(string){.ptr = a->ptr + 1, .len = a->len, .lim = a->lim},
                    &(string){.ptr = b->ptr + 1, .len = b->len, .lim = b->lim});
  return 1 + min(min(lev_dist(tail(a), b), lev_dist(a, tail(b))),
                 lev_dist(tail(a), tail(b)));
}

/// TODO: implementation of Damerau-Levenshtein distance
/// https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
/// implemented using the Wagner-Fischer algorithm
/// https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm
// size_t dlwf_dist(string *a, string *b) {
//   size_t diff = a->len - b->len;
//   for (size_t i = 1 ; i < m ; ++i) {
//   
//   }
// }
/// TODO: implementation of the Rabin fingerprint used later in Rabin-karp
/// https://en.wikipedia.org/wiki/Rabin_fingerprint

/// TODO: implementation of the two-way algorithm
/// https://en.wikipedia.org/wiki/Two-way_string-matching_algorithm
size_t two_way(const char *needle, const char *haystack) {}

/// TODO: implementation of the knut-Morris-Pratt algorithm
/// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm

/// TODO: implementation of the Boyer-Moore algorithm
/// https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string-search_algorithm

/// TODO: implementation of Rabin-karp algorithm
/// https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
