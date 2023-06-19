#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#include "all.h"

void search_wrapper(search_mode set) {}

EMPLOYEE *linear_search_namef(EMPLOYEE *table, string *key) {}
EMPLOYEE *linear_search_namel(EMPLOYEE *table, string *key) {}
EMPLOYEE *exact_match_mat(EMPLOYEE *table, string *key) {}
EMPLOYEE *search_by_id(uint16_t key) {}

hash_table *allocate_hashtable(size_t n_elem) {
  hash_table *allocated = nullptr;
  allocated = (hash_table *)calloc(n_elem, sizeof(hash_table));
  check_alloc(allocated);
  for (size_t i = 0; i < n_elem; ++i) {
    (allocated + i)->val = create_employee();
    check_alloc(allocated + i);
  }
  return allocated;
}

hash_table *reallocate_hashtable(size_t n_elem) {
  if (n_elem < table_len)
    unreachable();
  hash_table *reallocated = nullptr;
  reallocated =
      (hash_table *)realloc(global_table, sizeof(hash_table) * n_elem);
  check_alloc(reallocated);
  return reallocated;
}

void destroy_hashtable(hash_table *dead) { free(dead); }
