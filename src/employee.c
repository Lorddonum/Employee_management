#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "all.h"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) ||        \
    defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__)
#define get16bits(d) (*((const uint16_t *)(d)))
#endif

#if !defined(get16bits)
#define get16bits(d)                                                           \
  ((((const uint8_t *)(d))[1] << UINT32_C(8)) + ((const uint8_t *)(d))[0])
#endif

/**
 * hash function gives back a unique identifier for each employee to be put
 * into a hash table, based on Paul Hsieh's hash with minimal changes
 * http://burtleburtle.net/bob/hash/doobs.html
 */
size_t get_hash(EMPLOYEE emp) {
  const char *data = strcat(emp.namef->ptr, emp.namel->ptr);
  size_t len = emp.namef->len + emp.namel->len;

  size_t hash = len, tmp;
  int rem;

  if (len <= 0 || data == NULL)
    return 0;

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (; len > 0; len--) {
    hash += (size_t)get16bits(data);
    tmp = (size_t)(get16bits(data + 2) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    data += 2 * sizeof(uint16_t);
    hash += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
  case 3:
    hash += (size_t)get16bits(data);
    hash ^= hash << 16;
    hash ^= (size_t)data[sizeof(uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2:
    hash += (size_t)get16bits(data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1:
    hash += (size_t)*data;
    hash ^= hash << 10;
    hash += hash >> 1;
    break;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

/// allocates an employee object
EMPLOYEE *create_employee(void) {
  EMPLOYEE *local = NULL;
  local = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
  check_alloc(local);
  local->namef = allocate_string(FIRST_NAME_LIM);
  local->namel = allocate_string(LAST_NAME_LIM);
  return local;
}

/// add employee object to hashtable

/// frees employee fields
void destruct_employee(EMPLOYEE *deleted) {
  free(deleted->namel);
  free(deleted->namef);
  free(deleted);
}

/// prints all employee fields
void show_employee(EMPLOYEE *node) {
  printf("First Name:  %s\n", node->namef->ptr);
  printf("Last Name:   %s\n", node->namel->ptr);
  printf("Identifier:  %d\n", node->mat);
  printf("Salary:      %d\n", node->salary);
  printf("Region code: %d\n", node->region.code);
  printf("Rate:        %d\n", node->region.rate);
}

void list_employees(void) {
  for (size_t i = 0; i < table_len; i++) {
    show_employee(global_table[i].val);
  }
}

void add_employee(EMPLOYEE unit) {

}
