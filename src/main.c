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
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* ifndef _GNU_SOURCE */
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

// #include "gui.c"

extern int errno;

extern char *__progname;

static inline void usage(void);

extern char *optarg;
extern int optind, opterr, optopt;

bool aflag, vflag, qflag, cflag, sflag, tflag, lflag;

hash_table *global_table = nullptr;
size_t table_len = 0;

void DrawEmployeeData(int numEmployees, int startRow, int numRows) {
  int rowHeight = 20, y = 50, i;
  DrawText("Register", 50, y, 20, BLACK);
  DrawText("Salary", 150, y, 20, BLACK);
  DrawText("FName", 300, y, 20, BLACK);
  DrawText("LName", 500, y, 20, BLACK);
  DrawText("Code", 650, y, 20, BLACK);
  DrawText("Rate", 800, y, 20, BLACK);
  DrawLine(48, y + 1.5 * rowHeight, 850, y + 1.5 * rowHeight, BLACK);
  y += 2 * rowHeight;
  for (i = startRow; i < startRow + numRows && i < numEmployees; i++) {
    DrawText(TextFormat("%d", global_table[i].val->mat), 50, y, 20, BLACK);
    DrawText(TextFormat("%d", global_table[i].val->salary), 150, y, 20, BLACK);
    DrawText(global_table[i].val->namef->ptr, 300, y, 20, BLACK);
    DrawText(global_table[i].val->namel->ptr, 500, y, 20, BLACK);
    DrawText(TextFormat("%d", global_table[i].val->region.code), 650, y, 20,
             BLACK);
    DrawText(TextFormat("%d", global_table[i].val->region.rate), 800, y, 20,
             BLACK);
    y += rowHeight;
  }
}

void DrawInputBox(Rectangle box, char *buffer, int bufferSize, bool active) {
  static float backspaceHoldingTime = 0.0f;
  const float initialBackspaceDelay = 0.5f;
  const float backspaceRepeatDelay = 0.1f;

  int visibleTextStart = 0;
  int textWidth;

  int maxVisibleCharacters;

  DrawRectangleRec(box, LIGHTGRAY);
  DrawRectangleLinesEx(box, 2, BLACK);

  if (active) {
    int key = GetCharPressed();

    /// Check if more characters have been pressed on the same frame
    while (key > 0) {
      /// Only allow keys in range [32..125]
      if ((key >= 32) && (key <= 125) && (strlen(buffer) < bufferSize - 1)) {
        int len = strlen(buffer);
        buffer[len] = (char)key;
        buffer[len + 1] = '\0';
      }
      key = GetCharPressed(); /// Check next character in the queue
    }

    if (IsKeyDown(KEY_BACKSPACE)) {
      backspaceHoldingTime += GetFrameTime();
    } else {
      backspaceHoldingTime = 0.0f;
    }

    if (backspaceHoldingTime > initialBackspaceDelay) {
      if (((int)(backspaceHoldingTime * 10) %
           (int)(backspaceRepeatDelay * 10)) == 0) {
        int len = strlen(buffer);
        if (len > 0) {
          buffer[len - 1] = '\0';
        }
      }
    }
  }

  textWidth = MeasureText(buffer, 20);
  while (textWidth > box.width - 10) {
    visibleTextStart++;
    textWidth = MeasureText(buffer + visibleTextStart, 20);
  }

  maxVisibleCharacters = (box.width - 10) / MeasureText("A", 20);
  char visibleBuffer[maxVisibleCharacters + 1];
  strncpy(visibleBuffer, buffer + visibleTextStart, maxVisibleCharacters);
  visibleBuffer[maxVisibleCharacters] = '\0';

  DrawText(visibleBuffer, box.x + 5, box.y + (box.height / 4), 20, BLACK);

  if (visibleTextStart > 0) {
    DrawRectangleGradientH(box.x + 2, box.y + 2, 14, box.height - 4,
                           Fade(LIGHTGRAY, 0.8f), LIGHTGRAY);
  }
}

void SearchEmployee(int numEmployees, int startRow, int numRows, Rectangle box,
                    char *buffer, int bufferSize, bool active) {
  // Draw scrollbar
  Rectangle scrollbar = {900.0, 50.0, 20.0, (float)(numRows * 20)};
  bool scrollbarClicked = false;

  // draw scrollbar and handle input
  if (CheckCollisionPointRec(GetMousePosition(), scrollbar) ||
      scrollbarClicked) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      scrollbarClicked = true;
      startRow = (GetMouseY() - scrollbar.y) * numEmployees / (numRows * 20);
    } else {
      scrollbarClicked = false;
    }
    DrawRectangleRec(scrollbar, DARKGRAY);
  } else {
    DrawRectangleRec(scrollbar, LIGHTGRAY);
  }

  DrawEmployeeData(numEmployees, startRow, numRows);
  DrawInputBox(box, buffer, bufferSize, active);
}

void AddEmployee(void) { panic("no add employee implementation!!!\n"); }

void RemoveEmployee(void) { panic("no remove employee implementation!!!\n"); }

int DrawMenu(int numEmployees, int startRow, int numRows, Rectangle box,
             char *buffer, int bufferSize, bool active) {
  // Define positions, sizes, and colors
  Vector2 titlePosition = {GetScreenWidth() / 2, 50};
  int titleFontSize = 78;
  Color titleColor = DARKGRAY;
  Color shadowColor = BLACK;

  Vector2 lineStart = {0, titlePosition.y + titleFontSize + 10};
  Vector2 lineEnd = {GetScreenWidth(), lineStart.y};
  Color lineColor = DARKGRAY;

  Rectangle buttons[3] = {
      {GetScreenWidth() / 2 - 150, lineEnd.y + 100, 300, 70},
      {GetScreenWidth() / 2 - 150, lineEnd.y + 200, 300, 70},
      {GetScreenWidth() / 2 - 150, lineEnd.y + 300, 300, 70}};
  Color buttonColor = LIGHTGRAY;
  const char *buttonText[] = {"Search for employee", "Add employee",
                              "Remove employee"};
  int buttonTextFontSize = 20;

  // Draw title with shadow
  DrawTextEx(GetFontDefault(), "Emp Manager",
             (Vector2){titlePosition.x + 2, titlePosition.y + 2}, titleFontSize,
             1, shadowColor);
  DrawTextEx(GetFontDefault(), "Emp Manager", titlePosition, titleFontSize, 1,
             titleColor);

  // Draw line under title
  DrawLineEx(lineStart, lineEnd, 4, lineColor);

  // Draw rectangles with text inside
  for (int i = 0; i < 3; i++) {
    DrawRectangleRec(buttons[i], buttonColor);
    Vector2 textSize =
        MeasureTextEx(GetFontDefault(), buttonText[i], buttonTextFontSize, 1);
    Vector2 textPosition = {buttons[i].x + (buttons[i].width - textSize.x) / 2,
                            buttons[i].y +
                                (buttons[i].height - textSize.y) / 2};
    DrawTextEx(GetFontDefault(), buttonText[i], textPosition,
               buttonTextFontSize, 1, RAYWHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      // Check if the mouse position is inside the rectangle
      if (CheckCollisionPointRec(GetMousePosition(), buttons[i])) {
        return i;
      }
    }
  }
  unreachable();
}

int main(int argc, char *argv[]) {
  /* cli */
  vflag = false, qflag = false; /* default off => a quieter program    */
  sflag = true;                 /* default on => sync state with disk  */
  lflag = false;                /* default off => no redirect of stderr */
  tflag = false;                /* default off => gui on */

  char *path_to_disk_state = (char *)malloc(sizeof(char) * PATH_MAX);
  check_alloc(path_to_disk_state);
  char *path_to_log_file = (char *)malloc(sizeof(char) * PATH_MAX);
  check_alloc(path_to_log_file);

#if !defined(__GNUC__)
  int opt;
  while ((opt = getopt(argc, argv, "hcqstvl:p:")) != -1) {
    switch (opt) {
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
        {"log-file", required_argument, 0, 'l'},
        {"text-mode", no_argument, 0, 't'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0},
    };

    int option_index = 0;
    opt = getopt_long(argc, argv, "chqsvtl:p:", long_options, &option_index);
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
    strcpy(path_to_disk_state, "state/record.db");

  // redirect stderr to supplied path
  if (lflag) {
    FILE *fp = freopen(path_to_log_file, "a+", stderr);
    if (fp == nullptr) {
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
    // Initilization
    //----------------------------------------------------------------------------
    load_all(path_to_disk_state);

    const size_t Width = 1024;
    const size_t Height = 650;

    InitWindow(Width, Height, "Employee Data");
    SetTargetFPS(60);

    EMPLOYEE temp[4];

    size_t numEmployees = sizeof(temp) / sizeof(temp[0]);
    size_t startRow = 0;
    size_t numRows = 15;

    // create an inputbox && input buffer
    char inputBuffer[256] = {0};
    Rectangle inputBox = {(float)Width / 2.0 - 200.0, (float)Height - 70.0,
                          (float)400.0, (float)40.0};

    //------------------------------------------------------------------
    //                            main loop
    //------------------------------------------------------------------

    while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      SearchEmployee(numEmployees, startRow, numRows, inputBox, inputBuffer,
                     sizeof(inputBuffer) - 1, true);
      for (;;) {
        switch (DrawMenu(numEmployees, startRow, numRows, inputBox, inputBuffer,
                         sizeof(inputBuffer) - 1, true)) {
        case fuzz:
          break;
        case exact:
          break;
        case lev:
          break;
        }
      }
    }
    EndDrawing();

    CloseWindow();
  }

  if (tflag)
    shell_loop(path_to_disk_state);

  // NOTE: more cleanup can always be done
  free(path_to_disk_state);
  free(path_to_log_file);

  return EXIT_SUCCESS;
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
