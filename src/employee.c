#include "all.h"

extern EMPLOYEE *head;

/// allocates an employee object
EMPLOYEE *create_employee(void) {
  EMPLOYEE *local = NULL;
  local = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
  check_alloc(local);
  local->namef = (char *)malloc(sizeof(char) * 25);
  check_alloc(local->namef);
  local->namel = (char *)malloc(sizeof(char) * 50);
  check_alloc(local->namel);
  local->next = NULL;
  return local;
}

/// frees employee fields
void destruct_employee(EMPLOYEE *deleted) {
  free(deleted->namel);
  free(deleted->namef);
  free(deleted);
}

/// prints all employee fields
void show_employee(EMPLOYEE *node) {
  printf("First Name:           %s\n", node->namef);
  printf("Last Name:            %s\n", node->namel);
  printf("Identifier:           %d\n", node->mat);
  printf("Salary:               %d\n", node->salary);
  printf("Region code:          %d\n", node->region.code_region);
  printf("Rate:                 %d\n", node->region.taux);
}
