#ifndef ETUDIANT_H
#define ETUDIANT_H

typedef struct etudiant {
    char id[50];
    char nom[50];
    float moyenne;
    struct etudiant *next;
    struct etudiant *prev;
} etudiant;

// Existing pointers
extern etudiant *head;
extern etudiant *queue;

// GUI-Friendly Functions
etudiant* CreerEtudiant(char id[], float moy, char nom[]);
void InsertionQueue(char id[], float moy, char nom[]);
void Supprimer_GUI(char id_to_delete[]);
etudiant* RechercheID_GUI(char id_to_find[]);

void Modifier_GUI(char old_id[], char new_id[], char new_nom[], float new_moy);

// File Functions (Make sure these match your utilities/file .c names)
void SauvegarderListe();
void LoadEtudiants();
void ViderListe();
#endif