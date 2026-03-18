#ifndef FILE_H
#define FILE_H

#include "etudiant.h" // Ensure this is included for the etudiant struct

void SauvegarderListe();
void LoadEtudiants();
void ViderListe();
void enregistrer_log(const char* message); // Fixed: changed 'cont' to 'const'

#endif