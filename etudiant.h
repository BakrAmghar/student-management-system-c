#ifndef ETUDIANT_H
#define ETUDIANT_H

typedef struct etudiant {
    char id[50];
    char nom[50];
    float moyenne;
    struct etudiant *next;
    struct etudiant *prev;
} etudiant;

extern etudiant *head;
extern etudiant *queue;

// Functions
etudiant* CreerEtudiant(char id[], float moy, char nom[]);
int InsertionQueue(char id[], float moy, char nom[]); // Returns int for ID check
void Supprimer_GUI(char id_to_delete[]);
etudiant* RechercheID_GUI(char id_to_find[]);
void Modifier_GUI(char old_id[], char new_id[], char new_nom[], float new_moy);

// File functions
void SauvegarderListe();
void LoadEtudiants();
void ViderListe();

#endif