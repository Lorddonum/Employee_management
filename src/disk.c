#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <err.h>
#include <errno.h>

#include "all.h"

extern FILE *disk_state;
extern EMPLOYEE *head;
extern char *path_to_disk_state;

/// open a path to a disk file and returns a reference to it
FILE *cache_disk_file(char *path) {
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
  buffer = malloc(sizeof(char) * 100);
  check_alloc(buffer);
  while (fgets(buffer, sizeof(buffer), state) != NULL) {
    cur->next = create_employee();
    sscanf(buffer, "%d, %s, %s, %d, %d, %d", &cur->mat, cur->namef, cur->namel,
           &cur->region.code_region, &cur->region.taux, &cur->salary);
    cur = cur->next;
  }
  return 0;
}

/// initialize the EMPLOYEE linked list at global head
int initlist(void) {

  if (head != NULL) {
    fprintf(stderr, "Error: trying to initialize a non-empty list\n");
    exit(EXIT_FAILURE);
  }

  if (path_to_disk_state == NULL) {
    fputs("no file specificed for long term storage\n"
          "defaulting to state.txt\n",
          stderr);
  }

  FILE *cache = cache_disk_file(path_to_disk_state);
  parse_file_to_list(cache);

  return 0;
}
