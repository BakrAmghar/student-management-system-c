#include <stdio.h>
#include "file.h"
#include "etudiant.h"

// Save to file
void SauvegarderListe(){
    FILE *fp = fopen("etudiants.txt","w");
    if(fp==NULL){printf("Erreur ouverture fichier\n"); return;}
    etudiant *temp=head;
    while(temp!=NULL){
        fprintf(fp,"%s %s %.2f\n",temp->id,temp->nom,temp->moyenne);
        temp=temp->next;
    }
    fclose(fp);
    printf("\nEtudiants sauvegarder en etudiants.txt\n");
}

// Load from file
void LoadEtudiants(){
    FILE *fp = fopen("etudiants.txt","r");
    if(fp==NULL){printf("Fichier non existant\n"); return;}
    char id[50], nom[50];
    float moy;
    while(fscanf(fp,"%s %s %f\n", id, nom, &moy)==3){
        InsertionQueue(id,moy,nom);
    }
    fclose(fp);
}
