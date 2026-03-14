#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "etudiant.h"

// Global pointers
etudiant *head = NULL;
etudiant *queue = NULL;

// Pause function
void Pause(){
    printf("\nAppuyez sur ENTER pour continuer...");
    getchar();
    getchar();
}

// Create student
etudiant* CreerEtudiant(char id[], float moy, char nom[]){
    etudiant *nv = (etudiant*)malloc(sizeof(etudiant));
    strcpy(nv->id,id);
    nv->moyenne = moy;
    strcpy(nv->nom, nom);
    nv->next = NULL;
    nv->prev = NULL;
    return nv;
}

// Insert at head
void InsertionTete(char id[], float moy, char nom[]){
    etudiant *nv = CreerEtudiant(id,moy,nom);
    if(head == NULL){
        head = queue = nv;
    } else {
        head->prev = nv;
        nv->next = head;
        head = nv;
    }
}

// Insert at tail
void InsertionQueue(char id[], float moy, char nom[]){
    etudiant *nv = CreerEtudiant(id,moy,nom);
    if(queue == NULL){
        head = queue = nv;
    } else {
        queue->next = nv;
        nv->prev = queue;
        queue = nv;
    }
}

// Display all students
void Affichage(){
    etudiant *temp = head;
    if(head != NULL){
        printf("\n\n=========================================\n");
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
    } else {
        printf("\n=== LISTE VIDE ===\n");
    }
}

// Calculate list size
void CalculeTaille(){
    etudiant *temp = head;
    int i=0;
    while(temp != NULL){
        i++;
        temp = temp->next;
    }
    printf("\n\n======TAILLE DE LA LISTE=======\n");
    printf("-- %d --\n", i);
    printf("=================================\n");
}

// Class average
void Moyenne(){
    etudiant *temp = head;
    float sum = 0;
    int i = 0;
    while(temp != NULL){
        sum += temp->moyenne;
        i++;
        temp = temp->next;
    }
    if(i>0){
        printf("\n---------------------\n");
        printf("MOYENNE = %.2f\n", sum/i);
        printf("---------------------\n");
    } else {
        printf("\nListe vide.\n");
    }
}

// Search by ID
etudiant* RechercheID(){
    etudiant *temp = head;
    char id[50];
    printf("ID : ");
    scanf("%s", id);
    while(temp != NULL){
        if(strcmp(temp->id,id)==0) return temp;
        temp = temp->next;
    }
    printf("\n=== Non trouvee ===\n");
    return NULL;
}

// Display single student
void AffichagePrecis(etudiant *nv){
    if(nv==NULL) return;
    printf("\n+---------------------------------------+\n");
    printf("| ID       : %-28s |\n", nv->id);
    printf("| Nom      : %-28s |\n", nv->nom);
    printf("| Moyenne  : %-28.2f |\n", nv->moyenne);
    printf("+---------------------------------------+\n");
}

// Change moyenne
void ChangerMoy(){
    etudiant *etu = RechercheID();
    if(etu == NULL) return;
    float nv_moy;
    printf("Nouvelle moyenne : ");
    scanf("%f",&nv_moy);
    etu->moyenne = nv_moy;
    printf("\nModification avec SUCCESS\n");
}

// Change name
void ChangerNom(){
    etudiant *etu = RechercheID();
    if(etu == NULL) return;
    char nv_nom[50];
    printf("Nouveau nom : ");
    scanf("%s", nv_nom);
    strcpy(etu->nom, nv_nom);
}

// Delete student
void Supprimer(){
    etudiant *etu = RechercheID();
    if(etu == NULL) return;
    if(etu->prev == NULL && etu->next == NULL){
        free(etu);
        head = queue = NULL;
        return;
    }
    if(etu->prev == NULL){
        head = etu->next;
        head->prev = NULL;
        free(etu);
        return;
    }
    if(etu->next == NULL){
        queue = etu->prev;
        queue->next = NULL;
        free(etu);
        return;
    }
    etu->prev->next = etu->next;
    etu->next->prev = etu->prev;
    free(etu);
}

// Sort by moyenne
void TrierParMoyenne(){
    etudiant *temp1, *temp2;
    char id[50], nom[50];
    float moy;
    for(temp1=head; temp1!=NULL; temp1=temp1->next){
        for(temp2=temp1->next; temp2!=NULL; temp2=temp2->next){
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

// Verify duplicates
void VerificationDesAnomalies(){
    etudiant *temp1, *temp2;
    int count=0;
    if(head==NULL){printf("\n--AUCUNE ANOMALIE--\n"); return;}
    for(temp1=head; temp1!=NULL; temp1=temp1->next){
        for(temp2=temp1->next; temp2!=NULL; temp2=temp2->next){
            if(strcmp(temp1->id,temp2->id)==0){
                printf("!!! Anomalie : ID en double --> %s (%s et %s)\n", temp1->id, temp1->nom, temp2->nom);
                count++;
            }
            if(strcmp(temp1->nom,temp2->nom)==0){
                printf("!!! Anomalie : Nom en double --> %s (%s et %s)\n", temp1->nom, temp1->id, temp2->id);
                count++;
            }
        }
    }
    if(count==0) printf("\n=== Aucune anomalie detecteeee :D ===\n");
}

