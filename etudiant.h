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

// Existing Functions (UNTOUCHED)
etudiant* CreerEtudiant(char id[], float moy, char nom[]);
int InsertionQueue(char id[], float moy, char nom[]);
void Supprimer_GUI(char id_to_delete[]);
etudiant* RechercheID_GUI(char id_to_find[]);
int Modifier_GUI(char old_id[], char new_id[], char new_nom[], float new_moy);

// Existing File functions (UNTOUCHED)
void SauvegarderListe();
void LoadEtudiants();
void ViderListe();

// --- NEW FEATURE ONLY ---
void TrierListe(int mode); // 1: ID, 2: Name, 3: Grade
#endif