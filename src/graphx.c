#include <ctype.h>
#include <curses.h>
#include <form.h>
#include <menu.h>

#include "all.h"

/// Struct to hold keycode/keyname information
struct keydesc {
  int code;
  char name[20];
};

/// Returns a string describing a character passed to it
char *intprtkey(int ch) {

  ///  Define a selection of keys we will handle
  static struct keydesc keys[] = {{KEY_UP, "Up arrow"},
                                  {KEY_DOWN, "Down arrow"},
                                  {KEY_LEFT, "Left arrow"},
                                  {KEY_RIGHT, "Right arrow"},
                                  {KEY_HOME, "Home"},
                                  {KEY_END, "End"},
                                  {KEY_BACKSPACE, "Backspace"},
                                  {KEY_IC, "Insert"},
                                  {KEY_DC, "Delete"},
                                  {KEY_NPAGE, "Page down"},
                                  {KEY_PPAGE, "Page up"},
                                  {CTRL_D, "Control d"},
                                  {CTRL_U, "Control u"},
                                  {KEY_F(1), "Function key 1"},
                                  {KEY_F(2), "Function key 2"},
                                  {KEY_F(3), "Function key 3"},
                                  {KEY_F(4), "Function key 4"},
                                  {KEY_F(5), "Function key 5"},
                                  {KEY_F(6), "Function key 6"},
                                  {KEY_F(7), "Function key 7"},
                                  {KEY_F(8), "Function key 8"},
                                  {KEY_F(9), "Function key 9"},
                                  {KEY_F(10), "Function key 10"},
                                  {KEY_F(11), "Function key 11"},
                                  {KEY_F(12), "Function key 12"},
                                  {-1, "<unsupported>"}};
  static char keych[2] = {0};

  if (isprint(ch) && !(ch & KEY_CODE_YES)) {

    /*  If a printable character  */

    keych[0] = ch;
    return keych;

  } else {

    /*  Non-printable, so loop through our array of structs  */

    int n = 0;

    do {
      if (keys[n].code == ch)
        return keys[n].name;
      n++;
    } while (keys[n].code != -1);

    return keys[n].name;
  }
}

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 80;

  length = strlen(string);
  temp = (int)((width - length) / 2);
  x = startx + temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}

void win_show(WINDOW *win, char *label, int label_color) {
  int startx, starty, height, width;

  getbegyx(win, starty, startx);
  getmaxyx(win, height, width);

  box(win, 0, 0);
  mvwaddch(win, 2, 0, ACS_LTEE);
  mvwhline(win, 2, 1, ACS_HLINE, width - 2);
  mvwaddch(win, 2, width - 1, ACS_RTEE);

  print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

char *intprtkey(int ch);
