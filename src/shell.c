#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <editline.h>
#include <string.h>

#include "all.h"

static char *list[] = {"help ",   "init ", "load ",   "list ", "listall ",
                       "modify ", "view ", "search ", "save ", NULL};
static char *my_rl_complete(char *token, int *match) {
  int i;
  int index = -1;
  int matchlen = 0;
  int count = 0;

  for (i = 0; list[i]; i++) {
    int partlen = strlen(token); /* Part of token */

    if (!strncmp(list[i], token, partlen)) {
      index = i;
      matchlen = partlen;
      count++;
    }
  }

  if (count == 1) {
    *match = 1;
    return strdup(list[index] + matchlen);
  }

  return NULL;
}

/* Return all possible completions. */
static int my_rl_list_possib(char *token, char ***av) {
  int i, num, total = 0;
  char **copy;

  for (num = 0; list[num]; num++)
    ;

  if (!num)
    return 0;

  copy = malloc(num * sizeof(char *));
  for (i = 0; i < num; i++) {
    if (!strncmp(list[i], token, strlen(token))) {
      copy[total] = strdup(list[i]);
      total++;
    }
  }
  *av = copy;

  return total;
}

el_status_t list_possible(void) {
  char **av;
  char *word;
  int ac;

  word = el_find_word();
  ac = rl_list_possib(word, &av);
  if (word)
    free(word);
  if (ac) {
    el_print_columns(ac, av);
    while (--ac >= 0)
      free(av[ac]);
    free(av);

    return CSmove;
  }

  return el_ring_bell();
}

el_status_t do_suspend(void) {
  puts("Abort!");
  return CSstay;
}

el_status_t do_end(void) {
  puts("Bye!");
  return CSdone;
}

static void breakit(int signo) {
  (void)signo;
  puts("Got SIGINT");
}

/* Use el_no_echo when reading passwords and similar */
static int unlock(const char *passwd) {
  char *prompt = "Enter password: ";
  char *line;
  int rc = 1;

  el_no_echo = 1;

  while ((line = readline(prompt))) {
    rc = strncmp(line, passwd, strlen(passwd));
    free(line);

    if (rc) {
      printf("\nWrong password, please try again, it's secret.\n");
      continue;
    }

    printf("\nAchievement unlocked!\n");
    break;
  }

  el_no_echo = 0;

  return rc;
}

static inline void print_help(void) {
  fprintf(stderr, "help - prints this help\n"
                  "init - initialises the database\n"
                  "load - loads the data into a hashmap\n"
                  "list - list entries meeting a criteria\n"
                  "listall - list all entries in memory\n"
                  "modify - modify an entry\n"
                  "view - view details of an entry\n"
                  "search - enters search mode\n"
                  "save - save the state to the datastore\n");
}

int shell_loop(char *path_to_disk_state, char *path_to_log_file) {
  char *line;
  char *HISTORY = strcat(path_to_log_file, "-history");
  // we shall have a dynamic prompt
  char *prompt = nullptr;
  prompt = (char *)malloc(sizeof(char) * 100);
  strcpy(prompt, "EMP> ");

  /* Setup callbacks */
  rl_set_complete_func(&my_rl_complete);
  rl_set_list_possib_func(&my_rl_list_possib);

  el_bind_key('?', list_possible);
  el_bind_key(CTL('Z'), do_suspend);
  el_bind_key(CTL('D'), do_end);
  read_history(HISTORY);

  while ((line = readline(prompt)) != NULL) {
    /* first handle quiting */
    if (!strncmp(line, "quit", 4) || !strncmp(line, "exit", 4)) {
      fprintf(stderr, "Bye!\n");
      break;
    } /* then handle the rest of the base commands */
    else if (!strncmp(line, "help", 4))
      print_help();
    else if (!strncmp(line, "init", 4))
      force_init(path_to_disk_state);
    else if (!strncmp(line, "load", 4))
      load_all(path_to_disk_state);
    else if (!strncmp(line, "listall", 7))
      list_employees();
    else
      puts("unknown command");
    free(line);
  }

  write_history(HISTORY);

  return 0;
}
