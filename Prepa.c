#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

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

//Allocating a global pointer serving as the head of the list
EMPLOYE* HEAD;

int checkString(char *buffer){
    int n=strlen(buffer);
    for(;n!=0;n--){
        if(!(buffer[n]<='Z' && buffer[n]>='A'))
            return 0;
    }
    return 1;
}

void remplissage(EMPLOYE *A){
    char buffer[100];
    system("clear");

    while(1){
        printf("\t\tNom: ");
        fgets(buffer,100,stdin);
        if(checkString(buffer)==1){
            strcpy(buffer,A->nom);
            break;
        } 
    }  

    while(1){
        printf("\t\tPrenom: ");
        fgets(buffer,100,stdin);
        if(checkString(buffer)==1){
            strcpy(buffer,A->prenom);
            break;
        }
    }

    printf("\t\tMatricule: ");
    while(scanf("%d",A->mat)!=1){
        printf("\t\tMatricule:");
    }

    printf("\t\tSalaire brute: ");
    while(scanf("%d",A->sal_Brute)!=1){
        printf("\t\tSalaire brute: ");
    }

    printf("\t\tCode Region: ");
    while(scanf("%d",A->region.code_region)!=1){
        printf("\t\tCode Region: ");
    }

    printf("\t\tTaux: ");
    while(scanf("%d",A->region.taux)!=1){
        printf("\t\tTaux: ");
    }
}

void ajout_emp(EMPLOYE *head){
    //Cycling until the last node
    while(head->next!=NULL){
        head=head->next;
    }

    //Allocating and getting shit ready
    EMPLOYE *node=NULL;
    node=(EMPLOYE*)malloc(sizeof(EMPLOYE));
    node->next=NULL;

    //Self explanatory hhhhhhhhh
    remplissage(node);

    //affecting node at the end of the list
    head->next=node;
    node->next=NULL;
}

void modification(EMPLOYE *head, int n){
    int choice;
    char *buffer;
    //cycling through the list to get to what we want to modify
    for(;n!=0;n--){
        head=head->next;
    }
    
    //Instead of a long while loop, I opted for a goto when the modifications are all done :) it just looks better in my opinion but
    //idk optimization wise I still gotta check later
    START:
    printf("Choisissez quels valeurs voulez-vous changer:\n");
    printf("1 - Nom: %s", head->nom);
    printf("2 - Prenom: %s", head->prenom);
    printf("3 - Matricule: %s", head->mat);
    printf("4 - Salaire Brute: %s", head->sal_Brute);
    printf("5 - Code Region: %s", head->region.code_region);
    printf("6 - Taux: %s", head->region.taux);
    printf("0 - Revenir au menu");

    printf("\n\nChoix: ");
    while(scanf("%d", &choice)!=1 || choice>6 || choice<0){
        printf("Wromg imputt uwu\nChoix:");
    }

    switch(choice){
        case 1: 
            buffer=(char*)malloc(100*sizeof(char));
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
            while(scanf("%d",head->mat)!=1){
                printf("\t\tMatricule:");
            }
            goto MESSAGE_SUCCESS;

        case 4:
            printf("\t\tMatricule: ");
            while(scanf("%d",head->sal_Brute)!=1){
                printf("\t\tMatricule:");
            }
            goto MESSAGE_SUCCESS;

        case 5:
            printf("\t\tCode Region: ");
            while(scanf("%d",head->region.code_region)!=1){
                printf("\t\tCode Region: ");
            }
            goto MESSAGE_SUCCESS;
        
        case 6:
            printf("\t\tTaux: ");
            while(scanf("%d",head->region.taux)!=1){
                printf("\t\tTaux: ");
            }

        case 0:
            return;

        MESSAGE_SUCCESS:
            printf("Modification successful. Going back to menu in 3 seconds");
            sleep(3);
            break;
        default:
            exit(2);
    }
    system("clear");
    goto START; 
}   

void suppression(EMPLOYE *head, int n){
    for(;n!=1;n--){
        head=head->next;
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
}

void affichage(EMPLOYE *node){
    printf("Nom:           %s",&node->nom);
    printf("Prenom:        %s",&node->prenom);
    printf("Matricule:     %d",&node->mat);
    printf("Salaire brute: %d",&node->sal_Brute);
    printf("Code region:   %d",&node->region.code_region);
    printf("Taux:          %d",&node->region.taux);
}

void recherche(EMPLOYE *head){
    int choice;
    RECH:
    puts("Chercher par:"
         "\t1-Matricule"
         "\t2-Nom"
         "\t0-Annuler");
    printf("Choix: ");
    while(scanf("%d",&choice)!=1){
        printf("Wromg imput uwu.\nChoix: ");
    }
    switch(choice){
        case 1:
            int *Mat=malloc(sizeof(int));
            if(Mat==NULL){
                fprintf(stderr,"Error value: %d\n", errno);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(2);
            }
            printf("Matricule: ");
            while(scanf("%d", &mat)!=1);{
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
                goto RECH;
            }
            else{
                printf("Aucun employe trouve avec la matricule entree.\nTapez entrer pour continuer");
                getchar();
                system("clear");
                goto RECH;
            }
        case 2:
            char *Nom=malloc(sizeof(char));
            if(Nom==NULL){
                fprintf(stderr,"Error value: %d\n", errno);
                fprintf(stderr, "%s\n", strerror(errno));
                exit(2);
            }
            printf("Nom: ");
            while(fgets(head->nom, head->nom, stdin)!='s';{
                printf("Nom: ");
            }
            while(!(strcmp(head->nom,*Nom))){
                head=head->next;
            }
            if(!(head==NULL)){
                affichage(head);
                printf("Tapez entrer pour continuer");
                getchar();
                system("clear");
                goto RECH;
            }
            else{
                printf("Aucun employe trouve avec la matricule entree.\nTapez entrer pour continuer");
                getchar();
                system("clear");
                goto RECH;
            }
            
    }
}

int main(void){
    return 0;
}