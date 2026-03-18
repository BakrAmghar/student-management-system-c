#include "etudiant.h"
#include <stdio.h>

void SauvegarderListe() {
    FILE *f = fopen("etudiants.txt", "w");
    if (f == NULL) return;

    etudiant *temp = head;
    while (temp != NULL) {
        fprintf(f, "%s %s %.2f\n", temp->id, temp->nom, temp->moyenne);
        temp = temp->next;
    }
    fclose(f);
}

void LoadEtudiants() {
    FILE *f = fopen("etudiants.txt", "r");
    if (f == NULL) return;

    ViderListe(); // Clear old memory

    char id[50], nom[50];
    float moy;
    while (fscanf(f, "%s %s %f", id, nom, &moy) == 3) {
        InsertionQueue(id, moy, nom);
    }
    fclose(f);
}