#include "file.h"
#include <stdio.h>
#include <time.h>

extern etudiant *head; // Needed so this file knows about the list from etudiant.c

void enregistrer_log(const char* message) {
    FILE *f = fopen("system_history.log", "a");
    if (f == NULL) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char s[64];
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", t);

    fprintf(f, "[%s] %s\n", s, message);
    fclose(f);
}

void SauvegarderListe() {
    FILE *f = fopen("etudiants.txt", "w");
    if (f == NULL) return;

    etudiant *temp = head;
    while (temp != NULL) {
        fprintf(f, "%s %s %.2f\n", temp->id, temp->nom, temp->moyenne);
        temp = temp->next;
    }
    fclose(f);
    enregistrer_log("DATA_FILE: etudiants.txt updated successfully.");
}

void LoadEtudiants() {
    FILE *f = fopen("etudiants.txt", "r");
    if (f == NULL) return;

    ViderListe();

    char id[50], nom[50];
    float moy;
    while (fscanf(f, "%s %s %f", id, nom, &moy) == 3) {
        InsertionQueue(id, moy, nom);
    }
    fclose(f);
    enregistrer_log("DATA_FILE: etudiants.txt loaded into memory.");
}