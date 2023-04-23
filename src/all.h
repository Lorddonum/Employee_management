#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definitions

#ifdef __linux__
#include <linux/limits.h>
#elif defined(_WIN32)
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <sys/syslimits.h>
#endif
#define CTRL_U 21
#define CTRL_D 4

// type declarations

typedef struct EMPLOYEE EMPLOYEE;

struct EMPLOYEE {
  int mat;
  int salary;
  char *namef;
  char *namel;
  struct {
    int code_region;
    int taux;
  } region;
  EMPLOYEE *next;
};

// NOTE: utility functions see "utils.c"
void check_alloc(void *ptr);
int check_string(char *buffer);

// NOTE: graphical views / routines see "graphx.c"
void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);
char *intprtkey(int ch);

// NOTE: employee base functions see "employee.c"
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void show_employee(EMPLOYEE *node);

// NOTE: disk <=> memory interchange see "disk.c"
int initlist(void);
