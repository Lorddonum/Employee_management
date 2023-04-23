#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <err.h>
#include <errno.h>
#include <stddef.h>

#include <curses.h>
#include <form.h>
#include <menu.h>
#include <sys/ioctl.h>

#include <getopt.h>
#include <locale.h>
#include <unistd.h>

#include "all.h"

extern int errno;

EMPLOYEE *head = NULL;
FILE *disk_state = NULL;
char *path_to_disk_state = NULL;

extern char *__progname;

static inline void usage(void);

extern char *optarg;
extern int optind, opterr, optopt;

int vflag, qflag, cflag, sflag;

int main(int argc, char *argv[]) {

  // flag/global vars initializers
  vflag = 0, qflag = 0; /* default off => a quieter program    */
  cflag = 0;            /* default off => there will be colors */
  sflag = 1;            /* default on => sync state with disk  */

  path_to_disk_state = (char *)malloc(sizeof(char) * PATH_MAX);

  // env vars parsing
  char *no_color = getenv("NO_COLOR");
  bool color = false;

  if (no_color != NULL && no_color[0] != '\0') {
    color = true;
  }

// TODO: setup fallback to short args parsing when getopt_long is not available
#if 0
  int opt;
  while ((opt = getopt(argc, argv, "hcdp:")) != -1) {
    switch (opt) {
    case 'c':
      color = true;
      fprintf(stderr, "Info: disabled colors\n");
      break;
    case 'p':
      path_to_disk_state = optarg;
      fprintf(stderr, "Info: given path is %s\n", optarg);
      break;
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
    default:
      usage();
      exit(EXIT_FAILURE);
    }
  }
#endif

  int opt;
  while (1) {
    static struct option long_options[] = {
        {"quiet", no_argument, &qflag, 'q'},
        {"help", no_argument, 0, 'h'},
        {"no-color", no_argument, 0, 'c'},
        {"path", required_argument, 0, 'p'},
        {"stateless", no_argument, 0, 's'},
        {"verbose", no_argument, &vflag, 'v'},
        {0, 0, 0, 0},
    };

    int option_index = 0;
    opt = getopt_long(argc, argv, "chqvp:", long_options, &option_index);
    if (opt == -1)
      break;

    switch (opt) {
    case 0:
      /* If this option set a flag, do nothing else now. */
      if (long_options[option_index].flag != 0)
        break;
      printf("option %s", long_options[option_index].name);
      if (optarg)
        printf(" with arg %s", optarg);
      printf("\n");
      break;

    case 'q':
      qflag = 1;
      break;

    case 'c':
      cflag = 1;
      break;

    case 'p':
      path_to_disk_state = optarg;
      fprintf(stderr, "Info: given path to state file is %s\n", optarg);
      break;

    case 's':
      sflag = 0;
      break;

    case 'h':
      usage();
      exit(EXIT_SUCCESS);

    case 'v':
      vflag = 1;
      break;

    case '?':
      /* getopt_long already printed an error message. */
      break;

    default:
      fprintf(stderr, "Error: unexpected failure while reading cli args\n");
      abort();
    }
  }

  vflag &= !qflag;
  cflag |= color;

  if (vflag)
    fprintf(stderr, "Info: verbose mode is on\n");

  // warn on extraneous args
  if (optind < argc) {
    fprintf(stderr, "Warn: extra unused argv elements\n");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    putchar('\n');
  }

  initlist();

  setlocale(LC_ALL, "");

  WINDOW *mainwind;

  if ((mainwind = initscr()) == NULL) {
    fprintf(stderr, "Error initializing ncurses.\n");
    exit(EXIT_FAILURE);
  }

  // only enables color if NO_COLOR is unset and no -c / --no-color is passed
  if (!cflag)
    start_color();
  /*
   * we exercise complete control over all the input of the program
   * including control sequences, ex: Ctrl+C, Ctrl+D, etc...
   */
  raw();
  // enables function keys F1-F12, arrow keys, etc...
  keypad(stdscr, TRUE);
  // disables echoing by getch
  noecho();

  // TODO: add main menu
  char *choices_main[5] = {
      "1 - Adding employee record",
      "2 - Delete employee record",
      "3 - Modify employee field ",
      "4 - Search for employee by field",
      "0 - Quit",
  };

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  WINDOW *main_menu = newwin(10, 40, 4, 4);
  keypad(main_menu, TRUE);

  // test program
#if 0
  int ch;
  ch = getch();
  printw("The pressed key is ");
  attron(A_BOLD);
  printw("%c", ch);
  attroff(A_BOLD);
  refresh();
  getch();
#endif

  // TODO: add search menu
  char *choices_search[3] = {
      "1 - By Identifier",
      "2 - By Name",
      "3 - Go back to main menu",
  };

UNCURSE:
  delwin(mainwind);
  endwin();
  refresh();

  return EXIT_SUCCESS;
}

static inline void usage(void) {
  fprintf(stderr,
          "%s - Manage employees\n"
          "usage: %s [-p <path>] [-c] [-v] [-h]\n"
          "\t-c, --no-color       disable color\n"
          "\t-v, --verbose        enable verbose logging\n"
          "\t-q, --quiet          disable verbose logging\n"
          "\t-p, --path PATH      specify path of the state file\n"
          "\t-s, --stateless      disable use of a state file\n"
          "\t-h, --help           prints out this help\n",
          __progname, __progname);
}
