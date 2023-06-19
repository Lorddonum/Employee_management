#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <ctype.h>

#include <err.h>
#include <errno.h>

#include "all.h"

/// general purpose string type allocation
string *allocate_string(size_t size) {
  string *allocated = nullptr;
  allocated = (string *)malloc(sizeof(string));
  allocated->ptr = (char *)malloc(sizeof(char) * size);
  check_alloc(allocated->ptr);
  memset(allocated->ptr, '\0', size);
  allocated->lim = size;
  allocated->len = 0;
  return allocated;
}

/// general purpose string type destructor
void destruct_string(string *str) {
  free(str->ptr);
  str->lim = 0;
  str->len = 0;
}

/// check the success of allocation with exit on failure
/// NOTE: it should be run right after the coressponding malloc to have a
/// relevant error number displayed
void check_alloc(void *ptr) {
  if (ptr == nullptr) {
    fprintf(stderr, "Error value: %d\n", errno);
    fprintf(stderr, "%s\n", strerror(errno));
    exit(2);
  }
}

/// checks if a string is made of letters only
int check_string(char *buffer) {
  for (size_t index = 0; index < strlen(buffer); index++) {
    if (!isalpha(buffer[index]))
      return true;
  }
  return false;
}

/// abort with an error message
inline void panic(const char *msg) {
  fputs(msg, stderr);
  abort();
}

/// general purpose comparison
int cmp(int8_t a, int8_t b) {
  if (a > b)
    return 1;
  if (a == b)
    return 0;
  if (a < b)
    return -1;
  unreachable();
}

// helper function for levenshtein
// takes two numbers, returns the smaller one
size_t min(size_t a, size_t b) {
  if (a < b)
    return a;
  return b;
}
