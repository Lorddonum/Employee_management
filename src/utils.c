#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <ctype.h>

#include <err.h>
#include <errno.h>

#include "all.h"

/// check the success of allocation with exit on failure
/// NOTE: it should be run right after the coressponding malloc to have a relevant
/// error number displayed
void check_alloc(void *ptr) {
  if (ptr == NULL) {
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
void panic(const char *msg) {
  fputs(msg, stderr);
  abort();
}
