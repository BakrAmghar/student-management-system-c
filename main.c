#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct etudiant{
  char id[50];
  float moyenne;
  char nom[50];

  struct etudiant *next;
  struct etudiant *prev;
}etudiant;


void Pause(){
    printf("\nAppuyez sur ENTER pour continuer...");
    getchar();
    getchar();
}
//Var global :
etudiant *head = NULL;
etudiant *queue = NULL;

//fonction de Crreation:
etudiant* CreerEtudiant(char id[], float moy, char nom[]){
 etudiant *nv = (etudiant*)malloc(sizeof(etudiant));

 strcpy(nv->id,id);
 nv->moyenne = moy;
 strcpy(nv->nom, nom);

  nv->next = NULL;
  nv->prev = NULL;
  return(nv);
}

//fct insertion en tete :
void InsertionTete(char id[], float moy, char nom[]){
  etudiant *nv = CreerEtudiant(id,moy,nom);
  if(head == NULL){
    head = queue = nv;
  }else{
    head->prev = nv;
    nv->next = head;
    head = nv;
  }
}

//fct insetion en queue :
void InsertionQueue(char id[], float moy, char nom[]){
    etudiant *nv = CreerEtudiant(id,moy,nom);
    if(queue == NULL){queue = head = nv;
    }else{
      queue->next = nv;
      nv->prev = queue;
      queue = nv;
    }
}


//fct affichage
void Affichage(){
  etudiant *temp = head;

if(head != NULL){
    printf("\n\n");
    printf("=========================================\n");
    printf("           LISTE DES ETUDIANTS           \n");
    printf("=========================================\n");

  while(temp != NULL){
      printf("\n+---------------------------------------+\n");
        printf("| ID       : %-28s|\n", temp->id);
        printf("| Nom      : %-28s|\n", temp->nom);
        printf("| Moyenne  : %-28.2f|\n", temp->moyenne);
        printf("+---------------------------------------+\n");

    temp = temp->next;
  }
  printf("\n=========================================\n");
 }else{
   printf("\n=== LISTE VIDE ===");
 }
}

void CalculeTaille(){
  etudiant *temp = head;
  int i=0;

   while(temp !=NULL){
    i++;
    temp = temp->next;
   }

   printf("\n\n======TAILLE DE LA LISTE=======\n");
   printf("-- %d --",i);
   printf("\n=================================\n");
}


/*BAKR'S UPGRADESSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS*/
//fct to store data for later use, using fileeeeeeeeeees :

void SauvegarderListe(){
   FILE *fp = fopen("etudiants.txt","w");
   if(fp == NULL){printf("\n erreur lors de l'ouverture du fichier\n"); return;}

   etudiant *temp = head;
   while(temp != NULL){
    fprintf(fp,"%s %s %.2f\n",temp->id,temp->nom,temp->moyenne);
    temp = temp->next;
   }

   fclose(fp);
   printf("\nEtudiants sauvegarder en etudiant.txt\n");
}

//Loading up etud from the existant txt file :
void LoadEtudiants(){
  FILE *fp = fopen("etudiants.txt","r");
  char id[50];
  float moy;
  char nom[50];

  if(fp == NULL){printf("\n--Fichier non existant--"); return;}

  while(fscanf(fp,"%s %s %f\n",id,nom,&moy) == 3){
    InsertionQueue(id,moy,nom);
  }

  if(head == NULL) printf("\n===LISTE VIDE===");
  fclose(fp);
}


//fct de recherche par ID comme param, car l’utilisation de cette fct est necessaire dans plusieurs autres:
etudiant* RechercheID(){
    etudiant *temp = head;
    char id[50];

    printf("id :");
    scanf("%s",id);

    while(temp != NULL){
      if(strcmp(temp->id, id) == 0) return(temp);
      temp = temp->next;
   }
   printf("\n=== Non trouvee ===");
   return(NULL);
}

//Fct Affichage une seul chaine :
void AffichagePrecis(etudiant *nv){
    if(nv == NULL) return;
        printf("\n+---------------------------------------+\n");
        printf("| ID       : %-28s |\n", nv->id);
        printf("| Nom      : %-28s |\n", nv->nom);
        printf("| Moyenne  : %-28.2f |\n", nv->moyenne);
        printf("+---------------------------------------+\n");
}


//Fct pr changer la moyenne:
void ChangerMoy(){
  float nv_moy;
  etudiant *etu = RechercheID();

 if(etu == NULL) return;  //Verifie existance de l'etudiant:

  printf("Nouvelle moyenne :");
  scanf("%f",&nv_moy);

  etu->moyenne = nv_moy;
  printf("\nModification avec SUCCESS");
}

//fct pr changer le nom
void ChangerNom(){
  char nv_nom[50];
  etudiant *etu = RechercheID();

if(etu == NULL) return;

  printf("Nouveau nom :");
  scanf("%s",nv_nom);

  strcpy(etu->nom,nv_nom);
}


//SUPPRESSIOOOOOOON:
void Supprimer(){
  etudiant *etu = RechercheID();

if(etu == NULL) return;

    if(etu->prev == NULL && etu->next == NULL){ //cas un seul element
        free(etu);
        head = queue = NULL;
        return;}

    if(etu->prev == NULL){   //cas ou etudiant est le 1er ele aka HEAD
         head = etu->next;
         etu->next->prev = NULL;
         free(etu);
         return;}

    if(etu->next == NULL){   //cas ou etudiant est le dernier ele aka HEAD
         queue = etu->prev;
         etu->prev->next = NULL;
         free(etu);
         return;}

          etu->next->prev = etu->prev;
          etu->prev->next = etu->next;
          free(etu);
}

//chercher des anomalies
void VerificationDesAnomalies(){
  etudiant *temp1;
  etudiant *temp2;
  int anomalieCompteur = 0;

   if(head == NULL) { printf("\n--AUCUN ANOMALIE :) --"); return;}

   //detecteur de repetition:
    for(temp1 = head; temp1 != NULL; temp1 = temp1->next){
        for(temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next){
            if(strcmp(temp1->id, temp2->id) == 0){
                printf("!!! Anomalie : ID en double --> id = %s (etudiants %s et %s)\n",temp1->id, temp1->nom, temp2->nom);
                anomalieCompteur++;
            }

            if(strcmp(temp1->nom, temp2->nom) == 0){
                printf("!!! Anomalie : Nom en double --> nom = %s (etudiants %s et %s)\n",temp1->nom, temp1->id, temp2->id);
                anomalieCompteur++;
          }
        }
    }

  if(anomalieCompteur == 0){printf("\n=== Aucune anomalie detecteeee :D\n ====");
    }else{printf("\n===!!!ATTENTION!!=== nbr d'Anomalie detecter : %d =====\n", anomalieCompteur);}
}


//Trier par moyenne :
void TrierParMoyenne(){

   etudiant *temp1;
   etudiant *temp2;

   char id[50];
   char nom[100];
   float moy;

   for(temp1 = head; temp1 != NULL; temp1 = temp1->next){
    for(temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next){
         if(temp1->moyenne < temp2->moyenne){
            strcpy(id,temp1->id);
            strcpy(nom,temp1->nom);
            moy = temp1->moyenne;

            strcpy(temp1->id,temp2->id);
            strcpy(temp1->nom,temp2->nom);
            temp1->moyenne = temp2->moyenne;

            strcpy(temp2->id,id);
            strcpy(temp2->nom,nom);
            temp2->moyenne = moy;
         }
    }
   }
}


//Moyenne des etudiants:
void Moyenne(){
  etudiant *temp;
  temp = head;

  float sum = 0;
  float moy;
  int i = 0;

  while(temp != NULL){
    sum = sum + temp->moyenne;
    i++;
    temp = temp->next;
  }
  moy = sum/i;
                printf("\n---------------------\n");
                printf("MOYENNE = %.2f",moy);
                printf("\n--------------------\n");
}

//MENU:
void Menu(){
    printf("\n");
    printf("=========================================\n");
    printf("           GESTION DES ETUDIANTS         \n");
    printf("=========================================\n");

    printf("\n======== AJOUTER ETUDIANT ========\n");
    printf("1 -> Ajouter en tete\n");
    printf("2 -> Ajouter en queue\n");


    printf("\n======== MODIFICATION ========\n");
    printf("3 -> Changer moyenne d'un etudiant\n");
    printf("4 -> Changer le nom d'un etudiant\n");
    printf("5 -> Supprimer un etudiant\n");
    printf("6 -> Trier par moyenne\n");


    printf("\n======== RECHERCHE / INFO ========\n");
    printf("7 -> Rechercher un etudiant par ID\n");
    printf("8 -> Afficher un etudiant precis\n");


    printf("\n======== AFFICHAGE / STATS ========\n");
    printf("9 -> Afficher tous les etudiants\n");
    printf("10 -> Taille de la liste\n");
    printf("11 -> La moyenne de la class\n");

    printf("\n======== FICHIERS ========\n");
    printf("12 -> Sauvegarder la liste\n");
    printf("13 -> Charger liste depuis le fichier\n");
    printf("14 -> Verifier les anomalies");
    printf("\n=========================================\n");

    printf("0 -> Quitter\n");

    printf("Votre choix : ");
}

int main()
{
 int choix;
 char id[50];
 char nom[50];
 float moy;
 etudiant *etu;

 do{
     //ART MADE BY ASCIIART.EU + gpt
    printf("  _____ _______ _    _ _____  ______ _   _ _______     __  __          _   _          _____ ______ __  __ ______ _   _ _______         \n");
    printf(" / ____|__   __| |  | |  __ \\|  ____| \\ | |__   __|   |  \\/  |   /\\   | \\ | |   /\\   / ____|  ____|  \\/  |  ____| \\ | |__   __|\n");
    printf("| (___    | |  | |  | | |  | | |__  |  \\| |  | |      | \\  / |  /  \\  |  \\| |  /  \\ | |  __| |__  | \\  / | |__  |  \\| |  | |    \n");
    printf(" \\___ \\   | |  | |  | | |  | |  __| | . ` |  | |      | |\\/| | / /\\ \\ | . ` | / /\\ \\| | |_ |  __| | |\\/| |  __| | . ` |  | |   \n");
    printf(" ____) |  | |  | |__| | |__| | |____| |\\  |  | |      | |  | |/ ____ \\| |\\  |/ ____ \\ |__| | |____| |  | | |____| |\\  |  | |      \n");
    printf("|_____/   |_|   \\____/|_____/|______|_| \\_|  |_|      |_|  |_/_/    \\_\\_| \\_/_/    \\_\\_____|______|_|  |_|______|_| \\_|  |_|   \n");
    printf("\n");
    printf("                           Made by Bakr Amghar\n");
        Menu();
        scanf("%d", &choix);

        switch(choix){
             case 1:
                printf("\nID : ");
                scanf("%s",id);

                printf("Nom : ");
                scanf("%s",nom);

                printf("Moyenne : ");
                scanf("%f",&moy);

                InsertionTete(id,moy,nom);
                printf("\nEtudiant ajoute en tete.\n");
                Pause();
                break;

            case 2:
                printf("\nID : ");
                scanf("%s",id);

                printf("Nom : ");
                scanf("%s",nom);

                printf("Moyenne : ");
                scanf("%f",&moy);

                InsertionQueue(id,moy,nom);
                printf("\nEtudiant ajoute en queue.\n");
                Pause();
                break;

            case 3:
                ChangerMoy();
                Pause();
                break;

            case 4:
                ChangerNom();
                Pause();
                break;

            case 5:
                Supprimer();
                Pause();
                break;

            case 6:
                TrierParMoyenne();
                Pause();
                break;

            case 7:
                etu = RechercheID();
                AffichagePrecis(etu);
                Pause();
                break;


            case 8:
                etu = RechercheID();
                AffichagePrecis(etu);
                Pause();
                break;

            case 9:
                Affichage();
                Pause();
                break;

            case 10:
                 CalculeTaille();
                 Pause();
                 break;

            case 11:
                Moyenne();
                Pause();
                break;

            case 12:
                SauvegarderListe();
                Pause();
                break;

            case 13:
                LoadEtudiants();
                break;

            case 14:
                VerificationDesAnomalies();
                Pause();
                break;


            case 0:
                printf("\nProgramme termine.\n");
                break;

            default:
                printf("\nChoix invalideeeeee\n\n");
        }

    }while(choix != 0);
  free(etu);
return 0;
}
