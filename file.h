#ifndef FILE_H
#define FILE_H

#include "sqlite3.h"

// Database global handle
extern sqlite3 *db;

void InitDatabase();
void SauvegarderListe();
void LoadEtudiants();
void enregistrer_log(const char* message);

#endif