#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sqlite3.h>

#include <err.h>
#include <errno.h>

#include "all.h"

extern EMPLOYEE *head;
extern int vflag, qflag, cflag, sflag;

/// open a path to a disk file and returns a reference to it
FILE *cache_disk_file(const char *path) {
  struct stat info;
  stat(path, &info);

  if (access(path, (R_OK & W_OK)) == -1) {
    fprintf(stderr,
            "Error: can't access the state file\n"
            "%s",
            strerror(errno));
    exit(0);
  }

  if (info.st_size == 0) {
    if (vflag)
      fprintf(stderr, "Warn: state file is empty\n");
  }

  FILE *cache = NULL;
  cache = fopen(path, "r");
  if (cache == NULL) {
    fprintf(stderr, "Warn: failed to open state file\n");
    exit(2);
  }

  return cache;
}

/// incrementally parses a csv file into EMPLOYEE fields
int parse_file_to_list(FILE *state) {
  EMPLOYEE *cur = head = create_employee();
  char *buffer = NULL;
  buffer = (char *)malloc(sizeof(char) * 100);
  check_alloc(buffer);
  size_t field_count = 0;
  while (fgets(buffer, sizeof(buffer), state) != NULL) {
    field_count++;
    cur->next = create_employee();
    sscanf(buffer, "%d, %s, %s, %d, %d, %d", &cur->mat, cur->namef, cur->namel,
           &cur->region.code, &cur->region.rate, &cur->salary);
    cur = cur->next;
  }
  if (vflag)
    fprintf(stderr, "Info: initialized head with %ld", field_count);

  free(buffer);
  return 0;
}

/// initialize the EMPLOYEE linked list at global head
void initlist(const char *path_to_disk_state) {
  if (head != NULL) {
    fprintf(stderr, "Error: trying to initialize a non-empty list\n");
    exit(EXIT_FAILURE);
  }
  if (sflag) {
    if (path_to_disk_state == NULL) {
      fputs("Error: no file specificed for long term storage\n", stderr);
      exit(EXIT_FAILURE);
    }
    FILE *cache = cache_disk_file(path_to_disk_state);
    parse_file_to_list(cache);
    return;
  }
  head = create_employee();
  if (vflag)
    fprintf(stderr, "Info: initialized empty head");
}

/// TODO: dumps linked list to state file
void pastelist(const char *path_to_disk_state) {
  panic("TODO: implement pastelist");
}
