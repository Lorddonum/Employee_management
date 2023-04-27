#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <err.h>
#include <errno.h>
#include <stddef.h>

#ifdef __unix
#include <curses.h>
#include <form.h>
#include <menu.h>
#elif defined(_WIN32) || defined(WIN32)
#include <ncurses/form.h>
#include <ncurses/menu.h>
#include <ncurses/ncurses.h>
#include <winioctl.h>
#endif

#ifdef __GNUC__
#include <getopt.h>
#endif
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif
#include <locale.h>
#include <unistd.h>

#include "all.h"

extern int errno;

EMPLOYEE *head = NULL;
term_state *term = NULL;

extern char *__progname;

static inline void usage(void);

extern char *optarg;
extern int optind, opterr, optopt;

int vflag, qflag, cflag, sflag, lflag;

int main(int argc, char *argv[]) {

  // flag/global vars initializers
  vflag = 0, qflag = 0; /* default off => a quieter program    */
  cflag = 0;            /* default off => there will be colors */
  sflag = 1;            /* default on => sync state with disk  */
  lflag = 0;            /* default off => no redirect of stderr */

  char *path_to_disk_state =
      (char *)calloc(sizeof(char) * PATH_MAX, sizeof(char));
  check_alloc(path_to_disk_state);
  char *path_to_log_file =
      (char *)calloc(sizeof(char) * PATH_MAX, sizeof(char));
  check_alloc(path_to_log_file);

  // env vars parsing
  char *no_color = getenv("NO_COLOR");
  int color = 0;

  if (no_color != NULL && no_color[0] != '\0') {
    color = 1;
  }

// fallback to short args parsing when getopt_long is not available
#if !defined(__GNUC__)
  int opt;
  while ((opt = getopt(argc, argv, "hcqsvl:p:")) != -1) {
    switch (opt) {
    case 'c':
      color = true;
      fprintf(stderr, "Info: disabled colors\n");
      break;
    case 'p':
      path_to_disk_state = optarg;
      fprintf(stderr, "Info: given path is %s\n", optarg);
      break;
    case 'q':
      qflag = 1;
      break;
    case 'v':
      vflag = 1;
      break;
    case 's':
      sflag = 0;
      break;
    case 'l':
      path_to_log_file = optarg;
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
    default:
      usage();
      exit(EXIT_FAILURE);
    }
  }
#else
  int opt;
  while (1) {
    static struct option long_options[] = {
        {"quiet", no_argument, &qflag, 'q'},
        {"help", no_argument, 0, 'h'},
        {"no-color", no_argument, &cflag, 'c'},
        {"path", required_argument, 0, 'p'},
        {"stateless", no_argument, 0, 's'},
        {"log-file", required_argument, &lflag, 'l'},
        {"verbose", no_argument, &vflag, 'v'},
        {0, 0, 0, 0},
    };

    int option_index = 0;
    opt = getopt_long(argc, argv, "chqsvl:p:", long_options, &option_index);
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
      cflag = 0;
      break;

    case 'p':
      path_to_disk_state = optarg;
      fprintf(stderr, "Info: given path to state file is %s\n", optarg);
      break;

    case 's':
      sflag = 0;
      break;

    case 'l':
      lflag = 1;
      path_to_log_file = optarg;
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
#endif

  // default state file to "state/record.txt"
  if (path_to_disk_state[0] == '\0')
    path_to_disk_state = "state/record.txt";

  // redirect stderr to supplied path
  if (lflag) {
    FILE *Fp = freopen(path_to_log_file, "a+", stderr);
    if (Fp == NULL) {
      panic("Fatal: failed redirection of stderr");
    }
  }

  // warn on extraneous args
  if (optind < argc) {
    fprintf(stderr, "Warn: extra unused argv elements\n");
    while (optind < argc)
      fprintf(stderr, "%s ", argv[optind++]);
    fprintf(stderr, "\n");
  }

  vflag &= !qflag;
  cflag |= color;

  // general loging
  if (vflag) {
    // TODO: add miscellaneous info here
    fprintf(stderr, "Info: verbose mode is on\n");
    if (cflag)
      fprintf(stderr, "Info: color mode is on\n");
    if (!sflag)
      fprintf(stderr, "Info: state file at '%s'\n", path_to_disk_state);
    else
      fprintf(stderr, "Info: saving state to disk is disabled\n");
  }

  //------------------------------------------------------------------//
  //               Initializing curses && EMPLOYEE list               //
  //------------------------------------------------------------------//

  initlist(path_to_disk_state);

  setlocale(LC_ALL, "");

  WINDOW *mainwind = initscr();

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
  init_pair(1, COLOR_RED, COLOR_BLACK);
  // disables echoing by getch
  noecho();

  //------------------------------------------------------------------//
  //                            home menu                             //
  //------------------------------------------------------------------//

  // TODO: use the window's size in all calculation of positions
  term = (term_state *)malloc(sizeof(term_state));
  getmaxyx(stdscr, term->row, term->col);
  char mesg[] = "Welcome";
  mvprintw((int)term->row / 2, (term->col - (int)strlen(mesg)) / 2, "%s", mesg);
  if (vflag == 1)
    mvprintw(term->row - 2, 0, "This screen has %d rows and %d columns\n",
             term->row, term->col);

  //------------------------------------------------------------------//
  //                         main menu logic                          //
  //------------------------------------------------------------------//

  // TODO: add main menu
  const char *choices_main[5] = {
      "Adding employee record",
      "Delete employee record",
      "Modify employee field ",
      "Search for employee by field",
      "Quit",
  };

  ITEM **main_items;
  MENU *main_menu;
  WINDOW *main_menu_win;
  size_t main_choices_count;

  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  main_choices_count = ARRAY_SIZE(choices_main);
  main_items = (ITEM **)calloc(main_choices_count, sizeof(ITEM *));
  for (size_t i = 0; i < main_choices_count; ++i) {
    main_items[i] = new_item(choices_main[i], choices_main[i]);
  }

  /* Create menu */
  main_menu = new_menu(main_items);

  /* Create the window to be associated with the menu */
  main_menu_win = newwin(10, 40, 4, 4);
  keypad(main_menu_win, TRUE);

  /* Set main window and sub window */
  set_menu_win(main_menu, main_menu_win);
  set_menu_sub(main_menu, derwin(main_menu_win, 6, 38, 3, 1));

  /* Set menu mark to the string " * " */
  set_menu_mark(main_menu, " * ");

  box(main_menu_win, 0, 0);
  // print_in_middle(main_menu_win, "Main Menu", COLOR_PAIR(1));
  mvwaddch(main_menu_win, 2, 0, ACS_LTEE);
  mvwhline(main_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(main_menu_win, 2, 39, ACS_RTEE);
  mvprintw(LINES - 2, 0, "F1 to exit");
  refresh();

  post_menu(main_menu);
  wrefresh(main_menu_win);

  int c;
  while ((c = wgetch(main_menu_win)) != KEY_F(1)) {
    switch (c) {
    case KEY_DOWN:
      menu_driver(main_menu, REQ_DOWN_ITEM);
      break;
    case KEY_UP:
      menu_driver(main_menu, REQ_UP_ITEM);
      break;
    case 'Q':
      goto UNCURSE;
      break;
    }
    wrefresh(main_menu_win);
  }

  //------------------------------------------------------------------//
  //                         operations menu                          //
  //------------------------------------------------------------------//

  // TODO: add search menu
  const char *choices_search[3] = {
      "1 - By Identifier",
      "2 - By Name",
      "3 - Go back to main menu",
  };

  //------------------------------------------------------------------//
  //                           fuzzy search                           //
  //------------------------------------------------------------------//

  // TODO: add fuzzy search

  //------------------------------------------------------------------//
  //                         resource cleanup                         //
  //------------------------------------------------------------------//

  // NOTE: more cleanup can be done

UNCURSE:

  free(term);
  free(path_to_disk_state);
  free(path_to_log_file);

  unpost_menu(main_menu);
  free_menu(main_menu);
  for (size_t i = 0; i < main_choices_count; i++) {
    free_item(main_items[i]);
  }

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
