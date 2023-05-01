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

#define FIRST_NAME_LIM 25
#define LAST_NAME_LIM 50

// macros

// type declarations

/// general purpose string type
typedef struct {
  char *ptr;
  size_t len;
  size_t lim;
} string;

typedef struct EMPLOYEE EMPLOYEE;

/// Employee linked list node type
struct EMPLOYEE {
  int mat;
  int salary;
  string *namef;
  string *namel;
  struct {
    int code;
    int rate;
  } region;
  EMPLOYEE *next;
};

/* NOTE: general plumbing functions see "utils.c" */
void check_alloc(void *ptr);
int check_string(char *buffer);
void panic(const char *msg);
string *allocate_string(size_t size);
void destruct_string(string *str);

/* NOTE: employee base functions see "employee.c" */
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void show_employee(EMPLOYEE *node);

/* NOTE: disk <=> memory interchange see "disk.c" */
void initlist(const char *path_to_disk_state);

/* NOTE: search functions see "search.c" */
size_t lev(string *a, string *b);

// vim: ft=c
