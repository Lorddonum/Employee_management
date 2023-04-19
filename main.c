#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <errno.h>
#include <stddef.h>

#include <curses.h>
#include <sys/ioctl.h>

#include <locale.h>
#include <unistd.h>

#include "all.h"

extern int errno;

EMPLOYEE *head = NULL;

void fill_employee(EMPLOYEE *A) {
  char *buffer = malloc(sizeof(char) * 100);
  check_alloc(buffer);
  system("clear");

  while (true) {
    printf("\n\tNom: ");
    scanf("%s", buffer);
    if (check_string(buffer) == 1) {
      A->namef = (char *)malloc(100 * sizeof(char));
      strcpy(A->namef, buffer);
      break;
    }
  }

  while (1) {
    printf("\n\tPrenom: ");
    scanf("%s", buffer);
    if (check_string(buffer) == 1) {
      A->namel = (char *)malloc(100 * sizeof(char));
      strcpy(A->namel, buffer);
      break;
    }
  }

  printf("\n\tMatricule:\n");
  scanf("%d", &A->mat);

  printf("\n\tSalaire brute:\n");
  scanf("%d", &A->sal_Brute);

  printf("\n\tCode Region:\n");
  scanf("%d", &A->region.code_region);

  printf("\n\tTaux:\n");
  scanf("%d", &A->region.taux);
}

void add_employee_end(void) {
  EMPLOYEE *cur = head;

  if (cur == NULL) {
    head = create_employee();
    fill_employee(head);
    return;
  }

  while (cur->next != NULL) {
    cur = cur->next;
  }

  cur->next = create_employee();
  fill_employee(cur->next);
  return;
}

void modification(int index) {
  int choice = 0;
  char *buffer = NULL;
  buffer = (char *)malloc(100 * sizeof(char));
  check_alloc(buffer);
  // cycling through the list to get to what we want to modify
  for (; index != 0; index--) {
    head = head->next;
  }

// Instead of a long while loop, I opted for a goto when the modifications are
// all done :) it just looks better in my opinion but idk optimization wise I
// still gotta check later but it's probably the same
START:
  printf("Choisissez quelles valeurs voulez-vous changer:\n");
  printf("1 - Nom: %s\n", head->namef);
  printf("2 - Prenom: %s\n", head->namel);
  printf("3 - Matricule: %d\n", head->mat);
  printf("4 - Salaire Brute: %d\n", head->sal_Brute);
  printf("5 - Code Region: %d\n", head->region.code_region);
  printf("6 - Taux: %d\n", head->region.taux);
  printf("0 - Revenir au menu\n");

  printf("\n\nChoix: ");
  while (scanf("%d", &choice) != 1 || choice > 6 || choice < 0) {
    fprintf(stderr, "Wrong input\n");
    goto START;
  }

  switch (choice) {
  case 1:
    while (1) {
      printf("\tNom: ");
      fgets(buffer, 100, stdin);
      if (check_string(buffer) == 1) {
        strcpy(buffer, head->namef);
        free(buffer);
        break;
      }
    }
    goto MESSAGE_SUCCESS;

  case 2:
    while (1) {
      printf("\tPrenom: ");
      fgets(buffer, 100, stdin);
      if (check_string(buffer) == 1) {
        strcpy(buffer, head->namel);
        free(buffer);
        break;
      }
    }
    goto MESSAGE_SUCCESS;

  case 3:
    printf("\t\tMatricule: ");
    scanf("%d", &head->mat);
    goto MESSAGE_SUCCESS;

  case 4:
    printf("\t\tMatricule: ");
    scanf("%d", &head->sal_Brute);
    goto MESSAGE_SUCCESS;

  case 5:
    printf("\t\tCode Region: ");
    scanf("%d", &head->region.code_region);
    goto MESSAGE_SUCCESS;

  case 6:
    printf("\t\tTaux: ");
    scanf("%d", &head->region.taux);

  case 0:
    printf("Press enter to go back to menu. ");
    getchar();
    return;

  MESSAGE_SUCCESS:
    printf("Modification successful. Press enter to go back to menu. ");
    getchar();
    break;
  default:
    exit(2);
  }
  system("clear");
  goto START;
}

void suppression(EMPLOYEE *head, int n) {
  char *confirm = (char *)malloc(sizeof(char));

  for (; n != 1; n--) {
    head = head->next;
  }

  // checking if user really wants to delete this one
  show_employee(head);
  printf("Confirm deletion:(y/n) ");
  while (!scanf("%[ynYN]", confirm)) {
    printf("Confirm deletion:(y/n) ");
  }
  if (*confirm == 'n' || *confirm == 'N') {
    printf("Deletion cancelled. Press enter to return to menu");
    getchar();
    free(confirm);
    return;
  }
  // if there is a node after the one to delete
  if (!(head->next->next == NULL)) {
    head->next = head->next->next;
    free(head->next);
  } else {
    free(head->next);
    head->next = NULL;
  }
  printf("Deletion complete. Press enter to return to menu");
  getchar();
}

void recherche(void) {
  int choice;
RECH:
  system("clear");
  puts("Chercher par:"
       "\t1-Matricule"
       "\t2-Nom"
       "\t0-Annuler");
  printf("Choix: ");
  scanf("%d", &choice);

  // Declaring pointers
  int *Mat = malloc(sizeof(int));
  check_alloc(Mat);
  char *Nom = malloc(sizeof(char));
  check_alloc(Nom);

  // Processing
  switch (choice) {
  case 1:
    printf("Matricule: ");
    while (scanf("%d", Mat) != 1) {
      printf("Matricule: ");
    }
    while (!(head->mat == *Mat)) {
      head = head->next;
    }
    if (!(head == NULL)) {
      show_employee(head);
      printf("Tapez entrer pour continuer");
      getchar();
      system("clear");
      goto FREE;
    } else {
      printf("Aucun employe trouve avec la matricule entree.\n"
             "Tapez entrer pour continuer");
      getchar();
      system("clear");
      goto FREE;
    }
  case 2:
    if (Nom == NULL) {
      fprintf(stderr, "Error value: %d\n", errno);
      fprintf(stderr, "%s\n", strerror(errno));
      exit(2);
    }
    printf("Nom: ");
    while (fgets(head->namef, sizeof(head->namef), stdin) == NULL) {
      printf("Nom: ");
    }
    while (!(strcmp(head->namef, Nom))) {
      head = head->next;
    }
    if (!(head == NULL)) {
      show_employee(head);
      printf("Tapez entrer pour continuer");
      getchar();
      goto FREE;
    } else {
      puts("Aucun employe trouve avec le nom entre.\n"
           "Tapez entrer pour continuer\n");
      getchar();
      goto FREE;
    }
  case 0:
    printf("Press enter to go back to menu. ");
    getchar();
    return;
  }
FREE:
  free(Mat);
  free(Nom);
  goto RECH;
}

int main(int argc, char *argv[]) {

  setlocale(LC_ALL, "");

  EMPLOYEE *head = NULL;
  head = (EMPLOYEE *)malloc(sizeof(EMPLOYEE));
  head->next = NULL;
  int choice, i;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  while (1) {
    system("clear");
    for (i = w.ws_col; i != 0; i--) {
      printf("=");
    }
    printf("\n\t\t\t\tWELCOME TO DICTATONARIA\n");
    for (i = w.ws_col; i != 0; i--) {
      printf("=");
    }

    puts("\n\nChoose one of the options below:\n\n"
         "1 - Adding employee\n"
         "2 - ((Murdering)Firing) Delete employee\n"
         "3 - Modify employee(Brain wash)\n"
         "4 - Search employee\n"
         "5 - Quit\n");
    printf("Choice: ");
    while (!scanf("%d", &choice) || choice > 5 || choice < 1) {
      printf("Choice: ");
    }

    int employeeNumber = -1;
    switch (choice) {
    case 1:
      add_employee_end();
      break;
    case 2:
      printf("Number of employee: ");
      scanf("%d", &employeeNumber);
      suppression(head, employeeNumber);
      break;
    case 3:
      printf("Number of employee: ");
      while (!scanf("%d", &employeeNumber)) {
        printf("Number of employee: ");
      }
      modification(employeeNumber);
      break;
    case 4:
      recherche();
      break;
    case 5:
      return 0;
    default:
      exit(2);
    }
  }
  return 0;
}
