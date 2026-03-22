#ifndef FILE_H
#define FILE_H

#include "etudiant.h" // this is included for the etudiant struct eeee

void SauvegarderListe();
void LoadEtudiants();
void ViderListe();
void enregistrer_log(const char* message); // Fixed: changed 'cont' to 'const'

#endif