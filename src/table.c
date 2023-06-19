#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#include "all.h"

EMPLOYEE *linear_search_namef(EMPLOYEE *table, string *key) {}
EMPLOYEE *linear_search_namel(EMPLOYEE *table, string *key) {}
EMPLOYEE *exact_match_mat(EMPLOYEE *table, string *key) {}

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
  // get real now, that is just absolutely impossible
  if (n_elem < table_len)
    unreachable();
  global_table =
      (hash_table *)realloc(global_table, sizeof(hash_table) * n_elem);
  check_alloc(global_table);
  return global_table;
}

void destroy_hashtable(hash_table *dead) { free(dead); }
