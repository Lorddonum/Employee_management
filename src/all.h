#include <curses.h>
#include <stddef.h>
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

// macros

// NOTE: only works on static arrays (no vla)
#define ARRAY_SIZE(a) (sizeof a / sizeof a[0])
// TODO: extract all the windows dimesniosn from ioctl's window size
#define GLOBAL_CORDS(a)

// type declarations

typedef struct EMPLOYEE EMPLOYEE;

/// Employee linked list node type
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

// global view options
typedef enum { welcome = 0, menu, search } view;

/// global terminal size in a struct
typedef struct {
  int col;
  int row;
  view state;
} term_state;

// NOTE: utility functions see "utils.c"
void check_alloc(void *ptr);
int check_string(char *buffer);
void panic(const char *msg);

// NOTE: graphical views / routines see "graphx.c"
void print_in_middle(WINDOW *win, char *string, chtype color);
char *intprtkey(int ch);

// NOTE: employee base functions see "employee.c"
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void show_employee(EMPLOYEE *node);

// NOTE: disk <=> memory interchange see "disk.c"
void initlist(void);

// vim: ft=c
