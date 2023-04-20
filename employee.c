#include "all.h"

extern EMPLOYEE *head;

/* allocates an employee object */
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

/* frees employee fields */
void destruct_employee(EMPLOYEE *yeet) {
  free(yeet->namel);
  free(yeet->namef);
  free(yeet);
}

/* prints all employee fields */
void show_employee(EMPLOYEE *node) {
  printf("Nom:           %s\n", node->namef);
  printf("Prenom:        %s\n", node->namel);
  printf("Matricule:     %d\n", node->mat);
  printf("Salaire brute: %d\n", node->salary);
  printf("Code region:   %d\n", node->region.code_region);
  printf("Taux:          %d\n", node->region.taux);
}
