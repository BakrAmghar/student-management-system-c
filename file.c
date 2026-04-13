#include "file.h"
#include "etudiant.h"
#include <stdio.h>
#include <time.h>
#include "sqlite3.h"

sqlite3 *db = NULL;

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

void InitDatabase() {
    int rc = sqlite3_open("etudiants.db", &db);
    if (rc != SQLITE_OK) {
        enregistrer_log("DB_ERROR: Failed to open SQLite database.");
        return;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS ETUDIANTS("  \
                      "ID TEXT PRIMARY KEY NOT NULL," \
                      "NOM TEXT NOT NULL," \
                      "MOYENNE REAL NOT NULL);";

    char *zErrMsg = 0;
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
    } else {
        enregistrer_log("DB_SUCCESS: SQLite table ready.");
    }
}

void SauvegarderListe() {
    if (!db) return;

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, 0, NULL);
    sqlite3_exec(db, "DELETE FROM ETUDIANTS;", NULL, 0, NULL);

    etudiant *temp = head;
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ETUDIANTS (ID, NOM, MOYENNE) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    while (temp != NULL) {
        sqlite3_bind_text(stmt, 1, temp->id, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, temp->nom, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, temp->moyenne);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        temp = temp->next;
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
    enregistrer_log("DB_SYNC: SQLite database updated successfully.");
}

void LoadEtudiants() {
    if (!db) InitDatabase();
    ViderListe();

    sqlite3_stmt *res;
    const char *sql = "SELECT ID, NOM, MOYENNE FROM ETUDIANTS;";

    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) == SQLITE_OK) {
        while (sqlite3_step(res) == SQLITE_ROW) {
            char *id = (char*)sqlite3_column_text(res, 0);
            char *nom = (char*)sqlite3_column_text(res, 1);
            float moy = (float)sqlite3_column_double(res, 2);
            InsertionQueue(id, moy, nom);
        }
    }
    sqlite3_finalize(res);
    enregistrer_log("DB_LOAD: Records pulled from SQLite.");
}