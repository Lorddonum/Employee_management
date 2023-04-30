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

// constants
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

// macros

// type declarations

typedef struct EMPLOYEE EMPLOYEE;

/// Employee linked list node type
struct EMPLOYEE {
  int mat;
  int salary;
  char *namef;
  char *namel;
  struct {
    int code;
    int rate;
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

// NOTE: employee base functions see "employee.c"
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void show_employee(EMPLOYEE *node);

// NOTE: disk <=> memory interchange see "disk.c"
void initlist(const char *path_to_disk_state);

// vim: ft=c
