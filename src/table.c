#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#include "all.h"

EMPLOYEE *linear_search(EMPLOYEE *table, string *key) {}

hash_table *allocate_hashtable(size_t n_elem, size_t table_len) {
  hash_table *allocated = nullptr;
  allocated = (hash_table *)calloc(n_elem, sizeof(hash_table));
  check_alloc(allocated);
  for (size_t i = 0; i < n_elem; ++i) {
    (allocated + i)->val = create_employee();
    check_alloc(allocated + i);
  }
  return allocated;
}

void destroy_hashtable(hash_table *dead) { free(dead); }
