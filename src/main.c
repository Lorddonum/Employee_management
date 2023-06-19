#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#ifdef __GNUC__
#define _GNU_SOURCE
#include <getopt.h>
#endif
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif
#include <locale.h>
#include <unistd.h>

#include "all.h"

#include "../libs/raylib/zig-out/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../libs/raygui/src/raygui.h"

extern int errno;

extern char *__progname;

static inline void usage(void);

extern char *optarg;
extern int optind, opterr, optopt;

bool aflag, vflag, qflag, cflag, sflag, tflag, lflag;

hash_table *global_table = nullptr;
size_t table_len = 0;

int main(int argc, char *argv[]) {
  /* cli */
  vflag = false, qflag = false; /* default off => a quieter program    */
  sflag = true;                 /* default on => sync state with disk  */
  lflag = false;                /* default off => no redirect of stderr */
  aflag = false;                /* default off => won't use sqlite */
  tflag = false;                /* default off => gui on */

  char *path_to_disk_state = (char *)malloc(sizeof(char) * PATH_MAX);
  check_alloc(path_to_disk_state);
  char *path_to_log_file = (char *)malloc(sizeof(char) * PATH_MAX);
  check_alloc(path_to_log_file);

#if !defined(__GNUC__)
  int opt;
  while ((opt = getopt(argc, argv, "hcqstvl:p:")) != -1) {
    switch (opt) {
    case 'a':
      aflag = true;
      break;
    case 'p':
      strcpy(path_to_disk_state, optarg);
      fprintf(stderr, "Info: given path is %s\n", optarg);
      break;
    case 'q':
      qflag = true;
      break;
    case 'v':
      vflag = true;
      break;
    case 't':
      tflag = true;
      break;
    case 's':
      sflag = false;
      break;
    case 'l':
      strcpy(path_to_log_file, optarg);
      break;
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
    default:
      unreachable();
    }
  }
#else
  int opt;
  while (true) {
    static struct option long_options[] = {
        {"quiet", no_argument, 0, 'q'},
        {"help", no_argument, 0, 'h'},
        {"no-color", no_argument, 0, 'c'},
        {"path", required_argument, 0, 'p'},
        {"stateless", no_argument, 0, 's'},
        {"sqlite", no_argument, 0, 'a'},
        {"log-file", required_argument, 0, 'l'},
        {"text-mode", no_argument, 0, 't'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0},
    };

    int option_index = 0;
    opt = getopt_long(argc, argv, "achqsvtl:p:", long_options, &option_index);
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

    case 'a':
      aflag = true;
      break;

    case 'q':
      qflag = true;
      break;

    case 'p':
      strcpy(path_to_disk_state, optarg);
      break;

    case 's':
      sflag = false;
      break;

    case 't':
      tflag = true;
      break;

    case 'l':
      lflag = true;
      strcpy(path_to_log_file, optarg);
      break;

    case 'h':
      usage();
      exit(EXIT_SUCCESS);

    case 'v':
      vflag = true;
      break;

    case '?':
      /* getopt_long already printed an error message. */
      break;

    default:
      unreachable();
    }
  }
#endif

  // default state file to "state/record.txt"
  if (sflag && path_to_disk_state[0] == '\0')
    strcpy(path_to_disk_state, "state/record.txt");

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

  /* general loging */
  if (vflag) {
    // TODO: add miscellaneous info here
    fprintf(stderr, "Info: verbose mode is on\n");
    if (!sflag) {
      fprintf(stderr, "Info: state file at '%s'\n", path_to_disk_state);
    } else {
      fprintf(stderr, "Info: saving state to disk is disabled\n");
    }
  }

  if (!tflag) {
    //----------------------------------------------------------------------------
    // Gui main entry point
    //----------------------------------------------------------------------------

    const int screenWidth = 1024;
    const int screenHeight = 650;

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Employee Manager");

    Vector2 mousePosition = {0};
    Vector2 windowPosition = {500, 200};
    Vector2 panOffset = mousePosition;
    bool dragWindow = false;

    SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

    bool exitWindow = false;

    SetTargetFPS(60);

    // Example values

    EMPLOYEE temp[4];

    temp[0].mat = 123;
    temp[0].salary = 20000;
    temp[0].namef = (string *)malloc(sizeof(string));
    temp[0].namel = (string *)malloc(sizeof(string));
    temp[0].namef->ptr = (char *)malloc(50 * sizeof(char));
    temp[0].namel->ptr = (char *)malloc(50 * sizeof(char));
    strcpy(temp[0].namef->ptr, "Ali");
    strcpy(temp[0].namel->ptr, "Bouqlwa");
    temp[0].region.code = 103;
    temp[0].region.rate = 13;

    temp[1].mat = 122;
    temp[1].salary = 20000;
    temp[1].namef = (string *)malloc(sizeof(string));
    temp[1].namel = (string *)malloc(sizeof(string));
    temp[1].namef->ptr = (char *)malloc(50 * sizeof(char));
    temp[1].namel->ptr = (char *)malloc(50 * sizeof(char));
    strcpy(temp[1].namef->ptr, "Zakaria");
    strcpy(temp[1].namel->ptr, "Bouqliwa");
    temp[1].region.code = 102;
    temp[1].region.rate = 12;

    temp[2].mat = 121;
    temp[2].salary = 20000;
    temp[2].namef = (string *)malloc(sizeof(string));
    temp[2].namel = (string *)malloc(sizeof(string));
    temp[2].namef->ptr = (char *)malloc(50 * sizeof(char));
    temp[2].namel->ptr = (char *)malloc(50 * sizeof(char));
    strcpy(temp[2].namef->ptr, "Bouflouss");
    strcpy(temp[2].namel->ptr, "YekhMennek");
    temp[2].region.code = 101;
    temp[2].region.rate = 11;

    temp[3].mat = 120;
    temp[3].salary = 20000;
    temp[3].namef = (string *)malloc(sizeof(string));
    temp[3].namel = (string *)malloc(sizeof(string));
    temp[3].namef->ptr = (char *)malloc(50 * sizeof(char));
    temp[3].namel->ptr = (char *)malloc(50 * sizeof(char));
    strcpy(temp[3].namef->ptr, "Atae");
    strcpy(temp[3].namel->ptr, "M9wda3lih");
    temp[3].region.code = 100;
    temp[3].region.rate = 10;

    // End of example values

    int numEmployees = sizeof(temp) / sizeof(temp[0]), numRows = 15,
        startRow = 0;

    Font defaultFont = GetFontDefault();
    defaultFont.baseSize = 20;
    GuiSetFont(defaultFont);

    Font customFont = LoadFontEx("Hack-Regular.ttf", 40, NULL, 0);
    GuiSetFont(customFont);

    // create a scrollbar
    Rectangle scrollbar = {900, 50, 20, numRows * 20};
    bool scrollbarClicked = false;

    char inputBuffer[256] = {0};
    Rectangle inputBox = {screenWidth / 2 - 200, screenHeight - 70, 400, 40};

    while (!exitWindow && !WindowShouldClose()) {

      mousePosition = GetMousePosition();

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) {
        if (CheckCollisionPointRec(mousePosition,
                                   (Rectangle){0, 0, screenWidth, 20})) {
          dragWindow = true;
          panOffset = mousePosition;
        }
      }

      if (dragWindow) {
        windowPosition.x += (mousePosition.x - panOffset.x);
        windowPosition.y += (mousePosition.y - panOffset.y);

        SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
          dragWindow = false;
      }

      BeginDrawing();

      ClearBackground(RAYWHITE);

      exitWindow = GuiWindowBox((Rectangle){0, 0, screenWidth, screenHeight},
                                "#198# PORTABLE WINDOW");

      DrawText(TextFormat("Mouse Position: [ %.0f, %.0f ]", mousePosition.x,
                          mousePosition.y),
               10, 40, 10, DARKGRAY);

      // draw scrollbar and handle input
      if (CheckCollisionPointRec(GetMousePosition(), scrollbar) ||
          scrollbarClicked) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
          scrollbarClicked = true;
          startRow =
              (GetMouseY() - scrollbar.y) * numEmployees / (numRows * 20);
        } else {
          scrollbarClicked = false;
        }
        DrawRectangleRec(scrollbar, DARKGRAY);
      } else {
        DrawRectangleRec(scrollbar, LIGHTGRAY);
      }

      // display EMPLOYEE data
      DrawEmployeeData(temp, numEmployees, startRow, numRows);
      // display text input box
      GuiTextBox(inputBox, inputBuffer, sizeof(inputBuffer) - 1, true);
      EndDrawing();
    }

    if (tflag)
      shell_loop(path_to_disk_state, path_to_log_file);

    // NOTE: more cleanup can always be done
    free(path_to_disk_state);
    free(path_to_log_file);

    return EXIT_SUCCESS;
  }
}

static inline void usage(void) {
  fprintf(stderr,
          "%s - Manage employees\n"
          "usage: %s [-p <path>] [-l <path>][-c] [-v] [-h]\n"
          "\t-v, --verbose        enable verbose logging\n"
          "\t-q, --quiet          disable verbose logging\n"
          "\t-p, --path PATH      specify path of the state db\n"
          "\t-l, --log-file PATH  specify path of the log file\n"
          "\t-s, --stateless      disable the use of databases\n"
          "\t-t, --text-mode      replace the gui with a repl\n"
          "\t-h, --help           prints out this help\n",
          __progname, __progname);
}
