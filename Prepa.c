#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

extern int errno;

typedef struct reg{
    int code_region, taux;
}REGION;

typedef struct emp{
    int mat,sal_Brute;
    char *nom, *prenom;
    REGION region;
    struct emp *next;
}EMPLOYE;

int checkString(char *buffer){
    int n=strlen(buffer);
    for(;n!=0;n--){
        if((buffer[n]<='Z' && buffer[n]>='A')||(buffer[n]<='z' && buffer[n]>='a'))
            return 1;
    }
    return 0;
}

void remplissage(EMPLOYE *A){
    char buffer[100];
    system("clear");

    while(1){
        printf("\n\tNom: ");
        scanf("%s",buffer);
        if(checkString(buffer)==1){
            A->nom=(char*)malloc(100*sizeof(char));
            strcpy(A->nom,buffer);
            break;
        } 
    }  

    while(1){
        printf("\n\tPrenom: ");
        scanf("%s",buffer);
        if(checkString(buffer)==1){
            A->prenom=(char*)malloc(100*sizeof(char));
            strcpy(A->prenom,buffer);
            break;
        }
    }

    printf("\n\tMatricule: ");
    while(scanf("%d",&A->mat)!=1){
        printf("\n\tMatricule:");
    }

    printf("\n\tSalaire brute: ");
    while(scanf("%d",&A->sal_Brute)!=1){
        printf("\n\tSalaire brute: ");
    }

    printf("\n\tCode Region: ");
    while(scanf("%d",&A->region.code_region)!=1){
        printf("\n\tCode Region: ");
    }

    printf("\n\tTaux: ");
    while(scanf("%d",&A->region.taux)!=1){
        printf("\n\tTaux: ");
    }
}

void ajout_emp(EMPLOYE *head){
    int n;
    //Cycling until the last node with a counter to know if it's the first or else
    while(head->next!=NULL){
        n++;
        head=head->next;
    }
    if(n!=0){
        head->next=(EMPLOYE*)malloc(sizeof(EMPLOYE));

        if(head->next==NULL){
            fprintf(stderr,"Error value: %d\n", errno);
            fprintf(stderr, "%s\n", strerror(errno));
            exit(2);
        }
        remplissage(head->next);
        head->next->next=NULL;
        return;
    } 
    remplissage(head);
    head->next=NULL;
}

void affichage(EMPLOYE *node){
    printf("Nom:           %s\n",node->nom);
    printf("Prenom:        %s\n",node->prenom);
    printf("Matricule:     %d\n",node->mat);
    printf("Salaire brute: %d\n",node->sal_Brute);
    printf("Code region:   %d\n",node->region.code_region);
    printf("Taux:          %d\n",node->region.taux);
}

void modification(EMPLOYE *head, int n){
    int choice;
    char *buffer;
    //cycling through the list to get to what we want to modify
    for(;n!=0;n--){
        head=head->next;
    }
    
    //Instead of a long while loop, I opted for a goto when the modifications are all done :) it just looks better in my opinion but
    //idk optimization wise I still gotta check later but it's probably the same
    START:
    printf("Choisissez quelles valeurs voulez-vous changer:\n");
    printf("1 - Nom: %s\n", head->nom);
    printf("2 - Prenom: %s\n", head->prenom);
    printf("3 - Matricule: %d\n", head->mat);
    printf("4 - Salaire Brute: %d\n", head->sal_Brute);
    printf("5 - Code Region: %d\n", head->region.code_region);
    printf("6 - Taux: %d\n", head->region.taux);
    printf("0 - Revenir au menu\n");

    printf("\n\nChoix: ");
    while(scanf("%d", &choice)!=1 || choice>6 || choice<0){
        printf("Wromg imputt uwu\nChoix:");
    }

    switch(choice){
        case 1: 
            buffer=(char*)malloc(100*sizeof(char));
            if(buffer==NULL){
                fprintf(stderr,"Error value: %d\n", errno);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(2);
            }
            while(1){
                printf("\tNom: ");
                fgets(buffer,100,stdin);
                if(checkString(buffer)==1){
                    strcpy(buffer,head->nom);
                    free(buffer);
                    break;
                } 
            }
            goto MESSAGE_SUCCESS;
        
        case 2:
            buffer=(char*)malloc(100*sizeof(char));
            if(buffer==NULL){
                fprintf(stderr,"Error value: %d\n", errno);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(2);
            }
            while(1){
                printf("\tPrenom: ");
                fgets(buffer,100,stdin);
                if(checkString(buffer)==1){
                    strcpy(buffer,head->prenom);
                    free(buffer);
                    break;
                } 
            }
            goto MESSAGE_SUCCESS;

        case 3:
            printf("\t\tMatricule: ");
            while(scanf("%d",&head->mat)!=1){
                printf("\t\tMatricule:");
            }
            goto MESSAGE_SUCCESS;

        case 4:
            printf("\t\tMatricule: ");
            while(scanf("%d",&head->sal_Brute)!=1){
                printf("\t\tMatricule:");
            }
            goto MESSAGE_SUCCESS;

        case 5:
            printf("\t\tCode Region: ");
            while(scanf("%d",&head->region.code_region)!=1){
                printf("\t\tCode Region: ");
            }
            goto MESSAGE_SUCCESS;
        
        case 6:
            printf("\t\tTaux: ");
            while(scanf("%d",&head->region.taux)!=1){
                printf("\t\tTaux: ");
            }

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

void suppression(EMPLOYE *head, int n){
    char *confirm=(char*)malloc(sizeof(char));

    for(;n!=1;n--){
        head=head->next;
    }

    //checking if user really wants to delete this one
    affichage(head);
    printf("Confirm deletion:(y/n) ");
    while(!scanf("%[ynYN]",confirm)){
        printf("Confirm deletion:(y/n) ");
    }
    if(*confirm=='n' || *confirm=='N'){
        printf("Deletion cancelled. Press enter to return to menu");
        getchar();
        free(confirm);
        return;
    }
    //if there is a node after the one to delete
    if(!(head->next->next==NULL)){
        head->next=head->next->next;
        free(head->next);
    }
    else{
        free(head->next);
        head->next=NULL;
    }
    printf("Deletion complete. Press enter to return to menu");
    getchar();
}

void recherche(EMPLOYE *head){
    int choice;
    RECH:
    system("clear");
    puts("Chercher par:"
         "\t1-Matricule"
         "\t2-Nom"
         "\t0-Annuler");
    printf("Choix: ");
    while(scanf("%d",&choice)!=1){
        printf("Wromg imput uwu.\nChoix: ");
    }

    //Declaring pointers
    int *Mat=malloc(sizeof(int));
    char *Nom=malloc(sizeof(char));

    //Processing
    switch(choice){
        case 1:
            if(Mat==NULL){
                fprintf(stderr,"Error value: %d\n", errno);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(2);
            }
            printf("Matricule: ");
            while(scanf("%d", Mat)!=1){
                printf("Matricule: ");
            }
            while(!(head->mat==*Mat)){
                head=head->next;
            }
            if(!(head==NULL)){
                affichage(head);
                printf("Tapez entrer pour continuer");
                getchar();
                system("clear");
                goto FREE;
            }
            else{
                printf("Aucun employe trouve avec la matricule entree.\nTapez entrer pour continuer");
                getchar();
                system("clear");
                goto FREE;
            }
        case 2:
            if(Nom==NULL){
                fprintf(stderr,"Error value: %d\n", errno);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(2);
            }
            printf("Nom: ");
            while(fgets(head->nom, sizeof(head->nom), stdin)==NULL){
                printf("Nom: ");
            }
            while(!(strcmp(head->nom,Nom))){
                head=head->next;
            }
            if(!(head==NULL)){
                affichage(head);
                printf("Tapez entrer pour continuer");
                getchar();
                goto FREE;
            }
            else{
                printf("Aucun employe trouve avec le nom entre.\nTapez entrer pour continuer");
                getchar();
                goto FREE;
            }
        case 0:
            printf("Press enter to go back to menu. ");
            getchar();
            return;
    }
    FREE:
    free(Mat); free(Nom);
    goto RECH;
}

int main(void){
    EMPLOYE *head=NULL;
    head=(EMPLOYE*)malloc(sizeof(EMPLOYE));
    head->next=NULL;
    int choice, i;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    

    while(1){
        system("clear");
        for(i=w.ws_col;i!=0;i--){
            printf("=");
        }
        printf("\n\t\t\t\tWELCOME TO DICTATONARIA\n");
        for(i=w.ws_col;i!=0;i--){
            printf("=");
        }

        puts("\n\nChoose one of the options below:\n\n"
                "1 - Adding employee\n"
                "2 - ((Murdering)Firing) Delete employee\n"
                "3 - Modify employee(Brain wash)\n"
                "4 - Search employee\n"
                "5 - Quit\n");
        printf("Choice: ");
        while(!scanf("%d",&choice) || choice>5 || choice<1){
            printf("Choice: ");
        }

        int *employeeNumber=(int*)malloc(sizeof(int));
        switch(choice){
            case 1: ajout_emp(head);
                    break;
            case 2: printf("Number of employee: ");
                    while(!scanf("%d",employeeNumber)){
                        printf("Number of employee: ");
                    }
                    suppression(head, *employeeNumber);
                    free(employeeNumber);
                    break;
            case 3: printf("Number of employee: ");
                    while(!scanf("%d",employeeNumber)){
                        printf("Number of employee: ");
                    }
                    modification(head, *employeeNumber);
                    free(employeeNumber);
                    break;
            case 4: recherche(head);
                    break;
            case 5: return 0;
            default:exit(2);
        }
    }
    return 0;
}