#include <stdio.h>
#include <stdlib.h>

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

// utility functions 
// see "utils.c"
void check_alloc(void *ptr);
int check_string(char *buffer);

// employee specifc functions 
// see "employee.c"
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void show_employee(EMPLOYEE *node);

// disk <=> memory interchange see "disk.c"

