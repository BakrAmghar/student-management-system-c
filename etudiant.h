#ifndef ETUDIANT_H
#define ETUDIANT_H

typedef struct etudiant{
    char id[50];
    float moyenne;
    char nom[50];

    struct etudiant *next;
    struct etudiant *prev;
} etudiant;

// Global pointers
extern etudiant *head;
extern etudiant *queue;

// Function declarations
etudiant* CreerEtudiant(char id[], float moy, char nom[]);
void InsertionTete(char id[], float moy, char nom[]);
void InsertionQueue(char id[], float moy, char nom[]);
void Affichage();
void CalculeTaille();
void Moyenne();
etudiant* RechercheID();
etudiant* RechercheNom();
void AffichagePrecis(etudiant *nv);
void ChangerMoy();
void ChangerNom();
void Supprimer();
void TrierParMoyenne();
void VerificationDesAnomalies();
void SauvegarderListe();
void LoadEtudiants();
void Pause();

#endif
